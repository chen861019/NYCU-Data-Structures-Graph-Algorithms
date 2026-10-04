// Programming Assignment IX
// Shortest Path using Dijkstra's Algorithm
// author : 陳沛安
// date: 2024/12/31

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX // 定義無限大

// 邊的結構
typedef struct {
    int dest, weight;
} Edge;

// 鄰接表的節點
typedef struct Node {
    Edge edge;
    struct Node* next;
} Node;

// 鄰接表的圖結構
typedef struct {
    int V;
    Node** adjList;
} Graph;

// 最小堆積 (Priority Queue) 的節點
typedef struct {
    int vertex, dist;
} HeapNode;

// 最小堆積結構
typedef struct {
    int capacity, size;
    int* pos; // 記錄頂點的位置
    HeapNode** nodes;
} MinHeap;

// 建立新圖
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (Node**)malloc(V * sizeof(Node*));
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// 新增邊到圖中 (有向邊)
void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->edge.dest = dest;
    newNode->edge.weight = weight;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

// 建立最小堆積節點
HeapNode* createHeapNode(int v, int dist) {
    HeapNode* node = (HeapNode*)malloc(sizeof(HeapNode));
    node->vertex = v;
    node->dist = dist;
    return node;
}

// 建立空的最小堆積
MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->pos = (int*)malloc(capacity * sizeof(int));
    heap->nodes = (HeapNode**)malloc(capacity * sizeof(HeapNode*));
    return heap;
}

// 交換兩個堆積節點
void swapHeapNodes(HeapNode** a, HeapNode** b) {
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// 調整堆積 (維護最小堆)
void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left]->dist < heap->nodes[smallest]->dist)
        smallest = left;

    if (right < heap->size && heap->nodes[right]->dist < heap->nodes[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        // 更新位置
        heap->pos[heap->nodes[smallest]->vertex] = idx;
        heap->pos[heap->nodes[idx]->vertex] = smallest;

        // 交換節點
        swapHeapNodes(&heap->nodes[smallest], &heap->nodes[idx]);

        minHeapify(heap, smallest);
    }
}

// 減少堆積中的距離值
void decreaseKey(MinHeap* heap, int v, int dist) {
    int i = heap->pos[v];
    heap->nodes[i]->dist = dist;

    while (i && heap->nodes[i]->dist < heap->nodes[(i - 1) / 2]->dist) {
        heap->pos[heap->nodes[i]->vertex] = (i - 1) / 2;
        heap->pos[heap->nodes[(i - 1) / 2]->vertex] = i;
        swapHeapNodes(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// 移除堆積中的最小節點
HeapNode* extractMin(MinHeap* heap) {
    if (heap->size == 0)
        return NULL;

    HeapNode* root = heap->nodes[0];
    HeapNode* lastNode = heap->nodes[heap->size - 1];
    heap->nodes[0] = lastNode;

    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[lastNode->vertex] = 0;

    --heap->size;
    minHeapify(heap, 0);

    return root;
}

// Dijkstra 演算法
void dijkstra(Graph* graph, int src) {
    int V = graph->V;
    int dist[V];
    int parent[V]; // 紀錄最短路徑樹

    MinHeap* heap = createMinHeap(V);

    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        parent[v] = -1;
        heap->nodes[v] = createHeapNode(v, dist[v]);
        heap->pos[v] = v;
    }

    heap->nodes[src] = createHeapNode(src, 0);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    heap->size = V;

    while (heap->size > 0) {
        HeapNode* heapNode = extractMin(heap);
        int u = heapNode->vertex;

        Node* pCrawl = graph->adjList[u];
        while (pCrawl != NULL) {
            int v = pCrawl->edge.dest;

            if (heap->pos[v] < heap->size && dist[u] != INF &&
                pCrawl->edge.weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->edge.weight;
                parent[v] = u;
                decreaseKey(heap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("Shortest Path Tree:\n");
    for (int i = 0; i < V; i++) {
        if (parent[i] != -1)
            printf("%d -- %d == %d\n", parent[i], i, dist[i] - dist[parent[i]]);
    }
    free(heap);
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
    const char* filename = "graph.txt";
    Graph* graph = readGraphFromFile(filename);

    int source;
    printf("Enter source vertex: ");
    scanf("%d", &source);

    dijkstra(graph, source);

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

    return 0;
}

