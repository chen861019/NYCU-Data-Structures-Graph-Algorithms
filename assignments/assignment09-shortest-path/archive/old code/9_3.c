// Programming Assignment IX 
// Shortest Path for All Vertices using Dijkstra's Algorithm
// author : 陳沛安
// date: 2024/12/31

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define INF INT_MAX

typedef struct {
    int dest, weight;
} Edge;

typedef struct Node {
    Edge edge;
    struct Node* next;
} Node;

typedef struct {
    int V;
    Node** adjList;
} Graph;

// 建立圖
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (Node**)malloc(V * sizeof(Node*));
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// 新增邊
void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->edge.dest = dest;
    newNode->edge.weight = weight;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // 加入反向邊以處理無向圖
    newNode = (Node*)malloc(sizeof(Node));
    newNode->edge.dest = src;
    newNode->edge.weight = weight;
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

// 釋放圖的記憶體
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        Node* node = graph->adjList[i];
        while (node) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);
}

// Dijkstra 演算法
void dijkstra(Graph* graph, int src, int* parent, int* dist) {
    int V = graph->V;
    bool visited[V];
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = -1;
        for (int i = 0; i < V; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }

        if (u == -1) break; // 無法再訪問節點時提前退出
        visited[u] = true;

        Node* pCrawl = graph->adjList[u];
        while (pCrawl != NULL) {
            int v = pCrawl->edge.dest;
            if (!visited[v] && dist[u] != INF && dist[u] + pCrawl->edge.weight < dist[v]) {
                dist[v] = dist[u] + pCrawl->edge.weight;
                parent[v] = u;
            }
            pCrawl = pCrawl->next;
        }
    }
}

// 輸出原始圖到 DOT 檔案
void writeOriginalGraphDOT(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph OriginalGraph {\n");
    fprintf(file, "  node [shape=circle];\n");

    for (int u = 0; u < graph->V; u++) {
        Node* pCrawl = graph->adjList[u];
        while (pCrawl != NULL) {
            if (u < pCrawl->edge.dest) { // 確保每條無向邊只輸出一次
                fprintf(file, "    %d -- %d [label=\"%d\"];\n", u, pCrawl->edge.dest, pCrawl->edge.weight);
            }
            pCrawl = pCrawl->next;
        }
    }

    fprintf(file, "  label=\"Original Graph\";\n");
    fprintf(file, "}\n");

    fclose(file);
    printf("Original graph DOT file written: %s\n", filename);
}

// 輸出最短路徑樹到 DOT 檔案
void writeShortestPathTreeDOT(int* parent, int* dist, int V, int src, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph ShortestPathTree {\n");
    fprintf(file, "  node [shape=circle];\n");

    for (int i = 0; i < V; i++) {
        if (parent[i] != -1) {
            fprintf(file, "    %d -- %d [label=\"%d\"];\n", parent[i], i, dist[i] - dist[parent[i]]);
        }
    }

    fprintf(file, "  label=\"Shortest Path Tree from Vertex %d\";\n", src);
    fprintf(file, "}\n");

    fclose(file);
    printf("Shortest path tree DOT file written: %s\n", filename);
}

// 從檔案讀取圖
Graph* readGraphFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int V, E;
    fscanf(file, "%d %d", &V, &E);

    Graph* graph = createGraph(V);
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        fscanf(file, "%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    fclose(file);
    return graph;
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

// 主函數
int main() {
    const char* inputFile = "graph.txt";
    const char* originalGraphDOT = "original_graph.dot";

    Graph* graph = readGraphFromFile(inputFile);
    writeOriginalGraphDOT(graph, originalGraphDOT);

    int V = graph->V;
    int* parent = (int*)malloc(V * sizeof(int));
    int* dist = (int*)malloc(V * sizeof(int));

    // 對所有頂點執行 Dijkstra 演算法
    struct timespec start, end, duration;
    clock_gettime(CLOCK_MONOTONIC, &start); // 開始計時
    for (int src = 0; src < V; src++) {
        dijkstra(graph, src, parent, dist);

        // 輸出最短路徑樹到 DOT 檔案
        char filename[50];
        sprintf(filename, "shortest_path_tree_%d.dot", src);
        writeShortestPathTreeDOT(parent, dist, V, src, filename);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end); // 結束計時
    duration = diff(start, end);
    printf("Time taken: %ld.%09ld seconds\n", duration.tv_sec, duration.tv_nsec);

    // 釋放記憶體
    freeGraph(graph);
    free(parent);
    free(dist);
    
    system ("pause");

    return 0;
}

