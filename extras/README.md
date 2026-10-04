# 額外範例與來源說明

[課程首頁](../README.md)

## 區間總和 Segment Tree

[segment_tree.c](segment_tree.c) 使用迭代式 `2*n` 陣列建樹，支援 inclusive 區間總和與單點更新。原範例陣列為 `[1,3,5,7,9,11]`，查詢 `[1,3]` 得到 15；將 index 2 從 5 更新為 6 後，同一查詢得到 16。

建樹 O(n)，查詢／更新 O(log n)，空間 O(n)。這與 Assignment VI 的遞迴式最大值樹在運算聚合與介面上不同，單獨保存以免讓主作業看起來已經具有 update 功能。本次重跑上述兩個總和結果均符合預期。

## GCD 參考程式

[0a-gcd.c](0a-gcd.c) 原始標頭註記作者 **D. J. Guan**，在本 repository 保留作者資訊。它是課程資料夾內的參考範例，不宣稱由本帳戶作者自行創作。

程式讀取整數對，使用 Euclidean Algorithm 反覆取餘數。以非負數量表示，迭代次數為 O(log min(a,b))；測例 `48 18`、`-48 18`、`0 9` 分別得到 6、6、9。極值整數的絕對值／溢位未在這次驗證範圍內。

## 編譯

```powershell
gcc -std=gnu11 extras/segment_tree.c -o build/segment-sum.exe
gcc -std=gnu11 extras/0a-gcd.c -o build/gcd.exe
.\build\segment-sum.exe
'48 18' | .\build\gcd.exe
```

參考原碼的保留不表示取得新的再授權。使用或轉載時請保留原有作者註記並確認所需權利。
