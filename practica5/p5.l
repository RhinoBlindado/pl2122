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
"entero"                {
                            yylval.lexema = strdup(yytext);
                            yylval.type = ENTERO;
                            return(TIPODATO);
                        }
"real"                  {
                            yylval.lexema = strdup(yytext);
                            yylval.type = REAL;
                            return(TIPODATO);
                        }
"caracter"              {
                            yylval.lexema = strdup(yytext);
                            yylval.type = CARACTER;
                            return(TIPODATO);
                        }
"booleano"              {
                            yylval.lexema = strdup(yytext);
                            yylval.type = BOOLEANO;
                            return(TIPODATO);
                        }
"="                     return(ASIG);
"si"                    return(IF);
"sino"                  return(ELSE);
"mientras"              return(WHILE);
"escanear"              return(SCAN);
"repatriar"             return(RETURN);
"para"                  return(FOR);
"hasta"                 return(TO);
"aumentando"            yylval.atrib = 0; return(SENTIDO);
"decrementando"         yylval.atrib = 1; return(SENTIDO);
"anunciar"              return(PRINT);
";"                     return(PYC);
"+"                     yylval.atrib = 0; return(MASMENOS);
"-"                     yylval.atrib = 1; return(MASMENOS);
"/"                     yylval.atrib = 0; return(OPMULT);
"^"                     yylval.atrib = 1; return(OPMULT);
"*"                     yylval.atrib = 2; return(OPMULT);
"=="                    yylval.atrib = 0; return(IGUALDAD);
"!="                    yylval.atrib = 1; return(IGUALDAD);
"<"                     yylval.atrib = 0; return(DESIGUALDAD);
"<="                    yylval.atrib = 1; return(DESIGUALDAD);
">"                     yylval.atrib = 2; return(DESIGUALDAD);
">="                    yylval.atrib = 3; return(DESIGUALDAD);
"y"                     return(AND);
"o"                     return(OR);
"oex"                   return(XOR);
"++"                    return(PLUSPLUS);
"--"                    return(MINMIN);
"%"                     yylval.atrib = 3; return(OPMULT);
"**"                    return(CONCAT);
"no"                    return(NOT);
"#"                     return(HASH);
"?"                     return(INTERR);
"@"                     return(AT);
">>"                    yylval.atrib = 0; return(ITER);
"<<"                    yylval.atrib = 1; return(ITER);
"$"                     return(INITER);
\"[^"]*\"               {
                            yylval.lexema = strdup(yytext);
                            yylval.type = CARACTER;
                            yylval.isList = 1;
                            return(CADENA);
                        }
"Verdadero"             {
                            yylval.lexema = strdup(yytext);
                            yylval.type = BOOLEANO;
                            yylval.isList = 0;
                            return(LITERAL);
                        }
"Falso"                 {
                            yylval.lexema = strdup(yytext);
                            yylval.type = BOOLEANO;
                            yylval.isList = 0;
                            return(LITERAL);
                        }
[A-Za-z][A-Za-z0-9_]*    {
                            yylval.lexema = strdup(yytext);
                            // yylval.type = REAL;
                            return(IDENTIF);
                        }
[0-9]*\.[0-9]*          {
                            yylval.lexema = strdup(yytext);
                            yylval.type = REAL;
                            yylval.isList = 0;
                            return(LITERAL);
                        }
[0-9]+                  {
                            yylval.lexema = strdup(yytext);
                            yylval.type = ENTERO;
                            yylval.isList = 0;
                            return(LITERAL);
                        }
'\\?.'                     {
                            yylval.lexema = strdup(yytext);
                            yylval.type = CARACTER;
                            yylval.isList = 0;
                            return(LITERAL);
                        }

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
