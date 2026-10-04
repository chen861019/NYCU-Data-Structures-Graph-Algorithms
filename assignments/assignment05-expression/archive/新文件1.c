#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

// 初始化數值堆疊
void initNumStack(NumStack *s) {
    s->top = -1;
}

int isNumStackEmpty(NumStack *s) {
    return s->top == -1;
}

void numPush(NumStack *s, double value) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = value;
    }
}

double numPop(NumStack *s) {
    return isNumStackEmpty(s) ? 0 : s->data[(s->top)--];
}

// 初始化字符堆疊
void initCharStack(CharStack *s) {
    s->top = -1;
}

int isCharStackEmpty(CharStack *s) {
    return s->top == -1;
}

void charPush(CharStack *s, char c) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = c;
    }
}

char charPop(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[(s->top)--];
}

char charPeek(CharStack *s) {
    return isCharStackEmpty(s) ? '\0' : s->data[s->top];
}

int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '(': return 0;
        default: return 0;
    }
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero.\n");
                return NAN; // 或者可以選擇拋出錯誤
            }
            return a / b;
        default: return 0;
    }
}

double evaluateExpression(char *expr, VariableTable *vars) {
    NumStack values;
    CharStack operators;
    initNumStack(&values);
    initCharStack(&operators);

    char variableName = 0;
    if (isalpha(*expr) && strchr(expr, '=') != NULL) {
        variableName = *expr;
        expr += 2;
    }

    char *token = expr; 
    while (*token != '\0') {
        if (isdigit(*token) || (*token == '.' && isdigit(*(token + 1)))) {
            double value = strtod(token, &token);
            numPush(&values, value);
        } else if (isOperator(*token)) {
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
            int index = *token - 'a';
            if (index >= 0 && index < MAX_VAR) {
                numPush(&values, vars->values[index]);
            } else {
                printf("Error: Variable '%c' is not defined.\n", *token);
                return 0;
            }
            token++;
        } else {
            token++;
        }
    }

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

void runTests() {
    VariableTable vars = {0};

    char *tests[] = {           
        "1 + 2 * 3",                         
        "(1 + 2) * (3 - 4) / 5",              
        "a = 3.5",                            
        "b = 2 * a + 5",                     
        "x = 5", "y = x + 2 * 3",             
        "z = (x + y) / 2",                    
        "10 / (5 - 5)",                       
        "x + y"           
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("Running tests in DEBUG_MODE:\n");
    for (int i = 0; i < num_tests; i++) {
        printf("Input: %s\n", tests[i]);
        if (strchr(tests[i], '=')) {
            evaluateExpression(tests[i], &vars);
        } else {
            double result = evaluateExpression(tests[i], &vars);
            printf("Result: %g\n", result);
        }
    }
}

int main() {
    VariableTable vars = {0};
    char line[MAX_EXPR];

#ifdef DEBUG
    runTests();
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

