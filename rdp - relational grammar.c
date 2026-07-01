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
    printf("| %-15s | %-20s |\n", &input[pos], production);
}

void R();
void RO();

void R() {
    if (isalpha(lookahead)) {
        print_step("R → id RO id");
        pos++;
        lookahead = input[pos];
        RO();

        if (isalpha(lookahead)) {
            pos++;
            lookahead = input[pos];
        } else {
            error();
        }
    } else {
        error();
    }
}

void RO() {
    if (lookahead == '<') {
        pos++;
        lookahead = input[pos];
        if (lookahead == '=') {
            print_step("RO → <=");
            pos++;
            lookahead = input[pos];
        } else {
            print_step("RO → <");
        }
    } else if (lookahead == '>') {
        pos++;
        lookahead = input[pos];
        if (lookahead == '=') {
            print_step("RO → >=");
            pos++;
            lookahead = input[pos];
        } else {
            print_step("RO → >");
        }
    } else if (lookahead == '=') {
        pos++;
        lookahead = input[pos];
        if (lookahead == '=') {
            print_step("RO → ==");
            pos++;
            lookahead = input[pos];
        } else {
            error();
        }
    } else if (lookahead == '!') {
        pos++;
        lookahead = input[pos];
        if (lookahead == '=') {
            print_step("RO → !=");
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
    printf("Enter a relational expression ending with $: ");
    scanf("%s", input);
    lookahead = input[0];

    printf("\n=== Grammar Used ===\n");
    printf("R  → id RO id\n");
    printf("RO → < | > | <= | >= | == | !=\n");

    printf("\n=== Parsing Steps ===\n");
    printf("+----------------+----------------------+\n");
    printf("| Remaining Input| Production Used      |\n");
    printf("+----------------+----------------------+\n");

    R();

    if (lookahead == '$') {
        printf("+----------------+----------------------+\n");
        printf("\nParsing successful.\n");
    } else {
        error();
    }

    return 0;
}
