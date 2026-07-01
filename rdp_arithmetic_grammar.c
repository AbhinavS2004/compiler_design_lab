#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char input[100];
int pos = 0;
char lookahead;

void error() {
    printf("Syntax Error at position %d\n", pos);
    exit(1);
}

void print_step(const char *production) {
    printf("| %-15s | %-17s |\n", &input[pos], production);
}

void E(); void Ep(); void T(); void Tp(); void F();

void E() {
    print_step("E → T E'");
    T();
    Ep();
}

void Ep() {
    if (input[pos] == '+') {
        print_step("E' → + T E'");
        pos++;
        lookahead = input[pos];
        T();
        Ep();
    } else if (input[pos] == '-') {
        print_step("E' → - T E'");
        pos++;
        lookahead = input[pos];
        T();
        Ep();
    } else {
        print_step("E' → ε ");
    }
}

void T() {
    print_step("T → F T'");
    F();
    Tp();
}

void Tp() {
    if (input[pos] == '*') {
        print_step("T' → * F T'");
        pos++;
        lookahead = input[pos];
        F();
        Tp();
    } else if (input[pos] == '/') {
        print_step("T' → / F T'");
        pos++;
        lookahead = input[pos];
        F();
        Tp();
    } else {
        print_step("T' → ε ");
    }
}

void F() {
    if (isalpha(input[pos])) {
        print_step("F → id");
        pos++;
        lookahead = input[pos];
    } else if (input[pos] == '(') {
        print_step("F → ( E )");
        pos++;
        lookahead = input[pos];
        E();
        if (input[pos] == ')') {
            pos++;
            lookahead = input[pos];
        } else {
            error();
        }
    } else {
        error();
    }
}

int main() {
    printf("Enter an arithmetic expression ending with $: ");
    scanf("%s", input);
    lookahead = input[0];

    // 🔹 Print Grammar Rules First
    printf("\n=== Grammar Used ===\n");
    printf("E  → T E'\n");
    printf("E' → + T E' | - T E' | ε\n");
    printf("T  → F T'\n");
    printf("T' → * F T' | / F T' | ε\n");
    printf("F  → id | (E)\n");

    // 🔹 Start Parsing
    printf("\n=== Parsing Steps ===\n");
    printf("+----------------+-------------------+\n");
    printf("| Remaining Input| Production Used   |\n");
    printf("+----------------+-------------------+\n");

    E();

    if (input[pos] == '$') {
        printf("+----------------+-------------------+\n");
        printf("\nParsing successful.\n");
    } else {
        error();
    }

    return 0;
}

