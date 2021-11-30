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

symbolTable TS[MAX_TS];

typedef struct 
{
  int atrib;
  char* lexema;
  dType type;
} attr;

#define YYSTYPE attr


// FUNCIONES ANALIZADOR

void blockStart();

void printTS_ALL();

void printTS(symbolTable in);

void pushTS(typeInput input, char* name, dType dataType, uint params, uint dimens, int tamDimen);

void popTS();

void pushAttr(attr atrib);

void assignType(dType p_dataType);

void checkBlockFunction();

void blockEnd();

dType getTypeVar(attr atrib);

//void checkBooleans();

/**
 * @brief Imprime los valores de la pila
 */
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

/**
 * @brief Muestra un elemento de la pila
 */
void printTS(symbolTable in)
{
  printf("[Input: %d | Name: %s | Data type: %d]\n", in.input, in.name, in.dataType);
}

/**
 * @brief Ingresa los valores a la pila de símbolos y actualiza el puntero hacia delante. 
 */
void pushTS(typeInput input, char* name, dType dataType, uint params, uint dimens, int tamDimen)
{
  if(HEADER == MAX_TS){
    HEADER++;
  }
  else{
    printf(stderr, "Se ha alcanzado el tope de la pila");
    printTS_ALL();
		exit(-1);
  }

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
  
  for(int i = HEADER; i > 0 && TS[i].input != BLOCK_START; i--) // Buscar en el bloque actual
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

/**
 * @brief Asigna el tipo de una variable
 */
void assignType(dType p_dataType)
{
  int seekHead = HEADER;
  while(TS[seekHead].input == VARIABLE && TS[seekHead].dataType == NO_ASIGNADO)
  {
    TS[seekHead].dataType = p_dataType;
    seekHead--;
  }

}

// Función que realiza el chequeo de si el bloque está precedido de una declaración de función.
// En ese caso, mete todos los parámetros formales de la función como variables locales después
// de la marca de bloque.

void checkBlockFunction(){

  // Se comprueba el bit de declaración de función
  if (IN_FUNC == 1){

    // Buscar función justo antes de inicio de bloque
    int posNow = HEADER;
    int posFunction = -1;
    int found = 0;

    for(int i = HEADER; i > 0 && !found; i--)
      if(TS[i].input == FUNC){
        found = 1;
        posFunction = i;
      }

    // Si no hay función, error
    if(!found){
      char output[] = "[ERROR SEMÁNTICO], No se ha encontrado funcion \"";
      strcat(output,"\".");
      yyerror(output);
    }
    else{
      // Copiar todos los parámetros formales como variables locales del bloque
      for (int i = posFunction + 1; i < posNow; i++)
        pushTS(VARIABLE, TS[i].name, TS[i].dataType, -1 , -1, -1);
    }
    
  }
}

void blockStart()
{
  printf("Inicio bloque detectado\n");
  pushTS(BLOCK_START, "BLOCK_START", -1, -1, -1, -1);

  // Se chequea si se deben introducir los parámtros formales como variables variables locales
  checkBlockFunction();
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

// Función que mete una entrada de tipo función en la tabla de símbolos
void insertFunction(attr atrib){
  pushTS(FUNC, atrib.lexema, atrib.type, -1, -1, -1);
}

// Función que mete una entrada de tipo parámetro formal en la tabla de símbolos
void insertFormalParameter(attr atrib){
  pushTS(PARAMETER, atrib.lexema, atrib.type, -1, -1, -1);
}

/**
 * @brief Obtener el tipo de un identificador en una expresion.
 * @param atrib Atributo detectado por el 
 */
dType getTypeVar(attr atrib)
{
  dType auxType = SUS;
  int found = 0;

  for(int i = HEADER; i > 0; i--)
  {
    if(strcmp(atrib.lexema, TS[i].name) == 0)
    {
      found = 1;
      auxType = TS[i].dataType;
      break;
    }
  }

  if(!found)
  {
    char output[] = "[ERROR SEMÁNTICO], Variable \"";
    strcat(output,atrib.lexema);
    strcat(output,"\" no definida previamente.");
    yyerror(output);
  }

  return auxType;
}


// Función que mete un deez

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
                                declararFuncionMulti sentencias finBloque ; 

declararFuncionMulti          : declararFuncionMulti declararFuncion
                              | /* cadena vacía */
                              ;

declararFuncion               : cabeceraFuncion {IN_FUNC = 1;}
                                bloque  {IN_FUNC = 0;}
                                ;

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
                              | expresion AND expresion {
                                    if ($1.type == BOOLEANO && $1.type == BOOLEANO) {
                                        $$.type = $1.type;
                                    } else {
                                        fprintf(stderr, "Error en el AND\n");
                                    }
                                }
                              | expresion OR expresion { if ($1.type == $2.type && $1.type == BOOLEANO) { fprintf(stderr, "not pog\n"); } }
                              | expresion XOR expresion { if ($1.type == $2.type && $1.type == BOOLEANO) { fprintf(stderr, "Error en el XOR\n"); } }
                              | identificador { $$.type = getTypeVar($1) }
                              | literal { $$.type = $1.type; }
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
