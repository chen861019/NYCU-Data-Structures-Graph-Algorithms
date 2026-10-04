#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 隨機生成圖並寫入 graph.txt
void generateGraphFile(const char* filename, int V, int E) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d %d\n", V, E);

    srand(time(NULL));
    for (int i = 0; i < E; i++) {
        int src = rand() % V;
        int dest = rand() % V;
        while (dest == src) { // 避免自環
            dest = rand() % V;
        }
        int weight = rand() % 50 + 1; // 權重在 1 到 50 之間
        fprintf(file, "%d %d %d\n", src, dest, weight);
    }

    fclose(file);
    printf("Graph with %d vertices and %d edges saved to %s\n", V, E, filename);
}

int main() {
    int V, E;

    printf("Enter number of vertices: ");
    scanf("%d", &V);

    printf("Enter number of edges: ");
    scanf("%d", &E);

    if (E > V * (V - 1) / 2) {
        printf("Too many edges for %d vertices. Maximum possible is %d.\n", V, V * (V - 1) / 2);
        return 1;
    }

    generateGraphFile("graph.txt", V, E);

    return 0;
}
