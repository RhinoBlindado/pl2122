%option noyywrap

%{
#include "y.tab.h"
%}

numero  [0-9]+

%%

{numero}    return(NUMERO);
"-"         return(MINUS);
"+"         return(PLUS);
"*"         return(MULT);
"/"         return(DIV);
"("         return(PIZQ);
")"         return(PDER);
";"         return(PCOMA);

[ \t\n]	;
.	fprintf(stderr, "error lexico: %s\n", yytext);

%%