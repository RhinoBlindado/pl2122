%option noyywrap

%{
    #include "y.tab.h"
%}

%%

"("	return(APAR);
"["	return(ACOR);
")"	return(CPAR);
"]" return(CCOR);
,	return(SEP);
[a-z]	return(ELEM);

[ \t\n]	;
.	fprintf(stderr, "error lexico: %s\n", yytext);

%%