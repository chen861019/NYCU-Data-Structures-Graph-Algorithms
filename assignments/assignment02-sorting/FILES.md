# Assignment II：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/test.c](archive/test.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [src/p2_1.c](src/p2_1.c) | 基礎排序比較 | 成功 |
| [src/P2_2.c](src/P2_2.c) | 主要 DEBUG／計時與排序檢查版本 | 成功（有診斷／警告） |
| [src/p2_3r.c](src/p2_3r.c) | 資料檔讀取與排序比較 | 成功（有診斷／警告） |
| [src/p2_3s.c](src/p2_3s.c) | 資料檔產生器 | 成功 |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/test.c

`insertionSort`、`selectionsort`、`quickSort`、`main`

### src/p2_1.c

`insertionSort`、`selectionSort`、`partition`、`quickSort`、`merge`、`mergeSort`、`heapify`、`heapSort`、`generateRandomArray`、`printArray`、`main`

### src/P2_2.c

`insertionSort`、`selectionSort`、`quickSort`、`mergeSort`、`heapSort`、`heapify`、`generateRandomArray`、`printArray`、`isSorted`、`diff`、`main`

### src/p2_3r.c

`insertionSort`、`selectionSort`、`quickSort`、`mergeSort`、`heapSort`、`heapify`、`readArrayFromFile`、`diff`、`main`

### src/p2_3s.c

`generateRandomArrayToFile`、`main`
