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
    // i → E
    if (top >= 0 && stack[top] == 'i') {
        popn(1);
        push('E');
        sprintf(action, "Reduce: E -> i");
        return 1;
    }
    // (E) → E
    if (top >= 2 && stack[top] == ')' && stack[top - 2] == '(' && stack[top - 1] == 'E') {
        popn(3);
        push('E');
        sprintf(action, "Reduce: E -> (E)");
        return 1;
    }
    // E+E or E*E → E
    if (top >= 2 && stack[top] == 'E' && (stack[top - 1] == '+' || stack[top - 1] == '*') && stack[top - 2] == 'E') {
        char op = stack[top - 1];
        popn(3);
        push('E');
        if (op == '+') sprintf(action, "Reduce: E -> E+E");
        else sprintf(action, "Reduce: E -> E*E");
        return 1;
    }
    return 0;
}

int main() {
    char input[MAX];
    printf("Grammar:\n");
    printf("E -> E+E\n");
    printf("E -> E*E\n");
    printf("E -> (E)\n");
    printf("E -> i\n\n");

    printf("Enter input string:");
    fgets(input, sizeof(input), stdin);

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';

    printf("\n%-15s %-15s %-20s\n", "Stack", "Input", "Action");
    printf("---------------------------------------------------\n");

    // Print initial state
    print_stack();
    printf("%-15s %-15s %-20s\n", "", input, "");

    int i = 0;
    char action[50];

    while (1) {
        // Shift if input not empty
        if (input[i] != '\0') {
            push(input[i]);
            sprintf(action, "Shift '%c'", input[i]);
            i++;
            print_stack();
            printf("%-15s %-15s %-20s\n", "", input + i, action);

            // After every shift, reduce as much as possible
            while (try_reduce(action)) {
                print_stack();
                printf("%-15s %-15s %-20s\n", "", input + i, action);
            }
            continue;
        }

        // No more input, try final reductions
        if (try_reduce(action)) {
            print_stack();
            printf("%-15s %-15s %-20s\n", "", "", action);
            continue;
        }

        break; // done
    }

    // Final result
    if (top == 0 && stack[0] == 'E')
        printf("\nResult: ACCEPTED\n");
    else
        printf("\nResult: REJECTED\n");

    return 0;
}

