# Assignment II：五種排序與效能比較

[課程首頁](../../README.md) · [全部報告圖片](RESULTS.md) · [逐檔索引](FILES.md)

## 目的與用途

在同一 C 語言環境實作五種排序，觀察 O(n²) 與 O(n log n) 隨資料量增加的差異，並練習把資料生成、正確性檢查與計時區間分開。排序是搜尋前處理、資料整理、統計與圖論邊排序的基礎；本作業也銜接後續 Kruskal 的排序需求。

## 主要程式

| 程式 | 角色 |
| --- | --- |
| [p2_1.c](src/p2_1.c) | 基礎排序實作與比較版本 |
| [P2_2.c](src/P2_2.c) | 建議閱讀入口，含 DEBUG 顯示、五種排序與排序結果檢查 |
| [p2_3s.c](src/p2_3s.c) | 產生並儲存隨機資料 |
| [p2_3r.c](src/p2_3r.c) | 讀取 `random_numbers.txt` 再比較排序 |

[`examples/random_numbers.txt`](examples/random_numbers.txt) 為原資料範例。`archive/` 保留其他測試，不取代上述版本。

## 演算法設計

| 演算法 | 核心操作 | 時間複雜度 | 額外空間／穩定性 |
| --- | --- | --- | --- |
| Insertion Sort | 將目前元素插入已排序前綴 | 最佳 O(n)，平均／最差 O(n²) | O(1)，穩定 |
| Selection Sort | 選出剩餘區間最小值並交換 | O(n²) | O(1)，一般不穩定 |
| Quick Sort | 隨機 pivot、Lomuto partition、分治 | 平均 O(n log n)，最差 O(n²) | 遞迴堆疊平均 O(log n)、最差 O(n)，不穩定 |
| Merge Sort | 左右子陣列排序後合併 | O(n log n) | O(n) 暫存與遞迴堆疊，使用 `<=` 因而穩定 |
| Heap Sort | 建立 Max-Heap、逐次取最大值 | O(n log n) | 陣列操作額外 O(1)，本版遞迴 heapify 另有 O(log n) 堆疊；不穩定 |

Merge Sort 的暫存陣列採 VLA，位於 stack；資料量放大時，stack 限額是實際風險。`P2_2.c` 的 `heapify` 為函式內定義的 GNU 巢狀函式，應使用 GCC 的 GNU C 模式。

## 實驗流程與公平性

`P2_2.c` 的 DEBUG 模式使用 20 筆資料，通常模式使用 10,000 筆。各排序前重新設定相同種子 7，使它們從相同初始資料出發，而不是把前一種排序的已排序輸出給下一種排序。產生資料與結果檢查位於排序計時區間之外。

1. 初始化種子並產生陣列。
2. 執行指定排序並量測耗時。
3. DEBUG 顯示排序前後數值。
4. 檢查相鄰元素是否非遞減。
5. 對其餘演算法重複同樣流程。

相鄰次序檢查能發現未排序成功，但沒有獨立驗證所有輸入值的 multiplicity；若未來做嚴謹測試，可再與標準排序結果逐元素比對。

## 編譯與操作

```powershell
gcc -std=gnu11 -O0 -DDEBUG assignments/assignment02-sorting/src/P2_2.c -o build/sorting-debug.exe -lm
.\build\sorting-debug.exe
gcc -std=gnu11 -O0 assignments/assignment02-sorting/src/P2_2.c -o build/sorting.exe -lm
.\build\sorting.exe
```

檔案版本請在獨立工作資料夾執行產生器，再執行讀取器；它們依相對工作目錄找 `random_numbers.txt`，不是依原始碼位置找檔案。

## 歷史報告結果

單位為秒，取自原報告，**未在本次重跑大型效能測試**。

| n | Insertion | Selection | Quick | Merge | Heap |
| --- | --- | --- | --- | --- | --- |
| 10,000 | 0.03434 | 0.04543 | 0.00083 | 0.00095 | 0.00089 |
| 50,000 | 0.78823 | 1.11201 | 0.00350 | 0.00450 | 0.00554 |
| 100,000 | 3.23883 | 4.34088 | 0.00712 | 0.00916 | 0.01170 |

![十萬筆排序歷史結果](assets/report-04.png)

資料量由 10,000 增至 100,000，Insertion／Selection 約增加兩個數量級，符合平方級成長的趨勢；三種分治／Heap 方法明顯較快。單次微小耗時會受計時精度、編譯最佳化、CPU、快取及隨機分布影響，不能據此宣稱 Quick 在所有情境必然最快。

報告提到更大資料下 Merge 失敗。以目前 VLA 實作，stack 空間限制是合理風險，但未保留足夠診斷來證明原因，不能直接歸因「全部 RAM 不足」，也不是 Merge Sort 理論上無法處理大量資料。

## 本次驗證與改善方向

DEBUG 的五種排序皆輸出 `Array sorted correctly!`。這次僅重現小型正確性測試，不將其中 sub-microsecond 值當作新的排行榜。

更完整的比較應加入多次重複、平均／分位數、相同輸入副本、已排序／逆序／大量重複值、最佳化旗標與硬體記錄。大型 Merge 暫存可改為 heap 配置；GNU 巢狀函式可在需要可攜性時另外改寫，原始課程版目前保留不動。
