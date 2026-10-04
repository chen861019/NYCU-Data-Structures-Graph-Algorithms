# Assignment VII：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/7_1.c](archive/7_1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_10.c](archive/7_10.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_11.c](archive/7_11.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_12.c](archive/7_12.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_13.c](archive/7_13.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_2.c](archive/7_2.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_3.c](archive/7_3.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_4.c](archive/7_4.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_5.c](archive/7_5.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_6.c](archive/7_6.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_7.c](archive/7_7.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_8.c](archive/7_8.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/7_9.c](archive/7_9.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [archive/test.c](archive/test.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [src/7_14.c](src/7_14.c) | 頻率、Min-Heap、編碼、bit packing 與同程序解碼 | 成功（有診斷／警告） |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/7_1.c

`newNode`、`createMinHeap`、`swapMinHeapNode`、`minHeapify`、`isLeaf`、`buildMinHeap`、`printCodes`、`HuffmanCodes`、`main`

### archive/7_10.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`calculateCompressionRatio`、`generateFile`、`main`

### archive/7_11.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`compressFile`、`decompressFile`、`freeHuffmanTree`、`generateFile`、`main`

### archive/7_12.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`calculateFrequency`、`displayFrequencies`、`printCodes`、`compressFile`、`decompressFile`、`getFileSize`、`calculateCompressionRatio`、`freeHuffmanTree`、`generateFile`、`verifyFiles`、`main`

### archive/7_13.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`calculateFrequency`、`displayFrequencies`、`printCodes`、`compressFile`、`decompressFile`、`getFileSize`、`calculateCompressionRatio`、`freeHuffmanTree`、`generateFile`、`verifyFiles`、`main`

### archive/7_2.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`printCodes`、`main`

### archive/7_3.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`calculateCompressionRatio`、`compressFile`、`decompressFile`、`verifyFiles`、`main`

### archive/7_4.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`main`

### archive/7_5.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`main`

### archive/7_6.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`main`

### archive/7_7.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`main`

### archive/7_8.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`calculateCompressionRatio`、`main`

### archive/7_9.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`printCodes`、`compressFile`、`decompressFile`、`verifyFiles`、`calculateCompressionRatio`、`generateFile`、`main`

### archive/test.c

`verifyFiles`、`main`

### src/7_14.c

`createNode`、`createMinHeap`、`swapNodes`、`minHeapify`、`extractMin`、`insertMinHeap`、`buildHuffmanTree`、`generateCodes`、`calculateFrequency`、`displayFrequencies`、`printCodes`、`compressFile`、`decompressFile`、`generateFile`、`verifyFiles`、`getFileSize`、`calculateCompressionRatio`、`diff`、`freeHuffmanTree`、`main`
