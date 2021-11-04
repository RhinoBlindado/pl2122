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

%%

 /* Reconoce el token y lo muestra por la salida */
"inicio"                return(INIPROG);
"("                     return(ABRPAR);
")"                     return(CERPAR);
"}"                     return(CERLLA);
"{"                     return(ABRLLA);
"["                     return(ABRCOR);
"]"                     return(CERCOR);
"inivar"                return(INIVAR);
"finvar"                return(FINVAR);
","                     return(COMA);
lista[ \t\n]+de         return(DEFLISTA);
"entero"                return(TIPODATO);
"real"                  return(TIPODATO);
"caracter"              return(TIPODATO);
"booleano"              return(TIPODATO);
"="                     return(ASIG);
"si"                    return(IF);
"sino"                  return(ELSE);
"mientras"              return(WHILE);
"escanear"              return(SCAN);
"repatriar"             return(RETURN);
"para"                  return(FOR);
"hasta"                 return(TO);
"aumentando"            return(SENTIDO);
"decrementando"         return(SENTIDO);
"anunciar"              return(PRINT);
";"                     return(PYC);
"+"                     return(PLUS);
"-"                     return(MINUS);
"/"                     return(SLASH);
"^"                     return(HAT);
"*"                     return(ASTERISC);
"<"                     return(LT);
"<="                    return(LE);
">"                     return(GT);
">="                    return(GE);
"y"                     return(AND);
"o"                     return(OR);
"oex"                   return(XOR);
"++"                    return(PLUSPLUS);
"--"                    return(MINMIN);
"%"                     return(PERCENT);
"**"                    return(CONCAT);
"no"                    return(NOT);
"#"                     return(HASH);
"?"                     return(INTERR);
"@"                     return(AT);
">>"                    return(ITER);
"<<"                    return(ITER);
"$"                     return(INITER);
\"[^"]*\"               return(CADENA);
"Verdadero"             return(LITERAL);
"Falso"                 return(LITERAL);
[A-Za-z][A-Za-z0-9]*    return(IDENTIF);
[0-9]*\.[0-9]*          return(LITERAL);
[0-9]+                  return(LITERAL);
'.'                     return(LITERAL);

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
