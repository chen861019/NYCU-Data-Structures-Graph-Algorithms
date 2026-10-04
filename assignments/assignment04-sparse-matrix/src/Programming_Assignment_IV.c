//Programming Assignment IV 
//Sparse Matrix
//author : 陳沛安
//date: 2024/10/21
#include <stdio.h>
#include <stdlib.h>

// 節點結構，用於儲存非零元素
typedef struct Node {
    int col; // 儲存元素的列索引
    int value; // 儲存元素的值
    struct Node *Link; // 指向下一個節點的指標
} Node, *PNODE;

// SparseMatrix 結構，用來儲存稀疏矩陣
typedef struct {
    int rows; // 行數
    int cols; // 列數
    PNODE *rowsList; // 每行對應的鏈結串列
} SparseMatrix;

// 創建新節點，儲存非零元素
PNODE createNode(int col, int value) {
    PNODE newNode = (PNODE)malloc(sizeof(Node)); // 分配記憶體給新節點
    newNode->col = col; // 設置列索引
    newNode->value = value; // 設置值
    newNode->Link = NULL; // 初始化指標為NULL
    return newNode; // 返回新節點
}

// 創建稀疏矩陣，初始化為空的鏈結串列
SparseMatrix createSparseMatrix(int rows, int cols) {
    SparseMatrix matrix;
    matrix.rows = rows; // 設定行數
    matrix.cols = cols; // 設定列數
    matrix.rowsList = (PNODE*)malloc(rows * sizeof(PNODE)); // 分配記憶體給行的鏈結串列
    for (int i = 0; i < rows; i++) {
        matrix.rowsList[i] = NULL; // 初始化每行為NULL
    }
    return matrix; // 返回稀疏矩陣
}

// 釋放矩陣記憶體
void freeMatrix(PNODE *rowsList, int rows) {
    for (int i = 0; i < rows; i++) {
        PNODE current = rowsList[i];
        while (current != NULL) {
            PNODE temp = current;
            current = current->Link; // 逐個節點釋放記憶體
            free(temp);
        }
    }
    free(rowsList); // 釋放行的鏈結串列記憶體
}

// 計算稀疏矩陣所需的記憶體單元
int calculateMemoryUnits(SparseMatrix matrix) {
    int memoryUnits = 0;
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        while (current != NULL) {
            memoryUnits += 4; // 每個節點佔用兩個記憶體單元：索引和值
            current = current->Link;
        }
    }
    return memoryUnits; // 返回所需記憶體單元數量
}

// 將輸入解析並轉換成稀疏矩陣
void parseInputToMatrix(SparseMatrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        int index, value;
        printf("Please enter the elements for row %d (end with '0'):\n", i + 1);

        PNODE tail = NULL; // 用來跟蹤行的最後一個節點
        while (1) {
            scanf("%d", &index); // 輸入列索引
            if (index == 0) break; // 當輸入0時，結束該行的輸入

            if (index < 1 || index > matrix->cols) { // 檢查列索引是否有效
                printf("Invalid index. Try again.\n");
                continue;
            }

            scanf("%d", &value); // 輸入對應列索引的值
            PNODE newNode = createNode(index - 1, value); // 創建新節點 (列索引從0開始)

            if (matrix->rowsList[i] == NULL) { // 若行的鏈結串列為空，設為新節點
                matrix->rowsList[i] = newNode;
            } else {
                tail->Link = newNode; // 將新節點加到鏈結串列尾部
            }
            tail = newNode; // 更新尾指標
        }
    }
}

// 以常規格式輸出稀疏矩陣
void printMatrix(SparseMatrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i]; // 指向行的頭節點
        for (int j = 0; j < matrix.cols; j++) {
            if (current != NULL && current->col == j) { // 若找到對應的非零元素
                printf("%d ", current->value); // 輸出元素值
                current = current->Link; // 移動到下一個節點
            } else {
                printf("0 "); // 若無匹配則輸出0
            }
        }
        printf("\n"); // 換行
    }
}

// 以列表格式顯示非零元素
void displayNonZeroElements(SparseMatrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i];
        printf("Row %d: ", i + 1);
        while (current != NULL) {
            printf("(%d, %d) ", current->col + 1, current->value); // 顯示索引和值
            current = current->Link;
        }
        printf("\n");
    }
}

// 矩陣相加
SparseMatrix addMatrices(SparseMatrix matrixA, SparseMatrix matrixB) {
    SparseMatrix result = createSparseMatrix(matrixA.rows, matrixA.cols); // 創建結果矩陣

    for (int i = 0; i < matrixA.rows; i++) {
        PNODE currentA = matrixA.rowsList[i];
        PNODE currentB = matrixB.rowsList[i];
        PNODE tail = NULL;

        while (currentA != NULL || currentB != NULL) {
            int col, value;

            if (currentA == NULL) { // 若A的元素為空，使用B的元素
                col = currentB->col;
                value = currentB->value;
                currentB = currentB->Link;
            } else if (currentB == NULL) { // 若B的元素為空，使用A的元素
                col = currentA->col;
                value = currentA->value;
                currentA = currentA->Link;
            } else if (currentA->col == currentB->col) { // 若兩者的列相同，進行相加
                col = currentA->col;
                value = currentA->value + currentB->value;
                currentA = currentA->Link;
                currentB = currentB->Link;
            } else if (currentA->col < currentB->col) { // 若A的列較小，使用A的元素
                col = currentA->col;
                value = currentA->value;
                currentA = currentA->Link;
            } else { // 若B的列較小，使用B的元素
                col = currentB->col;
                value = currentB->value;
                currentB = currentB->Link;
            }

            PNODE newNode = createNode(col, value); // 創建新節點，存入結果
            if (result.rowsList[i] == NULL) {
                result.rowsList[i] = newNode;
            } else {
                tail->Link = newNode;
            }
            tail = newNode; // 更新尾指標
        }
    }

    return result; // 返回相加結果矩陣
}

// 矩陣相乘
SparseMatrix multiplyMatrices(SparseMatrix matrixA, SparseMatrix matrixB) {
    SparseMatrix result = createSparseMatrix(matrixA.rows, matrixB.cols); // 創建結果矩陣

    for (int i = 0; i < matrixA.rows; i++) {
        for (int j = 0; j < matrixB.cols; j++) {
            int sum = 0; // 用來儲存相乘結果

            PNODE currentA = matrixA.rowsList[i]; // 取出A的行
            while (currentA != NULL) {
                int colA = currentA->col; // A的列索引
                
                // 找到矩陣B中對應的列
                PNODE currentB = matrixB.rowsList[colA];
                while (currentB != NULL && currentB->col != j) {
                    currentB = currentB->Link; // 尋找對應的元素
                }

                if (currentB != NULL) {
                    sum += currentA->value * currentB->value; // 計算元素乘積並累加
                }

                currentA = currentA->Link; // 移動到下一個A的元素
            }

            if (sum != 0) { // 若結果非零，儲存到結果矩陣中
                PNODE newNode = createNode(j, sum);

                if (result.rowsList[i] == NULL) {
                    result.rowsList[i] = newNode;
                } else {
                    PNODE tail = result.rowsList[i];
                    while (tail->Link != NULL) {
                        tail = tail->Link; // 移動到行尾
                    }
                    tail->Link = newNode; // 插入新節點
                }
            }
        }
    }

    return result; // 返回相乘結果矩陣
}

// 矩陣轉置
SparseMatrix transposeMatrix(SparseMatrix matrix) {
    SparseMatrix transpose = createSparseMatrix(matrix.cols, matrix.rows); // 創建轉置矩陣

    for (int i = 0; i < matrix.rows; i++) {
        PNODE current = matrix.rowsList[i]; // 取出行
        while (current != NULL) {
            PNODE newNode = createNode(i, current->value); // 創建轉置後的新節點
            int col = current->col; // 取得列索引

            // 插入到轉置矩陣對應的行中
            if (transpose.rowsList[col] == NULL) {
                transpose.rowsList[col] = newNode; // 若轉置行為空，設為新節點
            } else {
                PNODE tail = transpose.rowsList[col];
                while (tail->Link != NULL) {
                    tail = tail->Link; // 移動到行尾
                }
                tail->Link = newNode; // 插入到行尾
            }
            current = current->Link; // 移動到下一個元素
        }
    }

    return transpose; // 返回轉置矩陣
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
    parseInputToMatrix(&matrixA); // 解析輸入轉換為稀疏矩陣

    // 讀取矩陣B的大小
    printf("Please enter the size of matrix B (rows and columns):\n");
    if (scanf("%d %d", &rowsB, &colsB) != 2 || rowsB <= 0 || colsB <= 0) {
        fprintf(stderr, "Invalid matrix size.\n");
        freeMatrix(matrixA.rowsList, rowsA);
        return 1;
    }

    // 初始化矩陣B
    matrixB = createSparseMatrix(rowsB, colsB);
    parseInputToMatrix(&matrixB); // 解析輸入轉換為稀疏矩陣

    // 顯示矩陣A
    printf("Matrix A:\n");
    printMatrix(matrixA); // 輸出矩陣A
    displayNonZeroElements(matrixA); // 輸出A的非零元素
    printf("Total memory units required: %d\n", calculateMemoryUnits(matrixA)); // 顯示記憶體單元數

    // 顯示矩陣A的轉置
    transposeA = transposeMatrix(matrixA);
    printf("Transpose of A:\n");
    printMatrix(transposeA); // 輸出轉置後的矩陣A
    displayNonZeroElements(transposeA); // 輸出轉置後A的非零元素
    printf("Total memory units required for Transpose of A: %d\n", calculateMemoryUnits(transposeA));

    // 顯示矩陣B
    printf("Matrix B:\n");
    printMatrix(matrixB); // 輸出矩陣B
    displayNonZeroElements(matrixB); // 輸出B的非零元素
    printf("Total memory units required: %d\n", calculateMemoryUnits(matrixB));

    // 顯示矩陣B的轉置
    transposeB = transposeMatrix(matrixB);
    printf("Transpose of B:\n");
    printMatrix(transposeB); // 輸出轉置後的矩陣B
    displayNonZeroElements(transposeB); // 輸出轉置後B的非零元素
    printf("Total memory units required for Transpose of B: %d\n", calculateMemoryUnits(transposeB));

    // 矩陣相加
    if (matrixA.rows == matrixB.rows && matrixA.cols == matrixB.cols) { // 檢查是否可相加
        resultAdd = addMatrices(matrixA, matrixB); // 進行矩陣相加
        printf("Result of A + B:\n");
        printMatrix(resultAdd); // 輸出相加結果
        displayNonZeroElements(resultAdd); // 輸出相加後的非零元素
        printf("Total memory units required: %d\n", calculateMemoryUnits(resultAdd)); // 顯示記憶體單元數

        // 顯示A + B的轉置
        transposeAdd = transposeMatrix(resultAdd);
        printf("Transpose of A + B:\n");
        printMatrix(transposeAdd); // 輸出轉置後的A+B
        displayNonZeroElements(transposeAdd); // 輸出轉置後A+B的非零元素
        printf("Total memory units required for Transpose of A + B: %d\n", calculateMemoryUnits(transposeAdd));

        freeMatrix(resultAdd.rowsList, resultAdd.rows); // 釋放相加結果記憶體
        freeMatrix(transposeAdd.rowsList, transposeAdd.rows); // 釋放轉置結果記憶體
    } else {
        printf("Matrices cannot be added due to dimension mismatch.\n"); // 顯示無法相加訊息
    }

    // 矩陣相乘
    if (matrixA.cols == matrixB.rows) { // 檢查是否可相乘
        resultMultiply = multiplyMatrices(matrixA, matrixB); // 進行矩陣相乘
        printf("Result of A * B:\n");
        printMatrix(resultMultiply); // 輸出相乘結果
        displayNonZeroElements(resultMultiply); // 輸出相乘後的非零元素
        printf("Total memory units required: %d\n", calculateMemoryUnits(resultMultiply)); // 顯示記憶體單元數

        // 顯示A * B的轉置
        transposeMultiply = transposeMatrix(resultMultiply);
        printf("Transpose of A * B:\n");
        printMatrix(transposeMultiply); // 輸出轉置後的A*B
        displayNonZeroElements(transposeMultiply); // 輸出轉置後A*B的非零元素
        printf("Total memory units required for Transpose of A * B: %d\n", calculateMemoryUnits(transposeMultiply));

        freeMatrix(resultMultiply.rowsList, resultMultiply.rows); // 釋放相乘結果記憶體
        freeMatrix(transposeMultiply.rowsList, transposeMultiply.rows); // 釋放轉置結果記憶體
    } else {
        printf("Matrices cannot be multiplied due to dimension mismatch.\n"); // 顯示無法相乘訊息
    }

    // 釋放所有記憶體
    freeMatrix(matrixA.rowsList, rowsA);
    freeMatrix(matrixB.rowsList, rowsB);
    freeMatrix(transposeA.rowsList, transposeA.rows);
    freeMatrix(transposeB.rowsList, transposeB.rows);

    return 0; // 程式結束
}

