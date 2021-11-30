%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Incluyendo la tabla de símbolos
#include "symbolTable.h"

// Activar modo debug.
//int yydebug=1; 
void yyerror (const char * msg);
int yylex(void);


/* ANALIZADOR SEMANTICO */

#define MAX_TS 500

int HEADER = -1;
uint IN_FUNC;

dType tempType;

symbolTable TS[MAX_TS];

typedef struct 
{
  int atrib;
  char* lexema;
  dType type;
} attr;

#define YYSTYPE attr


// FUNCIONES ANALIZADOR


void printTS_ALL()
{
  if(HEADER > -1)
  {
    for (int i = 0; i <= HEADER; i++)
    {
      printf("[%d]\tInput: %d\tName: %s\tData type: %d\n",i, TS[i].input, TS[i].name, TS[i].dataType);
    }
  }
}

void printTS(symbolTable in)
{
  printf("[Input: %d | Name: %s | Data type: %d]\n", in.input, in.name, in.dataType);
}

/**
 * @brief Ingresa los valores a la pila de símbolos y actualiza el puntero hacia delante. 
 */
void pushTS(typeInput input, char* name, dType dataType, uint params, uint dimens, int tamDimen)
{
  HEADER++;

  TS[HEADER].input = input;
  TS[HEADER].name = name;
  TS[HEADER].dataType = dataType;
  TS[HEADER].params = params;
  TS[HEADER].dimens = dimens;
  TS[HEADER].tamDimen = tamDimen;

}

/**
 * @brief Anula los valores actuales del tope de la pila y reduce el puntero de la pila.
 */
void popTS()
{
  TS[HEADER].input = EMPTY;
  TS[HEADER].name = NULL;
  TS[HEADER].dataType = SUS;
  TS[HEADER].params = -1;
  TS[HEADER].dimens = -1;
  TS[HEADER].tamDimen = -1;
  
  HEADER--;
}

/**
 * @brief Insertar un identificador en la tabla de símbolos
 * 
 */
void pushAttr(attr atrib)
{
  printf("Lexema leido: %s\n",atrib.lexema);

  char* varName = atrib.lexema;
  int found = 0;
  
  /*
   * - Se comienza a buscar desde el valor actual del tope de la pila hacia atras
   *   hasta el inicio, puesto que no deben haber tampoco variables repetidas en los
   *   superbloques.
   * - Si no hay coincidencias entonces es la unica.
   */
  
  for(int i = HEADER; i > 0; i--)
  {
    if(strcmp(varName, TS[i].name) == 0)
    {
      found = 1;
      break;
    }
  }

  if(!found)
  {
    pushTS(VARIABLE, atrib.lexema, NO_ASIGNADO, 0, 0, 0);
  }
  else
  {
    char output[] = "[ERROR SEMÁNTICO], redefinición de variable \"";
    strcat(output,atrib.lexema);
    strcat(output,"\".");
    yyerror(output);
  }

}

void assignType(dType p_dataType)
{
  int seekHead = HEADER;
  while(TS[seekHead].input == VARIABLE && TS[seekHead].dataType == NO_ASIGNADO)
  {
    TS[seekHead].dataType = p_dataType;
    seekHead--;
  }

}

void blockStart()
{
  printf("Inicio bloque detectado\n");
  pushTS(BLOCK_START, "BLOCK_START", -1, -1, -1, -1);
}

void blockEnd()
{
  printf("Fin bloque detectado, HEADER = %d\n", HEADER);
  printTS_ALL();
  while(TS[HEADER].input != BLOCK_START)
  {
    popTS();
  }
  popTS();
  printf("Ahora HEADER = %d\n", HEADER);
  printTS_ALL();

}

%}

/*INICIO YACC*/

%define parse.error verbose

%start programa

/* Tabla de tokens */ 
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

inicioBloque                  : ABRLLA { blockStart(); }
                              ;  

finBloque                     : CERLLA { blockEnd(); };

variablesLocalesMulti         : variablesLocalesMulti variableLocal
                              | /* cadena vacía */
                              ;

variableLocal                 : tipoDato variableSolitaria identificador finSentencia { pushAttr($3); assignType($1.type); }
                              | error;

variableSolitaria             : variableSolitaria identificador coma { pushAttr($2); }
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
