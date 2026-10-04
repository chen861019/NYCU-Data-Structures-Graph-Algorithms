#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VAR 26
#define MAX_EXPR 256
#define STACK_SIZE 100

typedef struct {
    char *vars[MAX_VAR];
    double values[MAX_VAR];
} VariableTable;

typedef struct {
    char data[STACK_SIZE];
    int top;
} CharStack;

void initStack(CharStack *s) {
    s->top = -1;
}

int isEmpty(CharStack *s) {
    return s->top == -1;
}

void push(CharStack *s, char c) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = c;
    }
}

char pop(CharStack *s) {
    return isEmpty(s) ? '\0' : s->data[(s->top)--];
}

char peek(CharStack *s) {
    return isEmpty(s) ? '\0' : s->data[s->top];
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

void parseAssignment(char *line, VariableTable *vars) {
    char var;
    double value;
    sscanf(line, "%c = %lf", &var, &value);
    if (var >= 'a' && var <= 'z') {
        vars->vars[var - 'a'] = (char *)malloc(sizeof(char) * 2);
        vars->vars[var - 'a'][0] = var;
        vars->vars[var - 'a'][1] = '\0';
        vars->values[var - 'a'] = value;
    }
}

double evaluateExpression(char *expr, VariableTable *vars) {
    CharStack operators;
    CharStack values;
    initStack(&operators);
    initStack(&values);

    char *token = strtok(expr, " ");
    while (token) {
        if (isdigit(token[0])) {
            double value = atof(token);
            push(&values, value);
        } else if (isOperator(token[0])) {
            while (!isEmpty(&operators) && precedence(peek(&operators)) >= precedence(token[0])) {
                double b = pop(&values);
                double a = pop(&values);
                char op = pop(&operators);
                push(&values, applyOperator(a, b, op));
            }
            push(&operators, token[0]);
        } else if (isalpha(token[0])) {
            int index = token[0] - 'a';
            if (index >= 0 && index < MAX_VAR && vars->vars[index]) {
                push(&values, vars->values[index]);
            } else {
                printf("Error: Variable '%c' is not defined.\n", token[0]);
                return 0;
            }
        }
        token = strtok(NULL, " ");
    }

    while (!isEmpty(&operators)) {
        double b = pop(&values);
        double a = pop(&values);
        char op = pop(&operators);
        push(&values, applyOperator(a, b, op));
    }

    return pop(&values);
}

int main() {
    VariableTable vars = {0};
    char line[MAX_EXPR];

    printf("Enter expressions or assignments (Ctrl+Z to end):\n");
    while (fgets(line, sizeof(line), stdin)) {
        if (strchr(line, '=')) {
            parseAssignment(line, &vars);
        } else {
            double result = evaluateExpression(line, &vars); // 修正此行，確保傳遞兩個參數
            printf("Result: %.6lf\n", result);
        }
    }

    // Free allocated memory for variables
    for (int i = 0; i < MAX_VAR; i++) {
        free(vars.vars[i]);
    }

    return 0;
}
