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

// 輸出矩陣 (常規格式)
void printMatrix(char **matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 列表格式輸出
void printListFormat(char **matrix, int rows, int cols) {
    int total_memory_units = 0;

    for (int i = 0; i < rows; i++) {
        printf("Row %d:", i + 1);
        int memory_units_row = 1;  // 初始化，至少需要1個單位來存儲行列表的指標
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != 0) {
                printf(" (%d, %d)", j + 1, matrix[i][j]);
                memory_units_row += 2;  // 每個元素需要2個單位
            }
        }
        printf("\n");
        total_memory_units += memory_units_row;  // 累加每行的內存單位
    }

    printf("Total memory units required: %d\n", total_memory_units);
}

// 矩陣相加
char** addMatrices(char **matrixA, char **matrixB, int rows, int cols) {
    char **result = createCharMatrix(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }

    return result;
}

// 矩陣相乘
char** multiplyMatrices(char **matrixA, int rowsA, int colsA, char **matrixB, int rowsB, int colsB) {
    if (colsA != rowsB) {
        printf("Matrix dimensions are not compatible for multiplication.\n");
        return NULL;
    }

    char **result = createCharMatrix(rowsA, colsB);

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}

// 釋放矩陣內存
void freeMatrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int rowsA, colsA, rowsB, colsB;
    char **matrixA, **matrixB, **result;

    // 讀取矩陣A的大小
    printf("Please enter the size of matrix A (rows and columns):\n");
    if (scanf("%d %d", &rowsA, &colsA) != 2 || rowsA <= 0 || rowsA > MAX_ROWS || colsA <= 0 || colsA > MAX_COLS) {
        fprintf(stderr, "Invalid matrix size. Please enter valid dimensions (1-%d for rows, 1-%d for columns).\n", MAX_ROWS, MAX_COLS);
        return 1;
    }

    // 創建矩陣A
    matrixA = createCharMatrix(rowsA, colsA);
    parseInputToMatrix(matrixA, rowsA);
    
    // 讀取矩陣B的大小
    printf("Please enter the size of matrix B (rows and columns):\n");
    if (scanf("%d %d", &rowsB, &colsB) != 2 || rowsB <= 0 || rowsB > MAX_ROWS || colsB <= 0 || colsB > MAX_COLS) {
        fprintf(stderr, "Invalid matrix size. Please enter valid dimensions (1-%d for rows, 1-%d for columns).\n", MAX_ROWS, MAX_COLS);
        freeMatrix(matrixA, rowsA);
        return 1;
    }

    // 創建矩陣B
    matrixB = createCharMatrix(rowsB, colsB);
    parseInputToMatrix(matrixB, rowsB);
    
    // 輸出兩個矩陣
    printf("Matrix A:\n");
    printMatrix(matrixA, rowsA, colsA);
    printf("Matrix B:\n");
    printMatrix(matrixB, rowsB, colsB);

    // 矩陣相加
    if (rowsA == rowsB && colsA == colsB) {
        result = addMatrices(matrixA, matrixB, rowsA, colsA);
        printf("A + B:\n");
        printMatrix(result, rowsA, colsA);
        freeMatrix(result, rowsA);
    } else {
        printf("Cannot add matrices: dimensions do not match.\n");
    }

    // 矩陣相乘
    result = multiplyMatrices(matrixA, rowsA, colsA, matrixB, rowsB, colsB);
    if (result != NULL) {
        printf("A * B:\n");
        printMatrix(result, rowsA, colsB);
        freeMatrix(result, rowsA);
    }

    // 釋放內存
    freeMatrix(matrixA, rowsA);
    freeMatrix(matrixB, rowsB);

    return 0;
}

