#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 10   // 定義迷宮的最大行數
#define MAX_COLS 10   // 定義迷宮的最大列數
#define QUEUE_SIZE 100 // 定義佇列的大小，用於BFS的節點存儲

// 定義位置結構，用於存儲節點的座標
typedef struct {
    int row, col;
} Position;

// 創建字符矩陣
char** createMatrix(int rows, int cols) {
    // 動態分配存儲行的指標數組
    char **matrix = (char **)malloc(rows * sizeof(char *));
    // 為每行分配存儲列的空間
    for (int i = 0; i < rows; i++) {
        matrix[i] = (char *)malloc(cols * sizeof(char));
    }

    // 初始化矩陣中的所有元素為字符 '0'，表示空白
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = '0';  // 默認填充為字符 '0'
        }
    }

    return matrix;
}

// 打印字符矩陣
void printMatrix(char **matrix, int rows, int cols) {
    // 按行列格式打印矩陣中的每個元素
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 將用戶輸入解析到矩陣中
void parseInputToMatrix(char **matrix, int rows) {
    // 根據輸入的行數進行解析
    for (int i = 0; i < rows; i++) {
        int index;  // 用於存儲單元格索引
        char ch;    // 用於存儲字符類型
        
        // 提示用戶輸入每行的元素，直到遇到 '0' 結束
        printf("Please enter the elements for row %d (end with '0'):\n", i + 1);
        
        while (1) {
            scanf("%d", &index); // 讀取位置索引
            if (index == 0) break; // 如果輸入是0，則停止這行的讀取
            if (index < 1 || index > MAX_COLS) { // 檢查索引範圍是否合法
                printf("Index out of bounds. Please enter an index between 1 and %d.\n", MAX_COLS);
                continue;
            }
            scanf(" %c", &ch); // 讀取指定位置的字符
            matrix[i][index - 1] = ch; // 更新矩陣對應位置，索引減1是因為C語言中的索引從0開始
        }
    }
}

// BFS（廣度優先搜索） 遍歷迷宮來尋找最短路徑
int bfs(char maze[MAX_ROWS][MAX_COLS], int start_row, int start_col, int dest_row, int dest_col, int rows, int cols, Position path[]) {
    Position queue[QUEUE_SIZE];  // 佇列用於存儲即將訪問的節點
    int front = 0, rear = 0;     // 佇列的前後指針
    int visited[MAX_ROWS][MAX_COLS] = {0}; // 訪問矩陣，用來標記是否已經訪問過某位置
    Position parent[MAX_ROWS][MAX_COLS];  // 用來存儲節點的父節點，以便於回溯路徑

    // 初始化所有父節點為 {-1, -1}，表示還沒有父節點
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            parent[i][j] = (Position){-1, -1};
        }
    }

    // 定義四個移動方向：右、下、左、上
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    // 將起點放入佇列並標記為已訪問
    queue[rear++] = (Position){start_row, start_col};
    visited[start_row][start_col] = 1;

    // 當佇列不為空時，繼續遍歷
    while (front < rear) {
        Position current = queue[front++];  // 取出佇列中的下一個節點
        
        // 如果到達目標點，則回溯父節點來找到最短路徑
        if (current.row == dest_row && current.col == dest_col) {
            int path_index = 0;
            Position backtrack = current;

            // 回溯路徑，直到找到起點
            while (backtrack.row != -1 && backtrack.col != -1) {
                path[path_index++] = backtrack;
                backtrack = parent[backtrack.row][backtrack.col];  // 取得上一步的父節點
            }
            return path_index;  // 回傳路徑長度
        }

        // 探索四個方向的相鄰節點
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            // 確保新位置在迷宮範圍內，並且不是障礙物且未被訪問
            if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols &&
                maze[new_row][new_col] != 'x' && !visited[new_row][new_col]) {
                visited[new_row][new_col] = 1;  // 標記為已訪問
                queue[rear++] = (Position){new_row, new_col};  // 將該節點加入佇列
                parent[new_row][new_col] = current;  // 設定父節點
            }
        }
    }
    return 0;  // 如果找不到路徑，返回0
}

// 打印找到的路徑
void printPath(Position path[], int path_length) {
    for (int i = path_length - 1; i >= 0; i--) {  // 從終點回溯到起點
        printf("(%d,%d)", path[i].row + 1, path[i].col + 1);  // 轉換為 1 索引格式
        if (i > 0) printf(" -> ");  // 添加箭頭來顯示路徑
    }
    printf("\n");
}

// 在迷宮中標記並顯示找到的路徑
void printMazeWithPath(char maze[MAX_ROWS][MAX_COLS], Position path[], int length, int rows, int cols) {
    char marked_maze[MAX_ROWS][MAX_COLS];  // 複製迷宮
    memcpy(marked_maze, maze, sizeof(marked_maze));
    
    // 用數字標記路徑
    for (int i = 0; i < length; i++) {
        int row = path[length - 1 - i].row; 
        int col = path[length - 1 - i].col;
        marked_maze[row][col] = '0' + i;  // 將路徑上的每一步標記為編號
    }
    
    // 起點標記為 's'，終點標記為 't'
    marked_maze[path[0].row][path[0].col] = 't';
    marked_maze[path[length - 1].row][path[length - 1].col] = 's';
    
    // 打印標記後的迷宮
    printf("The path can also be shown in the rectangular matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", marked_maze[i][j]);
        }
        printf("\n");
    }
}

// 釋放動態分配的矩陣內存
void freeCharMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);  // 釋放每行的內存
    }
    free(matrix);  // 釋放行數組的內存
}

int main() {
    int rows, cols;
    char **matrix;

    // 讀取用戶輸入的迷宮大小
    printf("Please enter the size of the maze (rows and columns):\n");
    if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        // 檢查輸入是否合法
        fprintf(stderr, "Invalid maze size. Please enter valid dimensions (1-%d for rows, 1-%d for columns).\n", MAX_ROWS, MAX_COLS);
        return 1;
    }

    // 創建字符矩陣
    matrix = createMatrix(rows, cols);
    
    // 解析用戶輸入並更新矩陣
    parseInputToMatrix(matrix, rows);
    
    // 打印初始迷宮
    printf("The maze M:\n");
    printMatrix(matrix, rows, cols);

    char maze[MAX_ROWS][MAX_COLS];

    // 將字符矩陣複製到迷宮陣列中
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = matrix[i][j];
        }
    }

    // 搜索起點和終點
    int start_row, start_col, dest_row, dest_col;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 's') {  // 找到起點
                start_row = i;
                start_col = j;
            } else if (maze[i][j] == 't') {  // 找到終點
                dest_row = i;
                dest_col = j;
            }
        }
    }    
    Position path[QUEUE_SIZE];  // 儲存路徑的陣列
    int path_length = bfs(maze, start_row, start_col, dest_row, dest_col, rows, cols, path);

    // 如果找到路徑，打印路徑並顯示迷宮中的路徑
    if (path_length > 0) {
        printPath(path, path_length);
        printMazeWithPath(maze, path, path_length, rows, cols);
    } else {
        printf("No path found from S to T.\n");
    }  

    // 釋放矩陣內存
    freeCharMatrix(matrix, rows);
    
    return 0;
}

