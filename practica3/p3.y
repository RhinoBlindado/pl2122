%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror ( char * msg ) ;

// int n_lineas = 1;
%}

/* Muestra los errores con más alegría */
// %error-verbose
%define parse.error verbose

%start programa

/* Tabla de tokens */
%token INIPROG
%token ABRPAR
%token CERPAR
%token INIVAR
%token FINVAR
%token CERLLA
%token ABRLLA
%token DEFLISTA
%token TIPODATO
%token ASIG
%token IF
%token ELSE
%token WHILE
%token SCAN
%token RETURN
%token FOR
%token TO
%token SENTIDO
%token COMA
%token PRINT
%token PYC
%token MAS
%token OPBIN
%token OPUNARIO
%token CONCAT
%token BORRAR
%token ARROBA
%token POR
%token INITER
%token ITER
%token ABRCOR
%token CERCOR
%token CADENA
%token IDENTIF
%token LITERAL


%%

programa                      : cabeceraPrograma bloque;

cabeceraPrograma              : INIPROG;

bloque                        : inicioBloque declararVariablesLocalesMulti
										  declararFuncionMulti sentencias finBloque;

declararFuncionMulti          : declararFuncionMulti
										| declararFuncion
										| /* cadena vacía */
										;

declararFuncion               : cabeceraFuncion bloque;

declararVariablesLocalesMulti : marcaInicioVariable variablesLocalesMulti
										  marcaFinVar
										| /* cadena vacía*/
										;

cabeceraFuncion               : tipoDato identificador inicioParametros
										  parametros finParametros;

inicioParametros              : ABRPAR;

finParametros                 : CERPAR;

marcaInicioVariable           : INIVAR;

marcaFinVar                   : FINVAR finSentencia;

inicioBloque                  : ABRLLA;

finBloque                     : CERLLA;

variablesLocalesMulti         : variablesLocalesMulti variableLocal
										| /* cadena vacía */
										;

variableLocal                 : tipoDato variableSolitaria identificador
										  finSentencia;

variableSolitaria             : variableSolitaria identificador COMA
										| /* cadena vacía */
										;

tipoDato                      : tipoElemental
										| DEFLISTA tipoElemental
										;

tipoElemental                 : TIPODATO;

sentencias                    : sentencias sentencia
										| sentencia
										;

sentencia                     : bloque
										| sentenciaAsignacion
										| sentenciaIf
										| sentenciaWhile
										| sentenciaEntrada
										| sentenciaSalida
										| sentenciaReturn
										| sentenciaFor
										| sentenciaLista
										;

sentenciaAsignacion           : identificador ASIG expresion finSentencia;

sentenciaIf                   : IF inicioParametros expresion finParametros
										  sentencia
										| IF inicioParametros expresion finParametros
										  sentencia ELSE sentencia
										;

sentenciaWhile                : WHILE inicioParametros expresion finParametros
										  sentencia;

sentenciaEntrada              : nombreEntrada listaVariables finSentencia;

nombreEntrada                 : SCAN;

listaVariables                : inicioParametros parametros finParametros;

sentenciaReturn               : RETURN expresion finSentencia;

sentenciaFor                  : FOR sentenciaAsignacion TO expresion sentido
										  sentencia
										| FOR expresion TO expresion sentido sentencia
										;

sentido                       : SENTIDO;

sentenciaSalida               : nombreSalida inicioParametros
										  listaExpresionesCadena finParametros
										  finSentencia;

listaExpresionesCadena        : expresion
										| cadena
										| listaExpresionesCadena COMA expresion
										;

nombreSalida                  : PRINT;

parametros                    : tipoDato identificador
                              | parametros COMA tipoDato identificador
										| /* cadena vacía */
										;

expresion                     : ABRPAR expresion CERPAR
										| opUnario expresion
										| expresion opBinario expresion
										| identificador
										| literal
										| funcion
										| expresionLista
										;

funcion                       : identificador ABRPAR argumentos CERPAR;

argumentos                    : expresion
										| argumentos COMA expresion
										;

identificador                 : IDENTIF;

opBinario                     : OPBIN;

opUnario                      : OPUNARIO;

finSentencia                  : PYC;

lista                         : ABRCOR contenidoLista CERCOR;

contenidoLista : expresion
               | contenidoLista COMA expresion;

sentenciaLista : identificador ITER finSentencia
               | INITER identificador finSentencia;

expresionLista : OPUNARIO expresion
               | expresion OPBIN expresion ARROBA expresion
               | expresion OPBIN expresion;

/* Literales */
literal : LITERAL | lista;

cadena : CADENA;

%%

/* Incluye el fichero c generado por flex*/
#include "lex.yy.c"

/* Se implementa la función yyerror */
void yyerror( char *msg )
{
	fprintf(stderr, "[Linea %d]: %s\n", n_lineas, msg);
}
