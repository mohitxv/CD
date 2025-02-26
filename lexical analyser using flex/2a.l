%{
/* need this for the call to atof() below */
#include <math.h>
%}

DIGIT [0-9]
ID [a-z][a-z0-9]*

%%
{DIGIT}+ {printf("An integer: %s (%d)\n", yytext, atoi(yytext));}
{DIGIT}+"."{DIGIT}* {printf("A float: %s (%g)\n", yytext, atof(yytext));}
int|main|return|if|then|begin|end|procedure|function {printf("A keyword: %s\n", yytext);}
{ID} {printf("An identifier: %s\n", yytext);}
"+"|"-"|"*"|"/"|"=" {printf("An operator: %s\n", yytext);}
"{"|"}"|"("|")" {printf("Brackets: %s\n", yytext);} /* eat up one-line comments */
[\t\n]+ /* eat up whitespace */
","|";"|" " {printf("Special symbol: %s\n", yytext);}

. printf("Unrecognized character: %s\n", yytext);
%%
int yywrap(){}

int main(int argc, char **argv)
{
    FILE *fp;
    char filename[50];

    printf("Enter the filename: \n");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    yyin = fp;
    yylex();

    fclose(fp);
    return 0;
}