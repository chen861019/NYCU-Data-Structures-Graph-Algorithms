// Programming Assignment VIII
// Shortest Path for All Vertices using Dijkstra's Algorithm with DOT Output
// author : 陳沛安
// date: 2024/12/18

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX // 定義無限大

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

// 新增邊到圖中
void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->edge.dest = dest;
    newNode->edge.weight = weight;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
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

// 輸出最短路徑樹到 DOT 文件
void writeDOT(int* parent, int V, int src, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "digraph ShortestPathTree {\n");
    fprintf(file, "  node [shape=circle];\n");

    for (int i = 0; i < V; i++) {
        if (parent[i] != -1) {
            fprintf(file, "  %d -> %d;\n", parent[i], i);
        }
    }

    fprintf(file, "  label=\"Shortest Path Tree from Vertex %d\";\n", src);
    fprintf(file, "}\n");

    fclose(file);
    printf("DOT file written: %s\n", filename);
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

    Graph* graph = createGraph(V);

    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        fscanf(file, "%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    fclose(file);
    return graph;
}

// 主函數
int main() {
    const char* inputFile = "graph.txt";
    Graph* graph = readGraphFromFile(inputFile);

    int V = graph->V;
    int* parent = (int*)malloc(V * sizeof(int));
    int* dist = (int*)malloc(V * sizeof(int));

    // 對所有頂點運行 Dijkstra 演算法
    for (int src = 0; src < V; src++) {
        dijkstra(graph, src, parent, dist);

        // 輸出最短路徑樹到 DOT 文件
        char filename[50];
        sprintf(filename, "shortest_path_tree_%d.dot", src);
        writeDOT(parent, V, src, filename);
    }

    // 清理記憶體
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
    free(parent);
    free(dist);

    return 0;
}

