%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(const char *s);
int yylex();

%}

%union {
    double fval;
}

%token <fval> NUMBER
%token SIN COS TAN LOG LN SQRT

%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%type <fval> expression

%%

calculation:
    expression { printf("Result: %lf\n", $1); }
    ;

expression:
    NUMBER                  { $$ = $1; }
    | expression '+' expression  { $$ = $1 + $3; }
    | expression '-' expression  { $$ = $1 - $3; }
    | expression '*' expression  { $$ = $1 * $3; }
    | expression '/' expression  {
        if ($3 == 0) {
            yyerror("Error: Division by zero!");
            exit(EXIT_FAILURE);
        }
        $$ = $1 / $3;
    }
    | expression '^' expression  { $$ = pow($1, $3); }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')'         { $$ = $2; }
    | SIN '(' expression ')'     { $$ = sin($3); }
    | COS '(' expression ')'     { $$ = cos($3); }
    | TAN '(' expression ')'     { $$ = tan($3); }
    | LOG '(' expression ')'     {
        if ($3 <= 0) {
            yyerror("Error: Logarithm of non-positive number!");
            exit(EXIT_FAILURE);
        }
        $$ = log10($3);
    }
    | LN '(' expression ')'      {
        if ($3 <= 0) {
            yyerror("Error: Natural log of non-positive number!");
            exit(EXIT_FAILURE);
        }
        $$ = log($3);
    }
    | SQRT '(' expression ')'    {
        if ($3 < 0) {
            yyerror("Error: Square root of a negative number!");
            exit(EXIT_FAILURE);
        }
        $$ = sqrt($3);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    printf("Enter a mathematical expression: ");
    return yyparse();
}
