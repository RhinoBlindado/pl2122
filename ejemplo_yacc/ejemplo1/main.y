%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror (const char *msg);
char* yytext;
int yylex(void);
%}

// Tabla de tokens
%token NUMERO
%token MINUS
%token PLUS
%token MULT
%token DIV
%token PIZQ
%token PDER
%token PCOMA

%left PLUS MINUS
%left MULT DIV
%right UNARIO

%start linea

%%

linea	: exp { printf("Valor actual: %d\n", $1); } PCOMA linea
    	|
		;

exp		: exp PLUS exp				{ $$ = $1 + $3; }
		| exp MINUS exp				{ $$ = $1 - $3; }
		| exp MULT exp				{ $$ = $1 * $3; }
		| exp DIV exp				{ $$ = $1 / $3; }
		| PIZQ exp PDER				{ $$ = $2; }
		| MINUS exp %prec UNARIO	{ $$ = -$2; }
		| NUMERO					{ sscanf(yytext, "%d", &$$); }
		;

%%

#include "lex.yy.c"

void yyerror(const char *msg)
{
	fprintf(stderr, "error sintactico\n");
}

int main()
{
	yyparse();
	return 0;
}