#include <stdio.h>
#include <stdlib.h>

// 節點結構，用於儲存非零元素
typedef struct Node {
    int col; // 列索引
    int value; // 值
    struct Node *Link; // 指向下一個節點
} Node, *PNODE;

// 使用 SparseMatrix 結構表示稀疏矩陣
typedef struct {
    int rows; // 行數
    int cols; // 列數
    PNODE *rowsList; // 每行的鏈結串列
} SparseMatrix;

// 創建新節點
PNODE createNode(int col, int value) {
    PNODE newNode = (PNODE)malloc(sizeof(Node));
    newNode->col = col;
    newNode->value = value;
    newNode->Link = NULL;
    return newNode;
}

// 創建鏈結串列矩陣
SparseMatrix createSparseMatrix(int rows, int cols) {
    SparseMatrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.rowsList = (PNODE*)malloc(rows * sizeof(PNODE));
    for (int i = 0; i < rows; i++) {
        matrix.rowsList[i] = NULL; // 初始化為NULL
    }
    return matrix;
}

// 釋放矩陣記憶體
void freeMatrix(PNODE *rowsList, int rows) {
    for (int i = 0; i < rows; i++) {
        PNODE current = rowsList[i];
        while (current != NULL) {
            PNODE temp = current;
            current = current->Link;
            free(temp);
        }
    }
    free(rowsList);
}

// 計算所需的記憶體單元數
int calculateMemoryUnits(SparseMatrix matrix) {
    int memoryUnits = 0;
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        while (current != NULL) {
            memoryUnits += 2; // 每個節點有一個索引和一個值
            current = current->Link;
        }
    }
    return memoryUnits;
}

// 將輸入解析到矩陣中 (Linked list)
void parseInputToMatrix(SparseMatrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        int index, value;
        printf("Please enter the elements for row %d (end with '0'):\n", i + 1);

        PNODE tail = NULL; // 用於記錄每一列的最後一個節點
        while (1) {
            scanf("%d", &index);
            if (index == 0) break;

            if (index < 1 || index > matrix->cols) {
                printf("Invalid index. Try again.\n");
                continue;
            }

            scanf("%d", &value);
            PNODE newNode = createNode(index - 1, value); // C語言索引從0開始

            if (matrix->rowsList[i] == NULL) {
                matrix->rowsList[i] = newNode;
            } else {
                tail->Link = newNode;
            }
            tail = newNode; // 更新尾指標
        }
    }
}

// 輸出矩陣
void printMatrix(SparseMatrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        for (int j = 0; j < matrix.cols; j++) {
            if (current != NULL && current->col == j) {
                printf("%d ", current->value);
                current = current->Link;
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

// 顯示非零元素的索引和值
void displayNonZeroElements(SparseMatrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        printf("Row %d: ", i + 1);
        while (current != NULL) {
            printf("(%d, %d) ", current->col + 1, current->value); // 顯示索引從1開始
            current = current->Link;
        }
        printf("\n");
    }
}

// 矩陣相加
SparseMatrix addMatrices(SparseMatrix matrixA, SparseMatrix matrixB) {
    SparseMatrix result = createSparseMatrix(matrixA.rows, matrixA.cols);

    for (int i = 0; i < matrixA.rows; i++) {
        PNODE currentA = matrixA.rowsList[i];
        PNODE currentB = matrixB.rowsList[i];
        PNODE tail = NULL;

        while (currentA != NULL || currentB != NULL) {
            int col, value;

            if (currentA == NULL) {
                col = currentB->col;
                value = currentB->value;
                currentB = currentB->Link;
            } else if (currentB == NULL) {
                col = currentA->col;
                value = currentA->value;
                currentA = currentA->Link;
            } else if (currentA->col == currentB->col) {
                col = currentA->col;
                value = currentA->value + currentB->value;
                currentA = currentA->Link;
                currentB = currentB->Link;
            } else if (currentA->col < currentB->col) {
                col = currentA->col;
                value = currentA->value;
                currentA = currentA->Link;
            } else {
                col = currentB->col;
                value = currentB->value;
                currentB = currentB->Link;
            }

            PNODE newNode = createNode(col, value);
            if (result.rowsList[i] == NULL) {
                result.rowsList[i] = newNode;
            } else {
                tail->Link = newNode;
            }
            tail = newNode;
        }
    }

    return result;
}

// 矩陣相乘
SparseMatrix multiplyMatrices(SparseMatrix matrixA, SparseMatrix matrixB) {
    SparseMatrix result = createSparseMatrix(matrixA.rows, matrixB.cols);

    for (int i = 0; i < matrixA.rows; i++) {
        for (int j = 0; j < matrixB.cols; j++) {
            int sum = 0;

            PNODE currentA = matrixA.rowsList[i];
            while (currentA != NULL) {
                int colA = currentA->col;

                // 找到矩陣B中第colA列的j位置元素
                PNODE currentB = matrixB.rowsList[colA];
                while (currentB != NULL && currentB->col != j) {
                    currentB = currentB->Link;
                }

                if (currentB != NULL) {
                    sum += currentA->value * currentB->value;
                }

                currentA = currentA->Link;
            }

            if (sum != 0) {
                PNODE newNode = createNode(j, sum);

                if (result.rowsList[i] == NULL) {
                    result.rowsList[i] = newNode;
                } else {
                    PNODE tail = result.rowsList[i];
                    while (tail->Link != NULL) {
                        tail = tail->Link;
                    }
                    tail->Link = newNode;
                }
            }
        }
    }

    return result;
}

// 矩陣轉置
SparseMatrix transposeMatrix(SparseMatrix matrix) {
    SparseMatrix transpose = createSparseMatrix(matrix.cols, matrix.rows);

    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        while (current != NULL) {
            PNODE newNode = createNode(i, current->value);
            int col = current->col;

            // 插入到轉置矩陣的對應行
            if (transpose.rowsList[col] == NULL) {
                transpose.rowsList[col] = newNode;
            } else {
                PNODE tail = transpose.rowsList[col];
                while (tail->Link != NULL) {
                    tail = tail->Link;
                }
                tail->Link = newNode;
            }
            current = current->Link;
        }
    }

    return transpose;
}

int main() {
    int rowsA, colsA, rowsB, colsB;
    SparseMatrix matrixA, matrixB, resultAdd, resultMultiply, transposeA, transposeB, transposeAdd, transposeMultiply;

    // 讀取矩陣A的大小
    printf("Please enter the size of matrix A (rows and columns):\n");
    if (scanf("%d %d", &rowsA, &colsA) != 2 || rowsA <= 0 || colsA <= 0) {
        fprintf(stderr, "Invalid matrix size.\n");
        return 1;
    }

    // 初始化矩陣A
    matrixA = createSparseMatrix(rowsA, colsA);
    parseInputToMatrix(&matrixA);

    // 讀取矩陣B的大小
    printf("Please enter the size of matrix B (rows and columns):\n");
    if (scanf("%d %d", &rowsB, &colsB) != 2 || rowsB <= 0 || colsB <= 0) {
        fprintf(stderr, "Invalid matrix size.\n");
        freeMatrix(matrixA.rowsList, rowsA);
        return 1;
    }

    // 初始化矩陣B
    matrixB = createSparseMatrix(rowsB, colsB);
    parseInputToMatrix(&matrixB);

    // 顯示矩陣A
    printf("Matrix A:\n");
    printMatrix(matrixA);
    displayNonZeroElements(matrixA);
    printf("Total memory units required: %d\n", calculateMemoryUnits(matrixA));

    // 顯示矩陣A的轉置
    transposeA = transposeMatrix(matrixA);
    printf("Transpose of A:\n");
    printMatrix(transposeA);
    displayNonZeroElements(transposeA);
    printf("Total memory units required for Transpose of A: %d\n", calculateMemoryUnits(transposeA));

    // 顯示矩陣B
    printf("Matrix B:\n");
    printMatrix(matrixB);
    displayNonZeroElements(matrixB);
    printf("Total memory units required: %d\n", calculateMemoryUnits(matrixB));

    // 顯示矩陣B的轉置
    transposeB = transposeMatrix(matrixB);
    printf("Transpose of B:\n");
    printMatrix(transposeB);
    displayNonZeroElements(transposeB);
    printf("Total memory units required for Transpose of B: %d\n", calculateMemoryUnits(transposeB));

    // 矩陣相加
    if (matrixA.rows == matrixB.rows && matrixA.cols == matrixB.cols) {
        resultAdd = addMatrices(matrixA, matrixB);
        printf("Result of A + B:\n");
        printMatrix(resultAdd);
        displayNonZeroElements(resultAdd);
        printf("Total memory units required: %d\n", calculateMemoryUnits(resultAdd));

        // 顯示A + B的轉置
        transposeAdd = transposeMatrix(resultAdd);
        printf("Transpose of A + B:\n");
        printMatrix(transposeAdd);
        displayNonZeroElements(transposeAdd);
        printf("Total memory units required for Transpose of A + B: %d\n", calculateMemoryUnits(transposeAdd));

        freeMatrix(resultAdd.rowsList, resultAdd.rows);
        freeMatrix(transposeAdd.rowsList, transposeAdd.rows);
    } else {
        printf("Matrices cannot be added due to dimension mismatch.\n");
    }

    // 矩陣相乘
    if (matrixA.cols == matrixB.rows) {
        resultMultiply = multiplyMatrices(matrixA, matrixB);
        printf("Result of A * B:\n");
        printMatrix(resultMultiply);
        displayNonZeroElements(resultMultiply);
        printf("Total memory units required: %d\n", calculateMemoryUnits(resultMultiply));

        // 顯示A * B的轉置
        transposeMultiply = transposeMatrix(resultMultiply);
        printf("Transpose of A * B:\n");
        printMatrix(transposeMultiply);
        displayNonZeroElements(transposeMultiply);
        printf("Total memory units required for Transpose of A * B: %d\n", calculateMemoryUnits(transposeMultiply));

        freeMatrix(resultMultiply.rowsList, resultMultiply.rows);
        freeMatrix(transposeMultiply.rowsList, transposeMultiply.rows);
    } else {
        printf("Matrices cannot be multiplied due to dimension mismatch.\n");
    }

    // 釋放記憶體
    freeMatrix(matrixA.rowsList, rowsA);
    freeMatrix(matrixB.rowsList, rowsB);
    freeMatrix(transposeA.rowsList, transposeA.rows);
    freeMatrix(transposeB.rowsList, transposeB.rows);

    return 0;
}

