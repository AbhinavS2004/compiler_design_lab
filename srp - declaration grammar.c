#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

void popn(int n) {
    top -= n;
}

void print_stack() {
    for (int i = 0; i <= top; i++) putchar(stack[i]);
    if (top == -1) printf(" ");
}

int try_reduce(char *action) {
    // ---- T → i | f | c ----
    if (top >= 0 && (stack[top] == 'i' || stack[top] == 'f' || stack[top] == 'c')) {
        char t = stack[top];
        popn(1);
        push('T');
        sprintf(action, "Reduce: T -> %c", t);
        return 1;
    }

    // ---- L → L , v ----
    if (top >= 2 && stack[top] == 'v' && stack[top - 1] == ',' && stack[top - 2] == 'L') {
        popn(3);
        push('L');
        sprintf(action, "Reduce: L -> L , v");
        return 1;
    }

    // ---- L → v ----
    if (top >= 0 && stack[top] == 'v') {
        popn(1);
        push('L');
        sprintf(action, "Reduce: L -> v");
        return 1;
    }

    // ---- D → T L ; ----
    if (top >= 2 && stack[top] == ';' && stack[top - 1] == 'L' && stack[top - 2] == 'T') {
        popn(3);
        push('D');
        sprintf(action, "Reduce: D -> T L ;");
        return 1;
    }

    return 0;
}

int main() {
    char input[MAX];
    printf("Grammar:\n");
    printf("D -> T L ;\n");
    printf("T -> i | f | c\n");
    printf("L -> L , v | v\n\n");

    printf("Enter input string: ");
    fgets(input, sizeof(input), stdin);

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';

    printf("\n%-15s %-15s %-20s\n", "Stack", "Input", "Action");
    printf("---------------------------------------------------------------\n");

    print_stack();
    printf("%-15s %-15s %-20s\n", "", input, "");

    int i = 0;
    char action[50];

    while (1) {
        if (input[i] != '\0') {
            if (input[i] == ' ') { i++; continue; } // skip spaces

            push(input[i]);
            sprintf(action, "Shift '%c'", input[i]);
            i++;
            print_stack();
            printf("%-15s %-15s %-20s\n", "", input + i, action);

            // Try reductions repeatedly
            while (try_reduce(action)) {
                print_stack();
                printf("%-15s %-15s %-20s\n", "", input + i, action);
            }
            continue;
        }

        if (try_reduce(action)) {
            print_stack();
            printf("%-15s %-15s %-20s\n", "", "", action);
            continue;
        }

        break;
    }

    if (top == 0 && stack[0] == 'D')
        printf("\nResult: ACCEPTED ✅\n");
    else
        printf("\nResult: REJECTED ❌\n");

    return 0;
}
