//Programming Assignment V 
//Evaluation of Arithmetic Expression
//author : 陳沛安
//date: 2024/11/03
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define MAX_VAR 26          // 最大變數數量 (a-z)
#define MAX_EXPR 256        // 最大表達式長度
#define STACK_SIZE 100      // 堆疊大小

// 變數儲存結構
typedef struct {
    char vars[MAX_VAR];     // 變數名稱 (僅 a-z)
    double values[MAX_VAR]; // 變數對應的值
} VariableTable;

// 數字堆疊結構
typedef struct {
    double data[STACK_SIZE];
    int top;
} NumStack;

// 符號堆疊結構
typedef struct {
    char data[STACK_SIZE];
    int top;
} CharStack;

// 初始化數字堆疊
void initNumStack(NumStack *s) {
    s->top = -1;
}

// 檢查數字堆疊是否為空
int isNumStackEmpty(NumStack *s) {
    return s->top == -1;
}

// 將數字推入堆疊
void numPush(NumStack *s, double value) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = value;
#ifdef DEBUG
        printf("Pushing number: %g\n", value);
#endif
    }
}

// 從數字堆疊彈出數字
double numPop(NumStack *s) {
    return isNumStackEmpty(s) ? 0 : s->data[(s->top)--];
}

// 初始化符號堆疊
void initCharStack(CharStack *s) {
    s->top = -1;
}

// 檢查符號堆疊是否為空
int isCharStackEmpty(CharStack *s) {
    return s->top == -1;
}

// 將符號推入堆疊
void charPush(CharStack *s, char c) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = c;
#ifdef DEBUG
        printf("Pushing operator: %c\n", c);
#endif
    }
}

// 從符號堆疊彈出符號
char charPop(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[(s->top)--];
}

// 取得符號堆疊的頂端符號
char charPeek(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[s->top];
}

// 符號的優先級
int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '(': return 0;
        default: return 0;
    }
}

// 判斷是否為運算符號
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

// 應用運算符進行計算
double applyOperator(double a, double b, char op) {
#ifdef DEBUG
    printf("Applying operator: %c with operands %g and %g\n", op, a, b);
#endif
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero.\n");
                return NAN;
            }
            return a / b;
        default: return 0;
    }
}

// 評估並計算表達式
double evaluateExpression(char *expr, VariableTable *vars) {
    NumStack values;
    CharStack operators;
    initNumStack(&values);
    initCharStack(&operators);

    char variableName = 0; // 用於記錄賦值變數名稱
    if (isalpha(*expr) && strchr(expr, '=') != NULL) {
        variableName = *expr;
        expr += 2; // 略過 "a = "

        // 檢查賦值變數是否為大寫
        if (variableName >= 'A' && variableName <= 'Z') {
            fprintf(stderr, "Error: Invalid variable name '%c'. Cannot use uppercase variables.\n", variableName);
            exit(1); // 退出程式
        }
    }

    char *token = expr;
    while (*token != '\0') {
        // 處理數字
        if (isdigit(*token) || (*token == '.' && isdigit(*(token + 1)))) {
            double value = strtod(token, &token);
            numPush(&values, value);
        } else if (isOperator(*token)) {
            // 處理括號和優先級
            if (*token == '(') {
                charPush(&operators, *token);
            } else if (*token == ')') {
                while (!isCharStackEmpty(&operators) && charPeek(&operators) != '(') {
                    double b = numPop(&values);
                    double a = numPop(&values);
                    char op = charPop(&operators);
                    numPush(&values, applyOperator(a, b, op));
                }
                charPop(&operators);
            } else {
                while (!isCharStackEmpty(&operators) && precedence(charPeek(&operators)) >= precedence(*token)) {
                    double b = numPop(&values);
                    double a = numPop(&values);
                    char op = charPop(&operators);
                    numPush(&values, applyOperator(a, b, op));
                }
                charPush(&operators, *token);
            }
            token++;
        } else if (isalpha(*token)) {
            // 檢查變數是否在合法範圍內
            if (*token < 'a' || *token > 'z') {
                fprintf(stderr, "Error: Invalid variable name '%c'.\n", *token);
                exit(1); // 退出程式
            }

            int index = *token - 'a';
            if (index >= 0 && index < MAX_VAR) {
                numPush(&values, vars->values[index]);
#ifdef DEBUG
                printf("Pushing variable %c with value %g\n", *token, vars->values[index]);
#endif
            } else {
                printf("Error: Variable '%c' is not defined.\n", *token);
                return 0;
            }
            token++;
        } else {
            token++;
        }
    }

    // 計算剩餘運算
    while (!isCharStackEmpty(&operators)) {
        double b = numPop(&values);
        double a = numPop(&values);
        char op = charPop(&operators);
        numPush(&values, applyOperator(a, b, op));
    }

    double result = numPop(&values);
    if (variableName) {
        vars->values[variableName - 'a'] = result;
        printf("Assigned %c = %g\n", variableName, result);
    }
    return result;
}

// 測試函式，執行多個表達式
void runTests() {
    VariableTable vars = {0};
    char *tests[] = {
    "1 + 2 * 3",                      	  // 基本算術運算
    "(1 + 2) * (3 - 4) / 5",              // 包含括號的算術運算
    "a = 3.5",                            // 賦值運算
    "b = 2 * a + 5",                      // 使用變數
    "x = 5", "y = x + 2 * 3",          	  // 連續使用賦值變數
    "z = (x + y) / 2",                    // 使用多次變數的計算
    "(2 + 3 * (4 - 2)) / (3 + 1)",        // 嵌套括號運算
    "10 / (5 - 5)",                       // 除以零的情況
    "x + y",                              // 未定義變數錯誤測試
    "a = 10",                             // 測試賦值更新
    "a = a + 5",                          // 賦值後的運算更新
    "b = a * 2",                          // 賦值後使用變數進行運算
    "c = (b + a) / 3",                    // 多個變數的計算
    "a + b + c",                          // 多變數無賦值計算
    "(a + b) * (c - a) / 2",              // 多變數包含括號運算
    "d = c * b / a + 3",                  // 變數和常數混合運算
    "e = (d + c) * (a - 5) / b",          // 嵌套變數運算
    "f = e - d * 3 + a",                  // 更深的嵌套運算
    "g = (f + d) / (a + b + c)",          // 複雜表達式測試
    "h = g + a * b - f / e",              // 複雜算術操作
    "i = (h - g) + (c / d) * (e + f)",    // 更深的運算優先級測試
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    printf("Running tests in DEBUG_MODE:\n");
    for (int i = 0; i < num_tests; i++) {
      
        if (strchr(tests[i], '=')) {
            evaluateExpression(tests[i], &vars);
        } else {
            double result = evaluateExpression(tests[i], &vars);
            printf("Result: %g\n", result);
        }
    }
}

struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
} 

// 主程式
int main() {
    VariableTable vars = {0}; // 變數表
    char line[MAX_EXPR];      // 用於儲存輸入的表達式

#ifdef DEBUG
	struct timespec start, end, time_diff; // 計時變數 
	clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    runTests(); // 若啟用 DEBUG 模式，則執行測試
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("Execution time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);
#else
    printf("Enter expressions or assignments (Ctrl+Z to end):\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;

        if (strchr(line, '=')) {
            evaluateExpression(line, &vars);
        } else {
            double result = evaluateExpression(line, &vars);
            printf("Result: %g\n", result);
        }
    }
#endif

    return 0;
}

