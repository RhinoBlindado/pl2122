%option noyywrap
%{
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
DEFLISTA "lista de"
TIPODATO entero|real|caracter|booleano
ASIG     =
IF       si
ELSE     sino
WHILE    mientras
SCAN     escanear
RETURN   repatriar
FOR      para
TO       hasta
SENTIDO  aumentando|decrementando
COMA     ,
PRINT    anunciar
PYC      ;
MAS      \+
MENOS    -
OPBIN    \/|\^|<|<=|>|>=|y|o|oex
OPUNARIO no|#|\?
CONCAT   \*\*
BORRAR   %|-
ARROBA   @
POR      \*
INITER   \$
ITER     \<<|>>
ABRCOR   \[
CERCOR   \]
CADENA   \"[^"]*\"
IDENTIF  [A-Za-z][A-Za-z0-9]*
LITERAL  [0-9]*\.[0-9]*|[0-9]+|'.'|Verdadero|Falso

%%

 /* Reconoce el token y lo muestra por la salida */
{INIPROG}  { printf("INIPROG "); }
{ABRPAR}   { printf("ABRPAR "); }
{CERPAR}   { printf("CERPAR "); }
{INIVAR}   { printf("INIVAR "); }
{FINVAR}   { printf("FINVAR "); }
{CERLLA}   { printf("CERLLA "); }
{ABRLLA}   { printf("ABRLLA "); }
{DEFLISTA} { printf("DEFLISTA "); }
{TIPODATO} { printf("TIPODATO "); }
{ASIG}     { printf("ASIG "); }
{IF}       { printf("IF "); }
{ELSE}     { printf("ELSE "); }
{WHILE}    { printf("WHILE "); }
{SCAN}     { printf("SCAN "); }
{RETURN}   { printf("RETURN "); }
{FOR}      { printf("FOR "); }
{TO}       { printf("TO "); }
{SENTIDO}  { printf("SENTIDO "); }
{COMA}     { printf("COMA "); }
{PRINT}    { printf("PRINT "); }
{PYC}      { printf("PYC "); }
{MAS}      { printf("MAS "); }
{MENOS}    { printf("MENOS "); }
{OPBIN}    { printf("OPBIN "); }
{OPUNARIO} { printf("OPUNARIO "); }
{CONCAT}   { printf("CONCAT "); }
{BORRAR}   { printf("BORRAR "); }
{ARROBA}   { printf("ARROBA "); }
{POR}      { printf("POR "); }
{INITER}   { printf("INITER "); }
{ITER}     { printf("ITER "); }
{ABRCOR}   { printf("ABRCOR "); }
{CERCOR}   { printf("CERCOR "); }
{CADENA}   { printf("CADENA "); }
{IDENTIF}  { printf("IDENTIF "); }
{LITERAL}  { printf("LITERAL "); }

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

int main() {
	yylex();
}
