#include <stdio.h>
#define MAX_ROWS 10
#define MAX_COLS 10
#define QUEUE_SIZE 100

typedef struct {
    int row, col;
} Position;

// BFS 遞迴尋找最短路徑
int bfs(char maze[MAX_ROWS][MAX_COLS], int start_row, int start_col, int dest_row, int dest_col, int rows, int cols, Position path[]) {
    Position queue[QUEUE_SIZE];
    int front = 0, rear = 0;
    int visited[MAX_ROWS][MAX_COLS] = {0};
    Position parent[MAX_ROWS][MAX_COLS];  // 父節點座標

    // 初始化 parent 的值為 {-1, -1}
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            parent[i][j] = (Position){-1, -1};
        }
    }

    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    // 將起點放入隊列並標記為已訪問
    queue[rear++] = (Position){start_row, start_col};
    visited[start_row][start_col] = 1;

    while (front < rear) {
        Position current = queue[front++];
        
        // 如果到達目標點，回溯父節點找到最短路徑
        if (current.row == dest_row && current.col == dest_col) {
            int path_index = 0;
            Position backtrack = current;

            // 回溯路徑
            while (backtrack.row != -1 && backtrack.col != -1) {
                path[path_index++] = backtrack;
                backtrack = parent[backtrack.row][backtrack.col];  // 取得上一步的父節點
            }
            return path_index;  // 回傳路徑長度
        }

        // 探索四個方向
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            // 確保新位置在迷宮範圍內且未被訪問
            if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols &&
                maze[new_row][new_col] != '#' && !visited[new_row][new_col]) {
                visited[new_row][new_col] = 1;
                queue[rear++] = (Position){new_row, new_col};
                parent[new_row][new_col] = current;  // 設定父節點
            }
        }
    }
    return 0;  // 找不到路徑
}


// 打印路徑
void printPath(Position path[], int path_length) {
    for (int i = path_length - 1; i >= 0; i--) {
        printf("(%d, %d)\n", path[i].row, path[i].col);
    }
}

int main() {
    char maze[MAX_ROWS][MAX_COLS] = {
        {'.', '.', '.', '#', '.'},
        {'.', '#', '.', '#', '.'},
        {'.', '#', '.', '.', '.'},
        {'.', '#', '#', '#', '.'},
        {'.', '.', '.', '.', '.'}
    };

    int rows = 5, cols = 5;
    int start_row = 0, start_col = 0;
    int dest_row = 4, dest_col = 4;

    Position path[QUEUE_SIZE];

    int path_length = bfs(maze, start_row, start_col, dest_row, dest_col, rows, cols, path);

    if (path_length > 0) {
        printf("最短路徑找到，長度為: %d\n", path_length);
        printPath(path, path_length);
    } else {
        printf("無法找到路徑\n");
    }

    return 0;
}
