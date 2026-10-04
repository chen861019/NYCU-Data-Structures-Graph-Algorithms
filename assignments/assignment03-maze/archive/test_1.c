#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define QUEUE_SIZE 100
#define MAX_INPUT_LINES 100
#define MAX_INPUT_LENGTH 100

typedef struct {
    int row, col;
} Position;

// 創建字符矩陣
char** createCharMatrix(int rows, int cols) {
    char **matrix = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (char *)malloc(cols * sizeof(char));
    }

    // 初始化矩陣中的所有元素為 '0'
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = '0';  // 默認填充為字符 '0'
        }
    }

    return matrix;
}

// 打印字符矩陣
void printCharMatrix(char **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 將輸入解析到矩陣中
void parseInputToMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        int index;
        char ch;
        
        // 根據輸入指定行的內容進行填充
        printf("Please enter the elements for row %d (end with '0'):\n", i + 1);
        
        // 讀取每行輸入，直到遇到換行符
        while (1) {
            scanf("%d", &index); // 讀取位置索引
            if (index == 0) break; // 如果是0，則停止讀取這一行
            scanf(" %c", &ch); // 讀取指定位置的字符
            matrix[i][index - 1] = ch; // 更新矩陣對應位置，索引減1是因為C的索引從0開始
        }
    }
}

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
                maze[new_row][new_col] != 'x' && !visited[new_row][new_col]) {
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
        printf("(%d,%d)", path[i].row + 1, path[i].col + 1);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

// 在迷宮中標記路徑
void printMazeWithPath(char maze[MAX_ROWS][MAX_COLS], Position path[], int length, int rows, int cols) {
    char marked_maze[MAX_ROWS][MAX_COLS];
    memcpy(marked_maze, maze, sizeof(marked_maze));
    
    for (int i = 0; i < length; i++) {
        int row = path[length - 1 - i].row; 
        int col = path[length - 1 - i].col;
        marked_maze[row][col] = '0' + i; 
    }
    
    marked_maze[path[0].row][path[0].col] = 't';
    marked_maze[path[length - 1].row][path[length - 1].col] = 's';
    
    printf("The path can also be shown in the rectangular matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", marked_maze[i][j]);
        }
        printf("\n");
    }
}

// 釋放矩陣內存
void freeCharMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int rows, cols;
    char **matrix;

#ifdef DEBUG
    char input_data[MAX_INPUT_LINES][MAX_INPUT_LENGTH] = {
        "4 5",          // 迷宮的大小
        "2 x 0",       // 第二行，第二個位置換成 x
        "2 s 4 x 0",   // 第三行，第二個位置換成 s，第四個位置換成 x
        "3 x 0",       // 第四行，第三個位置換成 x
        "2 x 4 x 5 t 0" // 第五行，第二個位置換成 x，第四個位置換成 x，第五個位置換成 t
    };
#else
    // 讀取矩陣大小
    printf("Please enter the size of the maze (rows and columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error reading maze size.\n");
        return 1;
    }

    // 創建字符矩陣
    matrix = createCharMatrix(rows, cols);
    
    // 解析用戶輸入並更新矩陣
    parseInputToMatrix(matrix, rows);
    
    // 打印矩陣
    printf("The maze M:\n");
    printCharMatrix(matrix, rows, cols);
#endif

    char maze[MAX_ROWS][MAX_COLS];

    // 初始化迷宮
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = matrix[i][j];
        }
    }

    // 輸入起點和終點
    int start_row, start_col, dest_row, dest_col;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 's') {
                start_row = i;
                start_col = j;
            } else if (maze[i][j] == 't') {
                dest_row = i;
                dest_col = j;
            }
        }
    }

    printf("start:(%d,%d)\n", start_row + 1, start_col + 1);
    printf("dest:(%d,%d)\n", dest_row + 1, dest_col + 1);
    
    Position path[QUEUE_SIZE];
    int path_length = bfs(maze, start_row, start_col, dest_row, dest_col, rows, cols, path);

    if (path_length > 0) {
        printPath(path, path_length);
        printMazeWithPath(maze, path, path_length, rows, cols);
    } else {
        printf("No path found from S to T.\n");
    }  

    // 釋放內存
    freeCharMatrix(matrix, rows);

    return 0;
}

