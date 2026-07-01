%{
#include "y.tab.h"
%}

%%

[0-9]+                  { return NUMBER; }                // Integer numbers
[a-zA-Z][_a-zA-Z0-9]*   { return ID; }                    // Identifiers like a, sum1
\n                      { return 0; }                     // End of input (Enter key)
.                       { return yytext[0]; }             // Any single character (like +, -, etc.)

%%
int yywrap() { return 1; }
