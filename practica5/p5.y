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

programa                      : cabeceraPrograma { globalVars = 1; } bloque { $$.gen = concatGen($1.gen, $3.gen); $$.funcGen = $3.funcGen; writeProgram($$); closeFile();}
                              ; 

cabeceraPrograma              : INIPROG { openFile(); $$.gen = genHeaders(); };

bloque                        : inicioBloque declararVariablesLocalesMulti
                                declararFuncionMulti sentencias finBloque {$$ = getCorrectBlock($1, $2, $3, $4, $5, IN_FUNC);}; 

declararFuncionMulti          : declararFuncionMulti declararFuncion {$$.gen = concatGen($1.gen, $2.gen);}
                              | {$$.gen = "";}/* cadena vacía */
                              ;

declararFuncion               : { controlStartBlockFunc();} cabeceraFuncion {IN_FUNC += 1; }
                                bloque {IN_FUNC -= 1; $$.gen = concatGen($2.gen, $4.gen); controlEndBlockFunc();}
                                ;

declararVariablesLocalesMulti : marcaInicioVariable variablesLocalesMulti
                                marcaFinVar { $$.gen = concatGen($2.gen, controlGlobalVars()); $$.funcGen = $2.funcGen;}
                              | {$$.gen = concatGen("", controlGlobalVars()); $$.funcGen = "";} /* cadena vacía*/
                              ;

cabeceraFuncion               : tipoDato identificador {$2.type = $1.type; $2.isList = $1.isList; insertFunction($2); }
                                inicioParametros
                                parametros finParametros {$$.gen = getCabecera($1, $2, $5);};

parametros                    : tipoDato identificador {$2.type = $1.type; insertFormalParameter($2); $$.gen = getParamDec($1, $2);}
                              | parametros COMA tipoDato identificador {$4.type = $3.type; insertFormalParameter($4); $$.gen = concatParamDec($1, $3, $4);}
                              | error
                              | {$$.gen = "";} /* cadena vacía */
                              ;

inicioParametros              : ABRPAR;

finParametros                 : CERPAR;

marcaInicioVariable           : INIVAR;

marcaFinVar                   : FINVAR finSentencia;

inicioBloque                  : ABRLLA { blockStart(); $$.gen = getStartBlock();}
                              ;  

finBloque                     : CERLLA { blockEnd(); $$.gen = getEndBlock();}
                              ;

variablesLocalesMulti         : variablesLocalesMulti variableLocal {$$.gen = concatGen($1.gen, $2.gen); $$.funcGen = concatGen($1.funcGen, $2.funcGen);}
                              | {$$.gen = ""; $$.funcGen = "";} /* cadena vacía */
                              ;

variableLocal                 : tipoDato variableSolitaria identificador finSentencia { pushAttr($3); assignType($1); $$.gen = getVars($1, $2, $3); $$.funcGen = getExternVars($1, $2, $3);} 
                              | error;

variableSolitaria             : variableSolitaria identificador coma { pushAttr($2); $$.nameTmp = concatVars($1, $2); }
                              | { $$.nameTmp = ""; } /* cadena vacía*/
                              ;

coma                          : COMA
                              | error
                              ;

tipoDato                      : tipoElemental { $$.type = $1.type; $$.isList = 0; $$.nameTmp = getCType($1); }
                              | DEFLISTA tipoElemental { $$.type = $2.type; $$.isList = 1; $$.nameTmp = getCType($$);}
                              ;

tipoElemental                 : TIPODATO;

sentencias                    : sentencias sentencia {$$.gen = concatGen($1.gen, $2.gen);}
                              | sentencia {$$.gen = $1.gen;}
                              ;

sentencia                     : bloque {$$.gen = $1.gen;}
                              | sentenciaAsignacion {$$.gen = $1.gen;}
                              | sentenciaIf {$$.gen = $1.gen;}
                              | sentenciaWhile {$$.gen = $1.gen;}
                              | sentenciaEntrada {$$.gen = $1.gen;}
                              | sentenciaSalida {$$.gen = $1.gen;}
                              | sentenciaReturn {$$.gen = $1.gen;}
                              | sentenciaFor {$$.gen = $1.gen;}
                              | sentenciaLista {$$.gen = $1.gen;}
                              ;

sentenciaAsignacion           : identificador ASIG expresion finSentencia { $$ = checkAsignacion($1, $3); $$.gen = getAsig($1, $3); $$.nameTmp = $1.lexema;}
                              | error
                              ;

sentenciaIf                   : IF inicioParametros expresion finParametros
                                sentencia {checkBooleans($3.type); $$.gen = getIf($3, $5);}
                              | IF inicioParametros expresion finParametros
                                sentencia ELSE sentencia {checkBooleans($3.type); $$.gen = getIfElse($3, $5, $7);}
                              ;

sentenciaWhile                : WHILE inicioParametros expresion finParametros
                                sentencia {checkBooleans($3.type); $$.gen = getWhile($3, $5);} ; 

sentenciaEntrada              : nombreEntrada listaVariablesEntrada finSentencia {$$.gen = $2.gen;};

listaVariablesEntrada         : inicioParametros listaIdentificadores finParametros {$$.gen = $2.gen;};

listaIdentificadores          : identificador {$1 = getTypeVar($1); $$.gen = getScan($1);}
                              | listaIdentificadores COMA identificador {$3 = getTypeVar($3); $$.gen = concatGen($1.gen, getScan($3));}
                              | error
                              | {$$.gen = "";} /* cadena vacía */
                              ;

nombreEntrada                 : SCAN;

sentenciaReturn               : RETURN expresion finSentencia{checkReturn($2); $$.gen = getReturn($2);};

sentenciaFor                  : FOR sentenciaAsignacion TO expresion sentido
                                sentencia {if ($2.type != getExpType(ENTERO, $4.type)) yyerror("Tipos distintos en bucle for"); $$.gen = getForAsig($2, $4, $5, $6);}
                              | FOR expresion TO expresion sentido sentencia {if (ENTERO != getExpType($2.type, $4.type)) yyerror("Tipos distintos en bucle for"); $$.gen = getFor($2, $4, $5, $6);}
                              ;

sentido                       : SENTIDO;

sentenciaSalida               : nombreSalida inicioParametros
                                listaExpresionesCadena finParametros
                                finSentencia {$$.gen = $3.gen;};
                              ;

listaExpresionesCadena        : expresion {checkPrint($1); $$.gen = getPrint($1);}  
                              | listaExpresionesCadena COMA expresion {checkPrint($3); $$.gen = concatGen($1.gen, getPrint($3));}
                              ;

nombreSalida                  : PRINT;

expresion                     : ABRPAR expresion CERPAR { $$ = $2; $$.nameTmp = $2.nameTmp; $$.gen = $2.gen; }
                              | MASMENOS expresion %prec HASH { $$.type = $2.type; $$.nameTmp = temporal(); $$.gen = $2.gen; $$.gen = concatGen($$.gen, getMasMenosSufExpr($$, $1, $2)); }
                              | NOT expresion %prec HASH { $$.type = checkBooleanExp($2.type, $2.type); $$.nameTmp = temporal(); $$.gen = concatGen($2.gen, getNotExpr($$, $1, $2)); }
                              | expresion MASMENOS expresion { $$ = checkMasMenosExp($1, $3, $2); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getMasMenosExpr($$, $1, $2, $3))); }
                              | expresion OPMULT expresion { $$ = checkOpMultExp($1, $3, $2); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getOpMultExpr($$, $1, $2, $3))); }
                              | expresion IGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getIgualdadExpr($$, $1, $2, $3))); }
                              | expresion DESIGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getDesigualExpr($$, $1, $2, $3))); }
                              | expresion AND expresion { $$.type = checkBooleanExp($1.type, $3.type); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getAndExpr($$, $1, $2, $3))); }
                              | expresion OR expresion { $$.type = checkBooleanExp($1.type, $3.type); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getOrExpr($$, $1, $2, $3))); }
                              | expresion XOR expresion { $$.type = checkBooleanExp($1.type, $3.type); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getXorExpr($$, $1, $2, $3))); }
                              | identificador { $$ = getTypeVar($1); $$.nameTmp = $1.lexema; $$.gen = ""; }
                              | literal { $$ = $1; $$.nameTmp = equivalentCLexema($1); $$.gen = ""; }
                              | funcion { $$ = getTypeFunc($1); $$.nameTmp = $1.nameTmp; $$.gen = $1.gen; }
                              | HASH expresion { $$ = checkHashExp($2); $$.nameTmp = temporal(); $$.gen = concatGen($2.gen, getHashExpr($$, $1, $2));}
                              | INTERR expresion %prec HASH { $$ = checkInterrExp($2); $$.nameTmp = temporal(); $$.gen = concatGen($2.gen, getInterrExpr($$, $1, $2));}
                              | expresion AT expresion { $$ = checkAtExp($1, $3);$$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getAtExpr($$, $1, $2, $3))); }
                              | expresion PLUSPLUS expresion AT expresion { $$= checkPlusPlusAtExp($1, $3, $5); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, concatGen($5.gen, getTernExpr($$, $1, $3, $5))));}
                              | expresion MINMIN expresion { $$ = checkMinMinExp($1, $3); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getMinMinExpr($$, $1, $3)));}
                              | expresion CONCAT expresion { $$ = checkConcatExp($1, $3); $$.nameTmp = temporal(); $$.gen = concatGen($1.gen, concatGen($3.gen, getConcatExpr($$, $1, $3)));}
                              ;

funcion                       : identificador { findFunctionCall($1); numTabs += 1;}
                                ABRPAR argumentos CERPAR { endCallParameters(); $$.nameTmp = getFuncCall($1, $4); $$.gen = $4.gen; numTabs -= 1;}
                              ;

argumentos                    : expresion { checkCallParameters($1); $1 = getParamFunc($1); $$.nameTmp = $1.nameTmp; $$.gen = $1.gen;}
                              | argumentos COMA expresion { checkCallParameters($3); $3 = getParamFunc($3); $$.nameTmp = paramConcat($1, $3); $$.gen = concatGen($1.gen, $3.gen);}
                              |
                              ;

identificador                 : IDENTIF ;

finSentencia                  : PYC
                              | error
                              ;

lista                         : ABRCOR contenidoLista CERCOR { $$ = $2; $$.isList = 1; };

contenidoLista                : expresion { $$ = $1; }
                              | contenidoLista COMA expresion { $$ = checkLista($1, $3); }
                              ;

sentenciaLista                : identificador ITER finSentencia { if (getTypeVar($1).isList != 0) yyerror("[ERROR SEMÁNTICO]: Solo se puede iterar una lista."); $1 = getTypeVar($1); $$.gen = getSentIter($1, $2);}
                              | INITER identificador finSentencia { if (getTypeVar($2).isList != 0) yyerror("[ERROR SEMÁNTICO]: Solo se puede iterar una lista."); $2 = getTypeVar($2); $$.gen = getIniIter($2);}
                              ;

literal                       : LITERAL { $$ = $1; }
                              | lista { $$ = $1; }
                              | cadena { $$.type = CARACTER; $$.isList = 2; }
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
