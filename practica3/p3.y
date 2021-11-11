%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror ( const char * msg ) ;
int yylex(void);

// int n_lineas = 1;
%}

%define parse.error verbose

%start programa

/* Tabla de tokens */
%token INIPROG
%token ABRPAR
%token CERPAR
%token CERLLA
%token ABRLLA
%token ABRCOR
%token CERCOR
%token INIVAR
%token FINVAR
%token COMA
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
%token PRINT
%token PYC
%token MASMENOS
//%token PLUS
//%token MINUS

%token OPMULT
// %token SLASH
// %token HAT
// %token ASTERISC
// %token PERCENT

%token IGUALDAD
// %token EQ
// %token NEQ

%token DESIGUALDAD
// %token LT
// %token LE
// %token GT
// %token GE
%token AND
%token OR
%token XOR
%token PLUSPLUS
%token MINMIN
%token CONCAT
%token NOT
%token HASH
%token INTERR
%token AT
%token ITER
%token INITER
%token CADENA
%token IDENTIF
%token LITERAL

%left OR                        // OR lógico
%left AND // AND lógico
                                // OR bits
%left XOR                       // Exclusive OR
// AND bits
// %left EQ NEQ                    // Operadores de igualdad
%left IGUALDAD
%left DESIGUALDAD
// %left LT LE GT GE               // Operadores de relación
// %left PLUS MINUS                // Operadores aditivos
%left MASMENOS
%right OPMULT
// %left ASTERISC SLASH PERCENT HAT// Operadores multiplicativos

%right NOT                      // Operadores unarios
%right PLUSPLUS MINMIN
// Añadir Decrementos e Incrementos pos y prefijos.
%left ABRPAR                    // índices

// Listas
%right AT
%left ITER
%left INITER
%left HASH INTERR
%right CONCAT



%%

programa                      : cabeceraPrograma bloque;

cabeceraPrograma              : INIPROG;

bloque                        : inicioBloque declararVariablesLocalesMulti
                                declararFuncionMulti sentencias finBloque;

declararFuncionMulti          : declararFuncionMulti declararFuncion
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

variableSolitaria             : variableSolitaria identificador coma
                              | /* cadena vacía*/
                              ;

coma                          : COMA
                              | error
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

sentenciaAsignacion           : identificador ASIG expresion finSentencia
                              | error
                              ;

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
                              | listaExpresionesCadena COMA expresion
                              ;

nombreSalida                  : PRINT;

parametros                    : tipoDato identificador
                              | parametros COMA tipoDato identificador
                              | error
                              | /* cadena vacía */
                              ;

expresion                     : ABRPAR expresion CERPAR
                              | MASMENOS expresion %prec HASH
                              | NOT expresion %prec HASH
                              | expresion MASMENOS expresion
                              | expresion OPMULT expresion
                              // | expresion SLASH expresion
                              // | expresion HAT expresion
                              // | expresion ASTERISC expresion
                              // | expresion PERCENT expresion
                              | expresion IGUALDAD expresion
                              | expresion DESIGUALDAD expresion
                              | expresion AND expresion
                              | expresion OR expresion
                              | expresion XOR expresion
                              | identificador
                              | literal
                              | funcion
                              | HASH expresion %prec HASH
                              | INTERR expresion %prec HASH
                              | expresion PLUSPLUS expresion AT expresion
                              | expresion MINMIN expresion
                              | expresion CONCAT expresion
                              ;

funcion                       : identificador ABRPAR argumentos CERPAR;

argumentos                    : expresion
                              | argumentos COMA expresion
                              ;

identificador                 : IDENTIF;

finSentencia                  : PYC
                              | error
                              ;

lista                         : ABRCOR contenidoLista CERCOR;

contenidoLista                : expresion
                              | contenidoLista COMA expresion
                              ;

sentenciaLista                : identificador ITER finSentencia
                              | INITER identificador finSentencia
                              ;

literal                       : LITERAL | lista | cadena
                              ;

cadena                        : CADENA
                              ;

%%

/* Incluye el fichero c generado por flex*/
#include "lex.yy.c"

/* Se implementa la función yyerror */
void yyerror( const char *msg )
{
   fprintf(stderr, "[Linea %d]: %s\n", n_lineas, msg);
}
