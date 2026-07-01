#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char items[MAX];
    int top;
} Stack;

Stack opStack;

void initStack() {
    opStack.top = -1;
}

int isEmpty() {
    return opStack.top == -1;
}

void push(char c) {
    if (opStack.top == MAX - 1) {
        printf("Stack overflow\n");
        exit(1);
    }
    opStack.items[++opStack.top] = c;
}

char pop() {
    if (isEmpty()) {
        printf("Stack underflow\n");
        exit(1);
    }
    return opStack.items[opStack.top--];
}

char peek() {
    if (isEmpty()) return '\0';
    return opStack.items[opStack.top];
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void infixToPostfix(char *infix, char *postfix) {
    initStack();
    int i = 0, k = 0;
    char c;

    while ((c = infix[i++]) != '\0') {
        if (isspace(c)) continue;

        if (isalnum(c)) {
            // Copy operand (variables or digits)
            postfix[k++] = c;
        }
        else if (c == '(') {
            push(c);
        }
        else if (c == ')') {
            while (!isEmpty() && peek() != '(') {
                postfix[k++] = pop();
            }
            if (!isEmpty() && peek() == '(') {
                pop(); // Remove '('
            }
            else {
                printf("Mismatched parentheses\n");
                exit(1);
            }
        }
        else if (isOperator(c)) {
            while (!isEmpty() && precedence(peek()) >= precedence(c)) {
                postfix[k++] = pop();
            }
            push(c);
        }
        else {
            printf("Invalid character %c\n", c);
            exit(1);
        }
    }

    while (!isEmpty()) {
        if (peek() == '(') {
            printf("Mismatched parentheses\n");
            exit(1);
        }
        postfix[k++] = pop();
    }
    postfix[k] = '\0';
}


typedef struct {
    char op[3];
    char arg1[10];
    char arg2[10];
    char result[10];
} Quadruple;

Quadruple quads[MAX];
int quadCount = 0;

int tempCount = 0;
char tempName[10];

char* newTemp() {
    sprintf(tempName, "t%d", ++tempCount);
    return tempName;
}

void generateIntermediateCode(char *postfix) {
    char stack[MAX][10];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isalnum(c)) {
            char operand[2] = {c, '\0'};
            strcpy(stack[++top], operand);
        }
        else if (isOperator(c)) {
            if (top < 1) {
                printf("Invalid postfix expression\n");
                exit(1);
            }
            char arg2[10], arg1[10];
            strcpy(arg2, stack[top--]);
            strcpy(arg1, stack[top--]);

            char *temp = newTemp();

            strcpy(quads[quadCount].op, (char[]){c, '\0'});
            strcpy(quads[quadCount].arg1, arg1);
            strcpy(quads[quadCount].arg2, arg2);
            strcpy(quads[quadCount].result, temp);
            quadCount++;

            strcpy(stack[++top], temp);
        }
    }

    if (top != 0) {
        printf("Invalid postfix expression\n");
        exit(1);
    }

    printf("\nThree-Address Code:\n");
    for (int i = 0; i < quadCount; i++) {
        printf("%s = %s %s %s\n",
               quads[i].result,
               quads[i].arg1,
               quads[i].op,
               quads[i].arg2);
    }

    printf("\nQuadruples:\n");
    printf("Op\tArg1\tArg2\tResult\n");
    for (int i = 0; i < quadCount; i++) {
        printf("%s\t%s\t%s\t%s\n",
               quads[i].op,
               quads[i].arg1,
               quads[i].arg2,
               quads[i].result);
    }

    printf("\nFinal result stored in: %s\n", stack[top]);
}

int main() {
    char infix[MAX], postfix[MAX*2];

    printf("Enter an arithmetic expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0;

    infixToPostfix(infix, postfix);

    printf("\nPostfix expression: %s\n", postfix);

    generateIntermediateCode(postfix);

    return 0;
}
