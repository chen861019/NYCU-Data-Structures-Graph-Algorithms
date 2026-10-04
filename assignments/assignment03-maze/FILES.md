# Assignment III：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/bfs.c](archive/bfs.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/display.c](archive/display.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/g.c](archive/g.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/input.c](archive/input.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/p3_1.c](archive/p3_1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/p3_2.c](archive/p3_2.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/P3_3.c](archive/P3_3.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/test.c](archive/test.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/test_1.c](archive/test_1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [src/Programming_Assignment_III.c](src/Programming_Assignment_III.c) | 整合矩陣輸入、BFS 與路徑回溯 | 成功（有診斷／警告） |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/bfs.c

`bfs`、`printPath`、`main`

### archive/display.c

`initializeMaze`、`fillMaze`、`printMaze`、`main`

### archive/g.c

`generate_maze`、`main`

### archive/input.c

`initializeMaze`、`fillMaze`、`printMaze`、`main`

### archive/p3_1.c

`enqueue`、`dequeue`、`is_empty`、`parse_maze`、`bfs`、`display_maze`、`main`

### archive/p3_2.c

`enqueue`、`dequeue`、`is_empty`、`parse_maze`、`bfs`、`display_maze`、`main`

### archive/P3_3.c

`createMatrix`、`printMatrix`、`parseInputToMatrix`、`bfs`、`printPath`、`printMazeWithPath`、`freeCharMatrix`、`main`

### archive/test.c

`initializeMaze`、`fillMaze`、`printMaze`、`bfs`、`printPath`、`printMazeWithPath`、`main`

### archive/test_1.c

`createCharMatrix`、`printCharMatrix`、`parseInputToMatrix`、`bfs`、`printPath`、`printMazeWithPath`、`freeCharMatrix`、`main`

### src/Programming_Assignment_III.c

`createCharMatrix`、`printCharMatrix`、`parseInputToMatrix`、`bfs`、`printPath`、`printMazeWithPath`、`freeCharMatrix`、`diff`、`main`
