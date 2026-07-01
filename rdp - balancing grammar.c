#include <stdio.h>
#include <stdlib.h>

char input[200];
int pos = 0;
char lookahead;

void error() {
    printf("Syntax Error at position %d\n", pos);
    exit(1);
}

void print_step(const char *production) {
    printf("| %-20s | %-12s |\n", &input[pos], production);
}

/* B -> ( B ) B | epsilon */
void B() {
    if (lookahead == '(') {
        print_step("B -> ( B ) B");
        pos++;                      // consume '('
        lookahead = input[pos];
        B();                        // parse inner B
        if (lookahead == ')') {     // expect ')'
            pos++;                  // consume ')'
            lookahead = input[pos];
            B();                    // parse trailing B (concatenation)
        } else {
            error();                // missing ')'
        }
    } else {
        print_step("B -> epsilon"); // epsilon production
        return;
    }
}

int main() {
    printf("Enter parentheses string ending with $ (e.g. (()())$): ");
    if (scanf("%199s", input) != 1) return 0;
    lookahead = input[0];

    printf("\n=== Grammar Used ===\n");
    printf("B -> ( B ) B | epsilon\n");

    printf("\n=== Parsing Steps ===\n");
    printf("+----------------------+--------------+\n");
    printf("| Remaining Input      | Production   |\n");
    printf("+----------------------+--------------+\n");

    B();

    if (lookahead == '$') {
        printf("+----------------------+--------------+\n");
        printf("\nParsing successful: input is balanced.\n");
    } else {
        error();
    }

    return 0;
}
