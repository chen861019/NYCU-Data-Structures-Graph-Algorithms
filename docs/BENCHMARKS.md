# 歷史報告的效能分析與限制

[課程首頁](../README.md) · [本次正確性驗證](VERIFICATION.md)

此頁解釋原 Word 報告留下的數值，不是新的大型 benchmark。數據表與圖片在各作業 README／RESULTS；硬體、編譯旗標與重複次數未完整保存，因此不將歷史秒數當成可跨機器比較的規格。

## 排序：觀察數量級，不宣稱固定排名

[Assignment II](../assignments/assignment02-sorting/README.md) 的 10,000、50,000、100,000 筆資料呈現 O(n²) 方法與 O(n log n) 方法明顯差距。以同一種子恢復初始資料是合理設計，但單次小耗時仍可能受 timer 解析度、快取、CPU 與編譯最佳化影響。

適合的結論是「在當時資料與環境下，平方級方法隨 n 增大更慢」；不適合的結論是「Quick 在所有資料永遠最快」。VLA stack 限制也不等於 Merge Sort 的理論容量限制。

## Segment Tree：比較條件未完全一致

[Assignment VI](../assignments/assignment06-segment-tree/README.md) 的大型圖顯示 tree 耗時隨 n 緩增，但 DEBUG 的 tree／Brute Force 分別產生隨機區間，不是同一批 query；結果也未防止最佳化移除工作。

因此可作為教學觀察，不用原圖直接宣稱精確幾倍加速。公平測試應共用固定 query list、累積 checksum、記錄建樹時間與查詢時間，並重複多輪。

## Huffman：比值、節省率與計時區間

[Assignment VII](../assignments/assignment07-huffman/README.md) 印出的 168% 是 `original / compressed * 100`，代表檔案約變成原本的 59.5%，不是減少 168%。壓縮大小含尾端原生 int，但不含可獨立解碼的樹，因為本格式尚未儲存它。

歷史壓縮／解壓縮時間沒有完整包住隨機檔生成、頻率統計及建樹。隨機英文字母結果不能外推到所有類型的檔案。比較其他壓縮工具前，也需一致的資料、可解碼資訊與耗時範圍。

## Kruskal：秒、邊數與輸出成本

[Assignment VIII](../assignments/assignment08-mst/README.md) 原表的 ms 與程式 seconds 不一致，文件採程式的秒；V=500 案例的 E 按截圖修正為 2,500。耗時包含排序、MST、console 與 DOT，不是純 Union-Find。

O(E log E) 的程式不能因幾筆時間增加就稱為指數級。隨機產生器不保證無重複邊與連通，資料品質也會影響「MST」是否其實為 forest。

## Dijkstra：全部 source 不等於單次搜尋

[Assignment IX](../assignments/assignment09-shortest-path/README.md) 依次計算所有 V 個 source，而且用 O(V) 線性掃描選下一點。演算法部分為約 O(V³+VE)，並有大量路徑文字與 DOT 輸出。

因此不能用報告的整輪耗時對比一次 Kruskal，或把 Heap 版本的單源複雜度套到目前程式。改善效能前，應先決定真正需求是單一 source、所有 source，還是只找特定兩點。

## 建議的後續量測設計

- 固定輸入與 seed，記錄 n／V／E、權重分布、連通與重複邊狀態。
- 記錄硬體、作業系統、編譯器、旗標與計時方法。
- 分開產生資料、演算法、I/O 與圖形渲染。
- 先驗證結果，再多次執行，呈現中位數或分位數而非只挑最好一筆。
- 大型測試需記錄 stack／heap 限額與失敗診斷，不將未知原因當成已證實分析。

以上是改善方法，不是這個 repository 已完成的大規模實驗。本次完成的是獨立編譯與 18 組小型代表性行為檢查。
