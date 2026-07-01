#include <stdio.h>
#include <string.h>
#include <ctype.h>

FILE *fp;
char c;
char lexbuf[50], symtab[50][20];
int lineno = 1, i = 0;


char *kw[] = {
    "void", "int", "float", "double", "short", "long", "if",
    "else", "switch", "case", "break", "return", "main",
    "static", "goto"
};


char delim[] = { '(', ')', '{', '}', '[', ']', ';', ',' };
char oper[]  = { '+', '=', '-', '*', '/', '<', '>' };


int isdelim(char d) {
    for (int k = 0; k < sizeof(delim) / sizeof(delim[0]); k++) {
        if (d == delim[k])
            return 1;  
    }
    return 0;  
}

int isoper(char op) {
    for (int k = 0; k < sizeof(oper) / sizeof(oper[0]); k++) {
        if (op == oper[k])
            return 1;  
    }
    return 0;   
}

int iskw(char s[]) {
    for (int k = 0; k < sizeof(kw) / sizeof(kw[0]); k++) {
        if (strcmp(s, kw[k]) == 0)
            return 1;  
    }
    return 0;   
}


int lookup(char s[]) {
    for (int k = 0; k < i; k++) {
        if (strcmp(s, symtab[k]) == 0)
            return 1;
    }
    return 0;
}

void main() {

    fp = fopen("sample.c", "r");
    if (!fp) {
        printf("Cannot open file.\n");
        return;
    }

  
    while ((c = getc(fp)) != EOF) 
    {
        if (c == '/')
        {
            char next = getc(fp);
            if (next == '/') 
            {
                while ((c = getc(fp)) != '\n' && c != EOF)
                {
                    
                }
                lineno++;
            }   
            else 
            {
                ungetc(next, fp);   
                printf("/\t\tOperator\n");
            }
        }
        else if (c == ' ' || c == '\t') {
            continue;
        }

      
        else if (c == '\n') {
            lineno++;
        }

        
        else if (isdelim(c)) {
            printf("%c\t\tDelimiter\n", c);
        }

      
        else if (isoper(c)) {
            printf("%c\t\tOperator\n", c);
        }

     
        else if (isdigit(c)) {
            int b = 0;
            int isFloat = 0;
            while (isdigit(c) || c == '.') {
                if (c == '.') isFloat = 1;
                lexbuf[b++] = c;
                c = getc(fp);
            }
            ungetc(c, fp);
            lexbuf[b] = '\0';

            if (isFloat)
                printf("%s\t\tFloat\n", lexbuf);
            else
                printf("%s\t\tDigit\n", lexbuf);
        }

        
        else if (isalpha(c) || c == '_') {
            int b = 0;
            while (isalpha(c) || isdigit(c) || c == '_') {
                lexbuf[b++] = c;
                c = getc(fp);
            }
            ungetc(c, fp);
            lexbuf[b] = '\0';

            if (!lookup(lexbuf) && !iskw(lexbuf)) {
                strcpy(symtab[i++], lexbuf);
            }

            if (iskw(lexbuf)) {
                printf("%s\t\tKeyword\n", lexbuf);
            } else {
                printf("%s\t\tIdentifier\n", lexbuf);
            }
        }

        
        else {
            printf("%c\t\tUnknown\n", c);
        }
    }

    fclose(fp);
    printf("\nNumber of lines = %d\n", lineno-1);
}

