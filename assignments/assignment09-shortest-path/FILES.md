# Assignment IX：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/old code/9_1.c](archive/old%20code/9_1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/old code/9_2.c](archive/old%20code/9_2.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/old code/9_3.c](archive/old%20code/9_3.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功（有診斷／警告） |
| [src/9_4.c](src/9_4.c) | Adjacency List、線性選點 Dijkstra 與所有 source 的 DOT | 成功（有診斷／警告） |
| [src/9_txt.c](src/9_txt.c) | 隨機圖資料產生器 | 成功 |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/old code/9_1.c

`createGraph`、`addEdge`、`createHeapNode`、`createMinHeap`、`swapHeapNodes`、`minHeapify`、`decreaseKey`、`extractMin`、`dijkstra`、`readGraphFromFile`、`main`

### archive/old code/9_2.c

`createGraph`、`addEdge`、`dijkstra`、`writeDOT`、`readGraphFromFile`、`main`

### archive/old code/9_3.c

`createGraph`、`addEdge`、`freeGraph`、`dijkstra`、`writeOriginalGraphDOT`、`writeShortestPathTreeDOT`、`readGraphFromFile`、`diff`、`main`

### src/9_4.c

`createGraph`、`addEdge`、`freeGraph`、`dijkstra`、`printPath`、`printShortestPaths`、`writeOriginalGraphDOT`、`writeShortestPathTreeDOT`、`readGraphFromFile`、`diff`、`main`

### src/9_txt.c

`generateGraphFile`、`main`
