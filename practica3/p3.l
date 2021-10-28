/* No se va a escribir la función 'yywrap'*/
%option noyywrap

%{
	// Incluye la tabla de tokens
	#include "y.tab.h"

	// Definiciones de colores para la terminal
	#define RED "\e[0;31m"
	#define NC  "\e[0m"

	// Inicializa el contador de líneas a 1
	int n_lineas = 1;
%}

/* TOKENS */
INIPROG  inicio
ABRPAR   \(
CERPAR   \)
INIVAR   inivar
FINVAR   finvar
CERLLA   \}
ABRLLA   \{
DEFLISTA "lista"\ +"de"
ASIG     =
IF       si
ELSE     sino
WHILE    mientras
SCAN     escanear
RETURN   repatriar
FOR      para
TO       hasta
COMA     ,
PRINT    anunciar
PYC      ;
MAS      \+
CONCAT   \*\*
ARROBA   @
POR      \*
INITER   \$
ABRCOR   \[
CERCOR   \]
CADENA   \"[^"]*\"
IDENTIF  [A-Za-z][A-Za-z0-9]*
LITERAL  [0-9]*\.[0-9]*|[0-9]+|'.'|Verdadero|Falso

%%

 /* Reconoce el token y lo muestra por la salida */
{INIPROG}       { printf("INIPROG - %s\n", yytext); return(INIPROG); }
{INIPROG}       { printf("INIPROG - %s\n", yytext); return(INIPROG); }
{ABRPAR}        { printf("ABRPAR - %s\n", yytext); return(ABRPAR); }
{CERPAR}        { printf("CERPAR - %s\n", yytext); return(CERPAR); }
{INIVAR}        { printf("INIVAR - %s\n", yytext); return(INIVAR); }
{FINVAR}        { printf("FINVAR - %s\n", yytext); return(FINVAR); }
{CERLLA}        { printf("CERLLA - %s\n", yytext); return(CERLLA); }
{ABRLLA}        { printf("ABRLLA - %s\n", yytext); return(ABRLLA); }
{DEFLISTA}      { printf("DEFLISTA - %s\n", yytext); return(DEFLISTA); }
"entero"        { printf("TIPODATO - %s 0\n", yytext); return(TIPODATO); }
"real"          { printf("TIPODATO - %s 1\n", yytext); return(TIPODATO); }
"caracter"      { printf("TIPODATO - %s 2\n", yytext); return(TIPODATO); }
"booleano"      { printf("TIPODATO - %s 3\n", yytext); return(TIPODATO); }
{ASIG}          { printf("ASIG - %s\n", yytext); return(ASIG); }
{IF}            { printf("IF - %s\n", yytext); return(IF); }
{ELSE}          { printf("ELSE - %s\n", yytext); return(ELSE); }
{WHILE}         { printf("WHILE - %s\n", yytext); return(WHILE); }
{SCAN}          { printf("SCAN - %s\n", yytext); return(SCAN); }
{RETURN}        { printf("RETURN - %s\n", yytext); return(RETURN); }
{FOR}           { printf("FOR - %s\n", yytext); return(FOR); }
{TO}            { printf("TO - %s\n", yytext); return(TO); }
"aumentando"    { printf("SENTIDO - %s 0\n", yytext); return(SENTIDO); }
"decrementando" { printf("SENTIDO - %s 1\n", yytext); return(SENTIDO); }
{COMA}          { printf("COMA - %s\n", yytext); return(COMA); }
{PRINT}         { printf("PRINT - %s\n", yytext); return(PRINT); }
{PYC}           { printf("PYC - %s\n", yytext); return(PYC); }
{MAS}           { printf("MAS - %s\n", yytext); return(MAS); }
"/"             { printf("OPBIN - %s 0\n", yytext); return(OPBIN); }
"^"             { printf("OPBIN - %s 1\n", yytext); return(OPBIN); }
"<"             { printf("OPBIN - %s 2\n", yytext); return(OPBIN); }
"<="            { printf("OPBIN - %s 3\n", yytext); return(OPBIN); }
">"             { printf("OPBIN - %s 4\n", yytext); return(OPBIN); }
">="            { printf("OPBIN - %s 5\n", yytext); return(OPBIN); }
"y"             { printf("OPBIN - %s 6\n", yytext); return(OPBIN); }
"o"             { printf("OPBIN - %s 7\n", yytext); return(OPBIN); }
"oex"           { printf("OPBIN - %s 8\n", yytext); return(OPBIN); }
"no"            { printf("OPUNARIO - %s 0\n", yytext); return(OPUNARIO); }
"#"             { printf("OPUNARIO - %s 1\n", yytext); return(OPUNARIO); }
"?"             { printf("OPUNARIO - %s 2\n", yytext); return(OPUNARIO); }
{CONCAT}        { printf("CONCAT - %s\n", yytext); return(CONCAT); }
"%"             { printf("BORRAR - %s 0\n", yytext); return(BORRAR); }
"-"             { printf("BORRAR - %s 1\n", yytext); return(BORRAR); }
{ARROBA}        { printf("ARROBA - %s\n", yytext); return(ARROBA); }
{POR}           { printf("POR - %s\n", yytext); return(POR); }
{INITER}        { printf("INITER - %s\n", yytext); return(INITER); }
"<<"            { printf("ITER - %s 0\n", yytext); return(ITER); }
">>"            { printf("ITER - %s 1\n", yytext); return(ITER); }
{ABRCOR}        { printf("ABRCOR - %s\n", yytext); return(ABRCOR); }
{CERCOR}        { printf("CERCOR - %s\n", yytext); return(CERCOR); }
{CADENA}        { printf("CADENA - %s\n", yytext); return(CADENA); }
{IDENTIF}       { printf("IDENTIF - %s\n", yytext); return(IDENTIF); }
{LITERAL}       { printf("LITERAL - %s\n", yytext); return(LITERAL); }

 /*
 	Ignora el espacio en blanco (espacio, tabuladores y saltos de línea)
 	Si lee un salto de línea (\n), incrementa el contador de líneas
 */
[ \t\n]  {
	      if (yytext[0] == '\n')
         	++n_lineas;
	      }

 /* Regla por defecto, si lee un caracter que no es un token, muestra un error */
.  { fprintf(stderr, RED "[ERROR LÉXICO]" NC ": Token inesperado en la linea %d: %s\n", n_lineas, yytext); }

%%

// TODO: borrar esto y pasarlo a un main nuevo
int main() {
	yylex();
}
