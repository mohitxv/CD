%{
#include <stdio.h>
#include <math.h>
#include "calc.tab.h"
%}

%option noyywrap

%%

[0-9]+(\.[0-9]+)?         { yylval.fval = atof(yytext); return NUMBER; }
"sin"                     { return SIN; }
"cos"                     { return COS; }
"tan"                     { return TAN; }
"log"                     { return LOG; }
"ln"                      { return LN; }
"sqrt"                    { return SQRT; }
"pi"                      { yylval.fval = M_PI; return NUMBER; }
"e"                       { yylval.fval = M_E; return NUMBER; }

"+"                       { return '+'; }
"-"                       { return '-'; }
"*"                       { return '*'; }
"/"                       { return '/'; }
"^"                       { return '^'; }
"("                       { return '('; }
")"                       { return ')'; }

[ \t\n]+                  { /* Ignore whitespace */ }

.                         { printf("Unrecognized character: %s\n", yytext); }

%%