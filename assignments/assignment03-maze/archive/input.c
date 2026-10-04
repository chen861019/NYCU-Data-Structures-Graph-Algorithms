#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ROWS 10
#define MAX_COLS 10
#define MAX_INPUT_LINES 50
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
void fillMaze(char maze[MAX_ROWS][MAX_COLS], char input_data[][MAX_INPUT_LENGTH], int rows, int cols) {
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

int main() {
    char input_data[MAX_INPUT_LINES][MAX_INPUT_LENGTH];
    int line_count = 0;

    // 請求用戶輸入
    printf("請輸入迷宮的大小 (行 列) 和填充資料，每行結束請按 Enter，最後以 'END' 結束輸入:\n");

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

    int rows, cols;

    // 讀取迷宮的大小
    sscanf(input_data[0], "%d %d", &rows, &cols);

    char maze[MAX_ROWS][MAX_COLS];

    initializeMaze(maze, rows, cols);
    fillMaze(maze, input_data + 1, rows, cols); // 只傳遞填充數據的部分
    printMaze(maze, rows, cols);

    return 0;
}

