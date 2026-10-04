//Programming Assignment VIII 
//Minimum Spanning Tree
//author : 陳沛安 
//date: 2024/12/18
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // 計算時間用的標頭檔

// 定義邊的結構
typedef struct {
    int src, dest, weight;
} Edge;

// 定義圖的結構
typedef struct {
    int V, E;
    Edge* edges;
} Graph;

// 定義聯集-查找的結構
typedef struct {
    int parent;
    int rank;
} Subset;

// 比較函數，用於排序邊
int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

// 查找集合
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// 合併兩個集合
void Union(Subset subsets[], int x, int y) {
    int rootX = find(subsets, x);
    int rootY = find(subsets, y);

    if (subsets[rootX].rank < subsets[rootY].rank) {
        subsets[rootX].parent = rootY;
    } else if (subsets[rootX].rank > subsets[rootY].rank) {
        subsets[rootY].parent = rootX;
    } else {
        subsets[rootY].parent = rootX;
        subsets[rootX].rank++;
    }
}

// 計算時間差
struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

// Kruskal's Algorithm
void KruskalMST(Graph* graph) {
    int V = graph->V;
    Edge result[V];
    int e = 0, i = 0;

    qsort(graph->edges, graph->E, sizeof(Edge), compareEdges);

    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));
    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < graph->E) {
        Edge nextEdge = graph->edges[i++];

        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        if (x != y) {
            result[e++] = nextEdge;
            Union(subsets, x, y);
        }
    }

    printf("Edges in MST:\n");
    int totalWeight = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight: %d\n", totalWeight);

    // 輸出 MST 的 .dot 文件
    FILE* dotFile = fopen("mst.dot", "w");
    fprintf(dotFile, "graph MST {\n");
    for (i = 0; i < e; i++) {
        fprintf(dotFile, "    %d -- %d [label=\"%d\"];\n", 
                result[i].src, result[i].dest, result[i].weight);
    }
    fprintf(dotFile, "}\n");
    fclose(dotFile);
    printf("MST graph saved to mst.dot\n");

    free(subsets);
}

// 輸出原始圖的 .dot 文件
void writeOriginalGraphDot(Graph* graph) {
    FILE* dotFile = fopen("graph.dot", "w");
    fprintf(dotFile, "graph OriginalGraph {\n");
    for (int i = 0; i < graph->E; i++) {
        fprintf(dotFile, "    %d -- %d [label=\"%d\"];\n", 
                graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
    }
    fprintf(dotFile, "}\n");
    fclose(dotFile);
    printf("Original graph saved to graph.dot\n");
}

// 從文件讀取圖
Graph* readGraphFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int V, E;
    fscanf(file, "%d %d", &V, &E);

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));

    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        fscanf(file, "%d %d %d", &src, &dest, &weight);
        graph->edges[i] = (Edge){src, dest, weight};
    }

    fclose(file);
    return graph;
}

// 主函數
int main() {
    const char* filename = "graph.txt"; // 圖的文件
    Graph* graph = readGraphFromFile(filename);

    printf("Graph read from file:\n");
    for (int i = 0; i < graph->E; i++) {
        printf("%d -- %d == %d\n", 
               graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
    }

    printf("\nSaving original graph to graph.dot...\n");
    writeOriginalGraphDot(graph);

    printf("\nComputing MST using Kruskal's Algorithm...\n");
    struct timespec start, end, duration;
    clock_gettime(CLOCK_MONOTONIC, &start); // 開始計時

    KruskalMST(graph);

    clock_gettime(CLOCK_MONOTONIC, &end); // 結束計時
    duration = diff(start, end);
    printf("Time taken: %ld.%09ld seconds\n", duration.tv_sec, duration.tv_nsec);

    free(graph->edges);
    free(graph);

    return 0;
}

