%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Activar modo debug.
//int yydebug=1; 

void yyerror (const char * msg);
int yylex(void);


/*
 * ANALIZADOR SEMANTICO
 */

#define YYSTYPE attr
#include "symbolTable.h"
#include "semantic.c"
uint IN_FUNC;

/*
 * GENERACION DE CODIGO INTERMEDIO 
 */

#include "codegen.c"

%}

/*INICIO YACC*/

%define parse.error verbose

%start programa

/* Tabla de tokens */ 
// #import "token.h"
/*###################### Esta mierda no se puede mover a un .h???*/
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
%token AT
%token PLUSPLUS
%token MINMIN
%token CONCAT
%token NOT
%token HASH
%token INTERR
%token ITER
%token INITER
%token CADENA
%token IDENTIF
%token LITERAL

%right AT

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
%right MASMENOS
%right OPMULT
// %left ASTERISC SLASH PERCENT HAT// Operadores multiplicativos

%right NOT                      // Operadores unarios
%right PLUSPLUS MINMIN
// Añadir Decrementos e Incrementos pos y prefijos.
%left ABRPAR                    // índices

// Listas
%left ITER
%left INITER
%left HASH INTERR
%right CONCAT

%%

programa                      : cabeceraPrograma { globalVars = 1; } bloque { closeFile();}
                              ; 

cabeceraPrograma              : INIPROG { openFile(); genHeaders(); };

bloque                        : inicioBloque declararVariablesLocalesMulti
                                declararFuncionMulti sentencias finBloque ; 

declararFuncionMulti          : declararFuncionMulti declararFuncion
                              | /* cadena vacía */
                              ;

declararFuncion               : cabeceraFuncion {IN_FUNC = 1;}
                                bloque {IN_FUNC = 0;}
                                ;

declararVariablesLocalesMulti : marcaInicioVariable variablesLocalesMulti
                                marcaFinVar { controlGlobalVars(); }
                              | /* cadena vacía*/
                              ;

cabeceraFuncion               : tipoDato identificador {$2.type = $1.type; insertFunction($2);}
                                inicioParametros
                                parametros finParametros;

inicioParametros              : ABRPAR;

finParametros                 : CERPAR;

marcaInicioVariable           : INIVAR;

marcaFinVar                   : FINVAR finSentencia;

inicioBloque                  : ABRLLA { blockStart(); writeStartBlock(); }
                              ;  

finBloque                     : CERLLA { blockEnd(); writeEndBlock();}
                              ;

variablesLocalesMulti         : variablesLocalesMulti variableLocal
                              | /* cadena vacía */
                              ;

variableLocal                 : tipoDato variableSolitaria identificador finSentencia { pushAttr($3); assignType($1); writeVars($1, $2, $3); } 
                              | error;

variableSolitaria             : variableSolitaria identificador coma { pushAttr($2); $$.nameTmp = concatVars($1, $2); }
                              | { $$.nameTmp = ""; } /* cadena vacía*/
                              ;

coma                          : COMA
                              | error
                              ;

tipoDato                      : tipoElemental { $$.type = $1.type; $$.isList = 0; $$.nameTmp = getCType($1.type); }
                              | DEFLISTA tipoElemental { $$.type = $2.type; $$.isList = 1; }
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

sentenciaAsignacion           : identificador ASIG expresion finSentencia { $$ = checkAsignacion($1, $3); }
                              | error
                              ;

sentenciaIf                   : IF inicioParametros expresion finParametros 
                                sentencia {checkBooleans($3.type);}
                              | IF inicioParametros expresion finParametros
                                sentencia ELSE sentencia {checkBooleans($3.type);}
                              ;

sentenciaWhile                : WHILE inicioParametros expresion finParametros
                                sentencia {checkBooleans($3.type);} ; 

sentenciaEntrada              : nombreEntrada listaVariables finSentencia;

nombreEntrada                 : SCAN;

listaVariables                : inicioParametros parametros finParametros;

sentenciaReturn               : RETURN expresion finSentencia{checkReturn($2);};

sentenciaFor                  : FOR sentenciaAsignacion TO expresion sentido
                                sentencia {if ($2.type != getExpType(ENTERO, $4.type)){ yyerror("Tipos distintos en bucle for"); }}
                              | FOR expresion TO expresion sentido sentencia {if (ENTERO != getExpType($2.type, $4.type)){ yyerror("Tipos distintos en bucle for"); }}
                              ;

sentido                       : SENTIDO;

sentenciaSalida               : nombreSalida inicioParametros
                                listaExpresionesCadena finParametros
                                finSentencia;

listaExpresionesCadena        : expresion
                              | listaExpresionesCadena COMA expresion
                              ;

nombreSalida                  : PRINT;

parametros                    : tipoDato identificador {$2.type = $1.type; insertFormalParameter($2);}
                              | parametros COMA tipoDato identificador {$4.type = $3.type; insertFormalParameter($4);}
                              | error
                              | /* cadena vacía */
                              ;

expresion                     : ABRPAR expresion CERPAR { $$ = $2; }
                              | MASMENOS expresion %prec HASH { $$.type = $2.type; }
                              | NOT expresion %prec HASH { $$.type = checkBooleanExp($2.type, $2.type); }
                              | expresion MASMENOS expresion { $$ = checkMasMenosExp($1, $3, $2); }//$$.nameTmp = temporal(); writeCType($$.type); normalWrite($$.nameTmp); normalWrite(";\n"); writeWithTabs($$.nameTmp); normalWrite(" = "); normalWrite($1.nameTmp); normalWrite(" "); normalWrite($2.lexema); normalWrite($3.nameTmp); normalWrite(";\n"); }
                              | expresion OPMULT expresion { $$ = checkOpMultExp($1, $3, $2); }
                              | expresion IGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); }
                              | expresion DESIGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); }
                              | expresion AND expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | expresion OR expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | expresion XOR expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | identificador { $$ = getTypeVar($1); $$.nameTmp = $1.lexema; }
                              | literal { $$ = $1; $$.nameTmp = $1.lexema; }
                              | funcion { $$.type = getTypeFunc($1); }
                              | HASH expresion { $$ = checkHashExp($2); }
                              | INTERR expresion %prec HASH { $$ = checkInterrExp($2); }
                              | expresion AT expresion { $$ = checkAtExp($1, $3); }
                              | expresion PLUSPLUS expresion AT expresion { $$= checkPlusPlusAtExp($1, $3, $5); }
                              | expresion MINMIN expresion { $$ = checkMinMinExp($1, $3); }
                              | expresion CONCAT expresion { $$ = checkConcatExp($1, $3); }
                              ;

funcion                       : identificador { findFunctionCall($1); }
                                ABRPAR argumentos CERPAR { endCallParameters(); }
                              ;

argumentos                    : expresion { checkCallParameters($1);}
                              | argumentos COMA expresion { checkCallParameters($3); }
                              ;

identificador                 : IDENTIF ;

finSentencia                  : PYC
                              | error
                              ;

lista                         : ABRCOR contenidoLista CERCOR { $$ = $2; $$.isList = 1; };

contenidoLista                : expresion { $$ = $1; }
                              | contenidoLista COMA expresion { $$ = checkLista($1, $3); }
                              ;

sentenciaLista                : identificador ITER finSentencia { if (getTypeVar($1).isList == 0) yyerror("[ERROR SEMÁNTICO]: Solo se puede iterar una lista."); }
                              | INITER identificador finSentencia { if (getTypeVar($1).isList == 0) yyerror("[ERROR SEMÁNTICO]: Solo se puede iterar una lista."); }
                              ;

literal                       : LITERAL { $$ = $1; }
                              | lista { $$ = $1; }
                              | cadena { $$.type = CARACTER; $$.isList = 1; }
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
