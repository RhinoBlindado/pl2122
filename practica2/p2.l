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
DEFLISTA "lista"\ +"de"
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
{INIPROG}  { printf("INIPROG - %s\n", yytext); }
{ABRPAR}   { printf("ABRPAR - %s\n", yytext); }
{CERPAR}   { printf("CERPAR - %s\n", yytext); }
{INIVAR}   { printf("INIVAR - %s\n", yytext); }
{FINVAR}   { printf("FINVAR - %s\n", yytext); }
{CERLLA}   { printf("CERLLA - %s\n", yytext); }
{ABRLLA}   { printf("ABRLLA - %s\n", yytext); }
{DEFLISTA} { printf("DEFLISTA - %s\n", yytext); }
"entero"   { printf("TIPODATO - %s 0\n", yytext); }
"real"     { printf("TIPODATO - %s 1\n", yytext); }
"caracter" { printf("TIPODATO - %s 2\n", yytext); }
"booleano" { printf("TIPODATO - %s 3\n", yytext); }
{ASIG}     { printf("ASIG - %s\n", yytext); }
{IF}       { printf("IF - %s\n", yytext); }
{ELSE}     { printf("ELSE - %s\n", yytext); }
{WHILE}    { printf("WHILE - %s\n", yytext); }
{SCAN}     { printf("SCAN - %s\n", yytext); }
{RETURN}   { printf("RETURN - %s\n", yytext); }
{FOR}      { printf("FOR - %s\n", yytext); }
{TO}       { printf("TO - %s\n", yytext); }
"aumentando" { printf("SENTIDO - %s 0\n", yytext); }
"decrementando" { printf("SENTIDO - %s 1\n", yytext); }
{COMA}     { printf("COMA - %s\n", yytext); }
{PRINT}    { printf("PRINT - %s\n", yytext); }
{PYC}      { printf("PYC - %s\n", yytext); }
{MAS}      { printf("MAS - %s\n", yytext); }
"/"        { printf("OPBIN - %s 0\n", yytext); }
"^"        { printf("OPBIN - %s 1\n", yytext); }
"<"        { printf("OPBIN - %s 2\n", yytext); }
"<="       { printf("OPBIN - %s 3\n", yytext); }
">"        { printf("OPBIN - %s 4\n", yytext); }
">="       { printf("OPBIN - %s 5\n", yytext); }
"y"        { printf("OPBIN - %s 6\n", yytext); }
"o"        { printf("OPBIN - %s 7\n", yytext); }
"oex"      { printf("OPBIN - %s 8\n", yytext); }
"no"       { printf("OPUNARIO - %s 0\n", yytext); }
"#"        { printf("OPUNARIO - %s 1\n", yytext); }
"?"        { printf("OPUNARIO - %s 2\n", yytext); }
{CONCAT}   { printf("CONCAT - %s\n", yytext); }
"%"        { printf("BORRAR - %s 0\n", yytext); }
"-"        { printf("BORRAR - %s 1\n", yytext); }
{ARROBA}   { printf("ARROBA - %s\n", yytext); }
{POR}      { printf("POR - %s\n", yytext); }
{INITER}   { printf("INITER - %s\n", yytext); }
"<<"       { printf("ITER - %s 0\n", yytext); }
">>"       { printf("ITER - %s 1\n", yytext); }
{ABRCOR}   { printf("ABRCOR - %s\n", yytext); }
{CERCOR}   { printf("CERCOR - %s\n", yytext); }
{CADENA}   { printf("CADENA - %s\n", yytext); }
{IDENTIF}  { printf("IDENTIF - %s\n", yytext); }
{LITERAL}  { printf("LITERAL - %s\n", yytext); }

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
