#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VAR 26
#define MAX_EXPR 256
#define STACK_SIZE 100

typedef struct {
    char vars[MAX_VAR];
    double values[MAX_VAR];
} VariableTable;

typedef struct {
    double data[STACK_SIZE];
    int top;
} NumStack;

typedef struct {
    char data[STACK_SIZE];
    int top;
} CharStack;

// 函數宣告
double evaluateExpression(char *expr, VariableTable *vars);
void parseAssignment(char *line, VariableTable *vars);
void initNumStack(NumStack *s);
int isNumStackEmpty(NumStack *s);
void numPush(NumStack *s, double value);
double numPop(NumStack *s);
void initCharStack(CharStack *s);
int isCharStackEmpty(CharStack *s);
void charPush(CharStack *s, char c);
char charPop(CharStack *s);
char charPeek(CharStack *s);
int precedence(char op);
int isOperator(char c);
double applyOperator(double a, double b, char op);

// 初始化數值堆疊
void initNumStack(NumStack *s) {
    s->top = -1;
}

// 檢查數值堆疊是否為空
int isNumStackEmpty(NumStack *s) {
    return s->top == -1;
}

// 向數值堆疊推入數值
void numPush(NumStack *s, double value) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = value;
    }
}

// 從數值堆疊彈出數值
double numPop(NumStack *s) {
    return isNumStackEmpty(s) ? 0 : s->data[(s->top)--];
}

// 初始化字符堆疊
void initCharStack(CharStack *s) {
    s->top = -1;
}

// 檢查字符堆疊是否為空
int isCharStackEmpty(CharStack *s) {
    return s->top == -1;
}

// 向字符堆疊推入字符
void charPush(CharStack *s, char c) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = c;
    }
}

// 從字符堆疊彈出字符
char charPop(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[(s->top)--];
}

// 獲取堆疊頂部字符
char charPeek(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[s->top];
}

// 獲取運算符優先級
int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}

// 檢查字符是否為運算符
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// 應用運算符
double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

// 解析賦值
void parseAssignment(char *line, VariableTable *vars) {
    char var;
    double value;

    char *equal_sign = strchr(line, '=');
    if (equal_sign) {
        *equal_sign = '\0';
        var = line[0];
        char *expr = equal_sign + 1;

        value = evaluateExpression(expr, vars);
        if (var >= 'a' && var <= 'z') {
            vars->vars[var - 'a'] = var;
            vars->values[var - 'a'] = value;
        }
    }
}

// 評估表達式
double evaluateExpression(char *expr, VariableTable *vars) {
    NumStack values;
    CharStack operators;
    initNumStack(&values);
    initCharStack(&operators);

    char *token = expr; 
    while (*token != '\0') {
        if (isdigit(*token) || (*token == '.' && isdigit(*(token + 1)))) {
            double value = strtod(token, &token);
            numPush(&values, value);
        } else if (isOperator(*token)) {
            while (!isCharStackEmpty(&operators) && precedence(charPeek(&operators)) >= precedence(*token)) {
                double b = numPop(&values);
                double a = numPop(&values);
                char op = charPop(&operators);
                numPush(&values, applyOperator(a, b, op));
            }
            charPush(&operators, *token);
            token++;
        } else if (*token == '(') {
            charPush(&operators, *token);
            token++;
        } else if (*token == ')') {
            while (!isCharStackEmpty(&operators) && charPeek(&operators) != '(') {
                double b = numPop(&values);
                double a = numPop(&values);
                char op = charPop(&operators);
                numPush(&values, applyOperator(a, b, op));
            }
            charPop(&operators); // 移除 '('
            token++;
        } else if (isalpha(*token)) {
            int index = *token - 'a';
            if (index >= 0 && index < MAX_VAR) {
                numPush(&values, vars->values[index]);
            } else {
                printf("Error: Variable '%c' is not defined.\n", *token);
                return 0;
            }
            token++;
        } else {
            token++; // 忽略其他字符（例如空格）
        }
    }

    while (!isCharStackEmpty(&operators)) {
        double b = numPop(&values);
        double a = numPop(&values);
        char op = charPop(&operators);
        numPush(&values, applyOperator(a, b, op));
    }

    return numPop(&values);
}

// 主程式
int main() {
    VariableTable vars = {0};
    char line[MAX_EXPR];

    printf("Enter expressions or assignments (Ctrl+Z to end):\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0; // 移除換行字符

        if (strchr(line, '=')) {
            parseAssignment(line, &vars);
        } else {
            double result = evaluateExpression(line, &vars);
            printf("Result: %g\n", result);
        }
    }

    return 0;
}

