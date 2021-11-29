%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Declaración de funciones/variables de yacc
void yyerror (const char *msg);
int yylex(void);
char* yytext;

// Pila
#define MAX_TS 500
int TS[MAX_TS];
const uint TOPE = 0;
uint pos = 0;

// Inserta un valor en la pila
void push(int val)
{
	TS[pos] = val;
	pos++;
}

// Devuelve el último valor de la pila y lo borra
int pop()
{
	if (pos > 0)
	{
		pos--;
	}
	return TS[pos];
}

%}

// Tabla de tokens
%token ELEM
%token SEP
%token APAR
%token ACOR
%token CPAR
%token CCOR

%%

lis : ini ELEM mas;
mas : SEP ELEM mas
    | fin { if (pop() == pop()) { printf("pog\n"); } else { printf("not pog\n"); }}
    ;
ini : APAR { push(1); }
    | ACOR { push(2); }
    ;
fin : CPAR { push(1); }
    | CCOR { push(2); }
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