#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define QUEUE_SIZE MAX_ROWS * MAX_COLS
#define MAX_INPUT_LINES 100
#define MAX_INPUT_LENGTH 100

// 初始化迷宮為 '0'
void initializeMaze(char maze[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = '0';
        }
    }
}

// 根據輸入資料填入迷宮
void fillMaze(char maze[MAX_ROWS][MAX_COLS], char input_data[MAX_INPUT_LINES][MAX_INPUT_LENGTH], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        int index;
        char character;

        // 解析每一行的資料
        char *token = strtok(input_data[i], " ");
        while (token != NULL) {
            index = atoi(token);  // 取得位置
            if (index == 0) break; // 遇到 '0' 結束

            // 獲取下一個字元
            token = strtok(NULL, " ");
            if (token != NULL) {
                character = token[0]; // 取得字元
                if (index > 0 && index <= cols) { // 確保索引有效
                    maze[i][index-1] = character; // 填入字元
                }
            }
            // 繼續解析
            token = strtok(NULL, " ");
        }
    }
}

// 打印迷宮
void printMaze(char maze[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

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
        printf("(%d,%d)", path[i].row+1, path[i].col+1);
    }
    printf("\n");
}

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

int main() {
#ifdef DEBUG
	char input_data[MAX_INPUT_LINES][MAX_INPUT_LENGTH] = {
        "4 5",          // 迷宮的大小
        "2 x 0",       // 第二行，第二個位置換成 x
        "2 s 4 x 0",   // 第三行，第二個位置換成 s，第四個位置換成 x
        "3 x 0",       // 第四行，第三個位置換成 x
        "2 x 4 x 5 t 0" // 第五行，第二個位置換成 x，第四個位置換成 x，第五個位置換成 t
    };
#else
    char input_data[MAX_INPUT_LINES][MAX_INPUT_LENGTH];
    int line_count = 0;

    // 請求用戶輸入
    printf("Please enter the maze size and fill data step by step:\n");
	printf("1. On the first line, enter two numbers representing the number of rows and columns of the maze.\n");
	printf("2. Starting from the second line, input the position and character pairs in the format: position character position character ... 0\n");
	printf("   For example: 2 x 4 s 0 means placing 'x' at position 2 and 's' at position 4, and ending the line with '0'.\n");
	printf("3. On the last line, input 'END' to indicate the end of the input.\n");
	printf("Please begin your input, and press Enter after each line:\n");

    // 逐行讀取輸入，直到遇到 'END'
    while (fgets(input_data[line_count], MAX_INPUT_LENGTH, stdin)) {
        // 去除輸入中的換行符號
        input_data[line_count][strcspn(input_data[line_count], "\n")] = 0;

        // 檢查是否是 "END"
        if (strcmp(input_data[line_count], "END") == 0) {
            break;
        }
        line_count++;
    }

    // 檢查輸入內容
    printf("\n您輸入的內容是:\n");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", input_data[i]);
    }
#endif
    int rows, cols;
    sscanf(input_data[0], "%d %d", &rows, &cols);

    char maze[MAX_ROWS][MAX_COLS];
    initializeMaze(maze, rows, cols);
    fillMaze(maze, input_data + 1, rows, cols);
    printMaze(maze, rows, cols);

    int start_row, start_col, dest_row, dest_col;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
        	//printf("%d,%d:%c\n",i,j,maze[i][j]);
            if (maze[i][j] == 's') {
                start_row = i;
                start_col = j;
            } else if (maze[i][j] == 't') {
                dest_row = i;
                dest_col = j;
            }
        }
    }
	printf("start:(%d,%d)\n",start_row+1,start_col+1);
	printf("dest:(%d,%d)\n",dest_row+1,dest_col+1);
    Position path[QUEUE_SIZE];
    int path_length = bfs(maze, start_row, start_col, dest_row, dest_col, rows, cols, path);

    if (path_length > 0) {
        printPath(path, path_length);
        printMazeWithPath(maze, path, path_length, rows, cols);
    } else {
        printf("No path found from S to T.\n");
    }

    return 0;
}

