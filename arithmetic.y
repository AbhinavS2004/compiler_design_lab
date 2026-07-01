%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
%}

%token NUMBER ID
%left '+' '-'
%left '*' '/'

%%

exp : exp '+' exp
    | exp '-' exp
    | exp '*' exp
    | exp '/' exp
    | '(' exp ')'
    | NUMBER
    | ID
    ;

%%

int main() {
    printf("Enter the expression: ");
    if (yyparse() == 0)
        printf("\nValid Expression\n");
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "\nInvalid Expression: %s\n", s);
    exit(1);
}
