# 作業對照與閱讀地圖

[課程首頁](../README.md)

主版本依原始碼、作業主題及報告線索選定，不是只依最後修改時間猜測。舊版本保留原檔名，僅將九個作業外層資料夾統一成 assignment01 至 assignment09。

| 作業 | 主題 | C 程式 | 報告圖 | 詳細入口 |
| --- | --- | --- | --- | --- |
| I | 日期運算 | 1 | 18 | [說明](../assignments/assignment01-date/README.md) |
| II | 五種排序 | 5 | 4 | [說明](../assignments/assignment02-sorting/README.md) |
| III | BFS 迷宮 | 10 | 5 | [說明](../assignments/assignment03-maze/README.md) |
| IV | 稀疏矩陣 | 8 | 33 | [說明](../assignments/assignment04-sparse-matrix/README.md) |
| V | 運算式計算器 | 8 | 24 | [說明](../assignments/assignment05-expression/README.md) |
| VI | 區間最大值 | 8 | 8 | [說明](../assignments/assignment06-segment-tree/README.md) |
| VII | Huffman | 15 | 10 | [說明](../assignments/assignment07-huffman/README.md) |
| VIII | Kruskal MST | 6 | 29 | [說明](../assignments/assignment08-mst/README.md) |
| IX | Dijkstra | 5 | 31 | [說明](../assignments/assignment09-shortest-path/README.md) |

## 學習銜接

日期操作練習資料模型與邊界；排序引入複雜度與計時；BFS 使用 queue；稀疏矩陣練習 pointer 與串列；計算器用 stack 管理優先序；Segment Tree 預處理重複查詢；Huffman 與 Kruskal 展示不同貪婪策略；Dijkstra 則將距離與路徑記錄結合。

## 來源與公開處理

原始課程資料只讀，不覆寫。匯入 C／文字／DOT 的原相對路徑、編碼與前後 SHA-256 記錄在 [source-manifest.json](source-manifest.json)。新增加的小型輸入範例與 Markdown 不冒充歷史來源。

Word 報告用於抽圖與交叉確認；PDF 題目／報告／講義不直接公開，亦未上傳 EXE、XLSX、備份及原垃圾桶中的 7 份 C 草稿。對第三方的 GCD 參考程式保留原作者 D. J. Guan 註記。

只移除原始碼標頭中的學號／電子郵件，轉 UTF-8 與 LF；沒有為了展示而補上缺失演算法。圖形時間屬歷史結果；新編譯與代表性測例見 [VERIFICATION.md](VERIFICATION.md)。
