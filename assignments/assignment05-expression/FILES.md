# Assignment V：程式逐檔索引

[作業說明](README.md) · [圖片索引](RESULTS.md)

主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。

| 原始碼 | 角色 | GCC 7.4.0 編譯 |
| --- | --- | --- |
| [archive/p5.c](archive/p5.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/TEST.c](archive/TEST.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/test1.c](archive/test1.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/新文件1.c](archive/%E6%96%B0%E6%96%87%E4%BB%B61.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/新文件2.c](archive/%E6%96%B0%E6%96%87%E4%BB%B62.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/新文件3.c](archive/%E6%96%B0%E6%96%87%E4%BB%B63.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [archive/新文件4.c](archive/%E6%96%B0%E6%96%87%E4%BB%B64.c) | 原始演進／拆分實驗版本；詳細入口見下方函式索引 | 成功 |
| [src/Programming_Assignment_V.c](src/Programming_Assignment_V.c) | 雙 stack 求值、基本四則與變數指派 | 成功 |

## 函式入口

以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。

### archive/p5.c

`initStack`、`isEmpty`、`push`、`pop`、`peek`、`precedence`、`isOperator`、`applyOperator`、`parseAssignment`、`evaluateExpression`、`main`

### archive/TEST.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`parseAssignment`、`evaluateExpression`、`main`

### archive/test1.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`parseAssignment`、`runTests`、`main`

### archive/新文件1.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`runTests`、`main`

### archive/新文件2.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`runTests`、`main`

### archive/新文件3.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`runTests`、`main`

### archive/新文件4.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`parseAssignment`、`runTests`、`main`

### src/Programming_Assignment_V.c

`initNumStack`、`isNumStackEmpty`、`numPush`、`numPop`、`initCharStack`、`isCharStackEmpty`、`charPush`、`charPop`、`charPeek`、`precedence`、`isOperator`、`applyOperator`、`evaluateExpression`、`runTests`、`diff`、`main`
