# Assignment IV：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/input.c](archive/input.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 失敗：保留原始草稿 |
| [archive/linked_list.c](archive/linked_list.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/NEW_TEST4.c](archive/NEW_TEST4.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/p4.c](archive/p4.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/size.c](archive/size.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/test.c](archive/test.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/test_1.c](archive/test_1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [src/Programming_Assignment_IV.c](src/Programming_Assignment_IV.c) | 鏈結串列矩陣、加法、乘法與轉置 | 成功 |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/input.c

`createCharMatrix`、`parseInputToMatrix`、`printMatrix`、`freeMatrix`、`main`

### archive/linked_list.c

`main`

### archive/NEW_TEST4.c

`createNode`、`createSparseMatrix`、`freeMatrix`、`calculateMemoryUnits`、`parseInputToMatrix`、`printMatrix`、`displayNonZeroElements`、`addMatrices`、`multiplyMatrices`、`main`

### archive/p4.c

`createCharMatrix`、`parseInputToMatrix`、`printMatrix`、`printListFormat`、`addMatrices`、`multiplyMatrices`、`freeMatrix`、`main`

### archive/size.c

`main`

### archive/test.c

`createNode`、`createSparseMatrix`、`freeMatrix`、`parseInputToMatrix`、`printMatrix`、`addMatrices`、`multiplyMatrices`、`main`

### archive/test_1.c

`createNode`、`createSparseMatrix`、`freeMatrix`、`calculateMemoryUnits`、`parseInputToMatrix`、`printMatrix`、`displayNonZeroElements`、`addMatrices`、`multiplyMatrices`、`transposeMatrix`、`main`

### src/Programming_Assignment_IV.c

`createNode`、`createSparseMatrix`、`freeMatrix`、`calculateMemoryUnits`、`parseInputToMatrix`、`printMatrix`、`displayNonZeroElements`、`addMatrices`、`multiplyMatrices`、`transposeMatrix`、`main`
