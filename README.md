# NYCU Data Structures & Graph Algorithms

以 C 語言實作資料結構與圖論演算法的課程作品集，包含九份 Programming Assignment、原始程式演進版本、報告結果圖片與重新執行的驗證紀錄。

本專案獨立於 FPGA 課程 repository。整理重點不是單純備份檔案，而是讓讀者理解每份作業的**問題、設計目的、資料結構、執行流程、結果、複雜度及適用情境**。

## 作業導覽

| 作業 | 主題 | 核心概念 | 詳細說明 |
| --- | --- | --- | --- |
| I | 日期運算 | Gregorian 閏年、Zeller 星期公式、日期位移 | [日期計算](assignments/assignment01-date/README.md) |
| II | 五種排序 | Insertion、Selection、Quick、Merge、Heap Sort | [排序與效能比較](assignments/assignment02-sorting/README.md) |
| III | 迷宮最短路徑 | BFS、佇列、走訪狀態、父節點回溯 | [迷宮搜尋](assignments/assignment03-maze/README.md) |
| IV | 稀疏矩陣 | 逐列鏈結串列、加法、乘法、轉置 | [稀疏矩陣運算](assignments/assignment04-sparse-matrix/README.md) |
| V | 運算式計算器 | 數值堆疊、運算子堆疊、優先序、變數 | [中序運算式求值](assignments/assignment05-expression/README.md) |
| VI | 區間最大值 | Segment Tree、遞迴建樹、區間查詢 | [區間查詢與比較](assignments/assignment06-segment-tree/README.md) |
| VII | Huffman 壓縮 | 字元頻率、Min-Heap、前綴碼、位元封裝 | [壓縮與解壓縮](assignments/assignment07-huffman/README.md) |
| VIII | 最小生成樹 | Kruskal、邊排序、Union-Find、DOT 圖形 | [最小生成樹](assignments/assignment08-mst/README.md) |
| IX | 最短路徑 | Dijkstra、Adjacency List、距離鬆弛、路徑樹 | [加權圖最短路徑](assignments/assignment09-shortest-path/README.md) |

## 內容與閱讀方式

每份作業都有 `README.md`、完整圖片索引 `RESULTS.md` 與逐檔程式索引 `FILES.md`。

- `src/`：依原始碼與報告內容對應後選出的主要程式；保留原本的檔名與演算法。
- `archive/`：早期版本、拆分測試與實驗程式；不等同於穩定產品，也不是全部符合題目要求。
- `assets/`：從本人課程 Word 報告抽出的 162 張 PNG/SVG 圖，重新命名後附閱讀說明。
- `examples/`：可公開的文字資料、圖形範例或新增的小型重現輸入；歷史資料與新測例分開命名。
- `extras/`：區間總和 Segment Tree 範例及具原作者註記的 GCD 參考程式。
- `tools/`：匯入、圖片索引與編譯／代表性行為檢查工具。

建議先閱讀各作業的設計與限制，再看圖片。圖中的耗時是**當時報告的歷史觀察**，不是本次重新量測，也不能直接與其他電腦的數字比較。

## 重新驗證

2026-10-05 使用 Windows GCC 7.4.0，以 GNU C11、`-O0 -Wall -Wextra` 編譯並執行小型測例：

| 範圍 | 結果 |
| --- | --- |
| 主要程式 | 14 / 14 編譯成功 |
| 全部保留的 C 程式，含舊草稿與額外範例 | 67 / 68 編譯成功 |
| 代表性行為檢查 | 18 / 18 通過 |
| 報告圖片 | 162 張，另外保留部分原始 DOT／PNG 圖形範例 |

唯一編譯失敗的是作業 IV 的 `archive/input.c`，其原始草稿含斷裂的函式名稱。主版本不受影響。**測例通過不代表所有規格均完成**：例如計算器的未定義變數會當作 0，Huffman 檔案未儲存解碼樹，Dijkstra 使用線性搜尋而非 Heap。這些現況均在各作業明列。

詳見 [驗證範圍與限制](docs/VERIFICATION.md)、[機器可讀的輸出](docs/verification.json) 及 [歷史效能分析](docs/BENCHMARKS.md)。

## 編譯與執行

需要 GCC、Python 3（工具使用 Python 3.10 以上語法），Graphviz 則是選用的 DOT 轉圖工具。排序主程式含 GNU 巢狀函式，因此此版本應使用 GCC 的 GNU C 模式，而非假設所有 ISO C／Clang 工具鏈都能直接編譯。

以下以 PowerShell、在 repository 根目錄執行為例：

```powershell
New-Item -ItemType Directory -Force build | Out-Null
gcc -std=gnu11 -O0 -Wall -Wextra assignments/assignment01-date/src/Programming_Assignment_I.c -o build/date.exe -lm
'2024/2/28 + 1' | .\build\date.exe
python -X utf8 tools/verify_course.py --cc gcc
```

其他作業的編譯範例見各自說明。圖論程式與 Huffman 會寫檔，請在獨立的 `build/` 子資料夾執行，避免覆寫保留的歷史範例。驗證工具使用暫存工作目錄，不執行來源資料夾內的舊 EXE。

## 資料來源與公開範圍

文件依原始 C 程式、課程題目及本人作業報告交叉整理。保留程式演進痕跡，只統一文字編碼與換行，並移除原始標頭中的學號與電子郵件；未替舊版程式補造功能或重新解讀成完整產品。

原始 Word／PDF 報告、課程講義、作業題目全文、EXE、試算表、備份與原「垃圾桶」內的程式未上傳。圖片只作為既有結果證據；其中網站截圖、工具輸出與第三方程式各依實際性質說明，不宣稱為本人自行設計的介面。

[整理與來源對照](docs/ASSIGNMENTS.md) · [檔案來源及雜湊](docs/source-manifest.json) · [額外範例](extras/README.md)
