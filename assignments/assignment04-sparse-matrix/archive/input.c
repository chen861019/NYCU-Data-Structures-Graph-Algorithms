#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 100
#define MAX_COLS 100

// 創建字符矩陣
char** createCharMatrix(int rows, int cols) {
    char **matrix = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (char*)calloc(cols, sizeof(char)); // 初始化為0
    }
    return matrix;
}

// 將輸入解析到矩陣中
void parseInputToMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        int index, value;
        
        // 根據輸入指定行的內容進行填充
        printf("Please enter the elements for row %d (end with '0'):\n", i + 1);
        
        // 讀取每行輸入，直到遇到0
        while (1) {
            scanf("%d", &index); // 讀取位置索引
            if (index == 0) break; // 如果是0，則停止讀取這一行
            if (index < 1 || index > MAX_COLS) {
                printf("Index out of bounds. Please enter an index between 1 and %d.\n", MAX_COLS);
                continue;
            }
            scanf("%d", &value); // 讀取指定位置的數值
            matrix[i][index - 1] = value; // 更新矩陣對應位置，索引減1是因為C的索引從0開始
        }
    }
}

// 輸出矩陣
void printMatrix(char **matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 釋放矩陣內存
void freeMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int rows, cols;
    char **matrix;

    // 讀取矩陣大小
    printf("Please enter the size of the matrix (rows and columns):\n");
    if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || rows > MAX_ROWS || cols <= 0 || cols > MAX_COLS) {
        fprintf(stderr, "Invalid matrix size. Please enter valid dimensions (1-%d for rows, 1-%d for columns).\n", MAX_ROWS, MAX_COLS);
        return 1;
    }

    // 創建字符矩陣
    matrix = createCharMa trix(rows, cols);
    
    // 解析用戶輸入並更新矩陣
    parseInputToMatrix(matrix, rows);
    
    // 輸出矩陣
    printMatrix(matrix, rows, cols);
    
    // 釋放內存
    freeMatrix(matrix, rows);
    
    return 0;
}

