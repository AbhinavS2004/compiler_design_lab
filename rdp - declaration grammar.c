#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[100];
int pos = 0;
char lookahead;

void error() {
    printf("Syntax Error at position %d near '%c'\n", pos, input[pos]);
    exit(1);
}

void print_step(const char *production) {
    printf("| %-15s | %-17s |\n", &input[pos], production);
}

void D();
void L();

void D() {
    print_step("D → type L ;");

    // Match type
    if (strncmp(&input[pos], "int", 3) == 0) {
        pos += 3;
    } else if (strncmp(&input[pos], "float", 5) == 0) {
        pos += 5;
    } else {
        error();
    }

    // Allow exactly one optional space after type
    if (input[pos] == ' ')
        pos++;

    lookahead = input[pos];

    // Call L
    L();

    // Expect ';'
    if (input[pos] == ';') {
        pos++;
        lookahead = input[pos];
    } else {
        error();
    }
}

void L() {
    if (input[pos] == 'i') {
        pos++;
        lookahead = input[pos];
        if (input[pos] == ',') {
            print_step("L → i , L");
            pos++;
            lookahead = input[pos];
            L();
        } else {
            print_step("L → i");
        }
    } else {
        error();
    }
}

int main() {
    printf("Enter input (end with $): ");
    scanf("%[^\n]", input);  // read full line (handles single space)

    lookahead = input[0];

    // Grammar
    printf("\n=== Grammar Used ===\n");
    printf("D → type L ;\n");
    printf("L → i , L | i\n");

    // Parsing Table Header
    printf("\n=== Parsing Steps ===\n");
    printf("+----------------+-------------------+\n");
    printf("| Remaining Input| Production Used   |\n");
    printf("+----------------+-------------------+\n");

    D();

    if (input[pos] == '$') {
        printf("+----------------+-------------------+\n");
        printf("\nParsing successful.\n");
    } else {
        error();
    }

    return 0;
}

