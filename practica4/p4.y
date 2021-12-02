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
#define MAX_SIZE_STRING 128
#define MAX_SIZE_FUNCTIONS 20

int HEADER = -1;
int HEADER_FUNC = -1;
uint IN_FUNC;

// Variables para el control de parámetros y tipo de las llamadas
typedef struct
{
  int currentFuncCall;  // Índice de la entrada de la función en TS
  uint nParamCheck;     // Parámetro a checkear en cada momento
  uint errorNParams;    // Bit para indicar num. incorrecto de parámetros
  uint errorTypeParams; // Bit para indicar tipo incorrecto de parámetros
} funcCheck;

funcCheck FT[MAX_SIZE_FUNCTIONS];

symbolTable TS[MAX_TS];

typedef struct 
{
  int atrib;
  char* lexema;
  dType type;
  int isList;
} attr;

#define YYSTYPE attr


// FUNCIONES ANALIZADOR

void blockStart();

void printTS_ALL();

void printTS(symbolTable in);

void pushTS(typeInput input, char* name, dType dataType, uint params, dType listDataType);

void popTS();

void pushAttr(attr atrib);

void assignType(attr p_dataType);

void checkBlockFunction();

void blockEnd();

attr getTypeVar(attr atrib);

dType getTypeFunc(attr atrib);

dType getTypeList(attr atrib);

dType getExpType(dType typ1, dType typ2);

void insertFunction(attr atrib);

void insertFormalParameter(attr atrib);

void findFunctionCall(attr atrib);

void checkCallParameters(attr atrib);

void endCallParameters();

int checkIfList(attr atrib);

attr checkAsignacion(attr, attr);

attr checkMasMenosExp(attr, attr b, attr op);

attr checkOpMultExp(attr a, attr b, attr op);

attr checkDesigualdadExp(attr a, attr b);

dType checkBooleanExp(dType a, dType b);

dType checkPlusPlusExp(attr a, attr b, attr c);

attr checkConcatExp(attr a, attr b);

char* getStr(dType t);

void checkBooleans(dType typ1);

void checkReturn(attr a);

attr checkLista(attr a, attr b);

/**
 * @brief Imprime los valores de la pila
 */
void printTS_ALL()
{
  if(HEADER > -1)
  {
    for (int i = 0; i <= HEADER; i++)
    {
      printf("[%d]input: %d name: %s dataType: %d params: %d listDataType: %d\n",i, TS[i].input, TS[i].name, TS[i].dataType, TS[i].params, TS[i].listDataType);
    }
  }
}

/**
 * @brief Muestra un elemento de la pila
 */
void printTS(symbolTable in)
{
      printf("input: %d name: %s dataType: %d params: %d listDataType: %d\n", in.input, in.name, in.dataType, in.params, in.listDataType);
}

/**
 * @brief Ingresa los valores a la pila de símbolos y actualiza el puntero hacia delante. 
 */
void pushTS(typeInput input, char* name, dType dataType, uint params, dType listDataType)
{
  if(HEADER != MAX_TS){
    HEADER++;
  }
  else{
    fprintf(stderr, "Se ha alcanzado el tope de la pila");
    printTS_ALL();
		exit(-1);
  }

  TS[HEADER].input = input;
  TS[HEADER].name = name;
  TS[HEADER].dataType = dataType;
  TS[HEADER].params = params;
  TS[HEADER].listDataType = listDataType;
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
  TS[HEADER].listDataType = SUS;
  
  HEADER--;
}

/**
 * @brief Insertar un identificador en la tabla de símbolos
 */
void pushAttr(attr atrib)
{
//  printf("Lexema leido: %s\n",atrib.lexema);

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
    pushTS(VARIABLE, atrib.lexema, NO_ASIGNADO, 0, 0);
  }
  else
  {
    char output[MAX_SIZE_STRING] = "[ERROR SEMÁNTICO], redefinición de variable \"";
    strcat(output,atrib.lexema);
    strcat(output,"\".");
    yyerror(output);
  }

}

/**
 * @brief Asigna el tipo de una variable
 */
void assignType(attr p_dataType)
{
  int seekHead = HEADER;
  while(TS[seekHead].input == VARIABLE && TS[seekHead].dataType == NO_ASIGNADO)
  {
    if(p_dataType.isList)
    {
      TS[seekHead].dataType = LISTA;
      TS[seekHead].listDataType = p_dataType.type;
    }
    else
    {
      TS[seekHead].dataType = p_dataType.type;
    }
      seekHead--;
  }

}

/**
 * @brief Función que realiza el chequeo de si el bloque está precedido de una declaración de función.
 * En ese caso, mete todos los parámetros formales de la función como variables locales después de la
 * marca de bloque.
 */
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
      char output[MAX_SIZE_STRING] = "[ERROR SEMÁNTICO], No se ha encontrado funcion \"";
      yyerror(output);
    }
    else{
      // Copiar todos los parámetros formales como variables locales del bloque
      for (int i = posFunction + 1; i < posNow; i++)
        pushTS(VARIABLE, TS[i].name, TS[i].dataType, -1 , TS[i].listDataType);
    }
    
  }
}

void blockStart()
{
  //printf("Inicio bloque detectado\n");
  pushTS(BLOCK_START, "BLOCK_START", -1, -1, 0);

  // Se chequea si se deben introducir los parámtros formales como variables variables locales
  checkBlockFunction();
}

void blockEnd()
{
 // printf("Fin bloque detectado\n");
 // printf("HEADER = %d\nTABLA SIMBOLOS HASTA AHORA:\n", HEADER);
 // printTS_ALL();
  while(TS[HEADER].input != BLOCK_START)
  {
    popTS();
  }
  popTS();
  //printf("Ahora HEADER = %d\n TABLA DE SIMBOLOS LUEGO:\n", HEADER);
  //printTS_ALL();

}

/**
 * @brief Irserción de una na entrada de tipo función en la tabla de símbolos.
 * El número de parámetros de la función se inicializa a 0.
 * @param atrib Atributo detectado.
 */
void insertFunction(attr atrib){
  if (atrib.isList)
    pushTS(FUNC, atrib.lexema, LISTA, 0, atrib.type);
  else
    pushTS(FUNC, atrib.lexema, atrib.type, 0, -1);
}

/**
 * @brief Inserción de un parámetro formal en la tabla de símbolos. Se incrementa
 * el número de parámetros de la función anterior.
 * @param atrib Atributo detectado.
 */
void insertFormalParameter(attr atrib){
  if (atrib.isList)
    pushTS(PARAMETER, atrib.lexema, LISTA, -1, atrib.type);
  else
    pushTS(PARAMETER, atrib.lexema, atrib.type, -1, -1);

  // Buscar función a la que hacen referencia los parámetros
  int posFunction = -1;
  int found = 0;

  for (int i = HEADER; i > 0 && !found; i--)
    if (TS[i].input == FUNC){
      found = 1;
      posFunction = i;
    }

  // Si no se ha encontrado función, error
  if (!found){
    char output[MAX_SIZE_STRING] = "[ERROR SEMÁNTICO], No se ha encontrado funcion \"";
    yyerror(output);
  }
  else{
    // Incrementar número de parámetros
    TS[posFunction].params += 1;
  }

}


/**
 * @brief Obtener el tipo de un identificador en una expresion.
 * @param atrib Atributo detectado por el analizador sintactico.
 * 
 */
attr getTypeVar(attr atrib)
{
  attr auxType;
  auxType.type = SUS;
  auxType.isList = 0;

  int found = 0;

  if (!IN_FUNC) {

    for(int i = HEADER; i > 0; i--)
    {
      if (strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == VARIABLE && TS[i].dataType == LISTA)
      {
        found = 1;
        auxType.type = TS[i].listDataType;
        auxType.isList = 1;
        break;
      }
      else if(strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == VARIABLE)
      {
        found = 1;
        auxType.type = TS[i].dataType;
        auxType.isList = 0;
        break;
      }
    }
  }
  else{

    // Buscar solo de la posición actual hasta la última marca de inicio de bloque
    for(int i = HEADER; i > 0 && TS[i].input != BLOCK_START && !found; i--)
    {
      if (strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == VARIABLE && TS[i].dataType == LISTA)
      {
        found = 1;
        auxType.type = TS[i].listDataType;
        auxType.isList = 1;
        break;
      }
      else if(strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == VARIABLE)
      {
        found = 1;
        auxType.type = TS[i].dataType;
        auxType.isList = 0;
        break;
      }
    }
  }

  // Si no encontrado error
  if(!found){
    char output[MAX_SIZE_STRING] = "";
    strcat(output, "[ERROR SEMÁNTICO] Variable \"");
    strcat(output,atrib.lexema);
    strcat(output,"\" no definida previamente.");
    yyerror(output);
  }

  return auxType;
}

/**
 * @brief   Obtener el tipo de una función en una expresion.
 * @param   atrib Atributo detectado por el analizador sintáctico
 * @return  
 */
dType getTypeFunc(attr atrib) {
  dType auxType = SUS;
  int found = 0;

  for(int i = HEADER; i > 0; i--)
  {
  	if(strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == FUNC)
  	{
      found = 1;
      auxType = TS[i].dataType;
      break;
  	}
  }

	if(!found)
	{
		char output[MAX_SIZE_STRING] = "";
		strcat(output, "[ERROR SEMÁNTICO] Variable \"");
		strcat(output,atrib.lexema);
		strcat(output,"\"no definida previamente.");
		yyerror(output);
	}
  

  return auxType;
}


/**
 * 
 */
dType getExpType(dType typ1, dType typ2)
{
  dType aux = typ1;
  if(typ1 != typ2)
  {
    aux = SUS;
  }

  return aux;
}

dType checkValidOp(dType typ1, dType typ2, char* err)
{
    dType res = typ1;
    if (typ1 != typ2) {
        res = SUS;
        yyerror(err);
    }
    return res;
}

dType getTypeList(attr atrib)
{
  dType auxType = SUS;
  int found = 0;

	for(int i = HEADER; i > 0; i--)
	{
		if(strcmp(atrib.lexema, TS[i].name) == 0)
		{
      found = 1;
      auxType = TS[i].listDataType;
      break;
		}
	}

  return auxType;
}

attr checkHashExp(attr a)
{
  attr retVal;

  retVal.type = SUS;
  retVal.isList = 0; 
  
  if(a.isList)
  {
    retVal.type = ENTERO;
  }
  else
  {
    yyerror("[ERROR SEMÁNTICO] Elemento no es de tipo lista.");
  }
  return retVal;
}

attr checkInterrExp(attr a)
{
  attr retVal;

  retVal.type = SUS;
  retVal.isList = 0;

  if(a.isList)
  {
    retVal.type = a.type;
  }
  else
  {
    yyerror("[ERROR SEMÁNTICO] Elemento no es de tipo lista.");
  }

  return retVal;
}

attr checkAtExp(attr a, attr b) {
  attr retVal;
  retVal.type = SUS;

  if (a.isList == 1) {
    if (b.type == ENTERO && b.isList == 0) {
      retVal.type = a.type;
      retVal.isList = 0;
    } else {
      char msg[100] = "[ERROR SEMÁNTICO] Se esperaba una variable de tipo ENTERO";
      yyerror(msg);
    }
  } else {
    char msg[100] = "[ERROR SEMÁNTICO] Se esperaba una lista";
    yyerror(msg);
  }

  return retVal;
}

attr checkAsignacion(attr a, attr b)
{
  attr t;
  t = getTypeVar(a);

  if (t.type != b.type || t.isList != b.isList) {
    if (t.isList == 0) {
      char msg[100] = "[ERROR SEMÁNTICO] Se esperaba una variable de tipo ";
      strcat(msg, getStr(t.type));
      yyerror(msg);
    } else {
      char msg[100] = "[ERROR SEMÁNTICO] Se esperaba una variable de tipo LISTA DE ";
      strcat(msg, getStr(t.type));
      yyerror(msg);
    }
  }

  return t;
}

void checkReturn(attr a){

  // Buscar marca de inicio  de bloque
  int markFound = 0;
  int posMark = -1;

  for(int i = HEADER; i > 0 && !markFound; i--)
    if(TS[i].input == BLOCK_START){
      markFound = 1;
      posMark = i;
    }
  
  // Buscar función
  int found = 0;
  int entryFunc = -1;

  for(int i = posMark - 1; i > 0 && !found; i--)
    if(TS[i].input == FUNC){
      found = 1;
      entryFunc = i;
    }
  
  if(found && (TS[entryFunc].dataType != a.type || (a.isList && TS[entryFunc].dataType == LISTA && a.type == TS[entryFunc].listDataType))){
    yyerror("Estas intentado devolver una variable de un tipo distinto al esperado");
  }
  else if(!found){
    yyerror("Este repatriar no esta asociado a una funcion");
  }

  
}

attr checkMasMenosExp(attr a, attr b, attr op)
{
  attr res;
  res.type = SUS;
  res.isList = 0;
  
  if (a.isList == 1 && b.isList == 0) {
    if (a.type == b.type) {
      if (a.type == ENTERO || a.type == REAL) {
        res.type = a.type;
        res.isList = 1;
      } else {
        char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
        strcat(msg, getStr(a.type));
        yyerror(msg);
      }
    } else {
      char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
      yyerror(msg);
    }
  } else if (a.isList == 0 && b.isList == 1) {
    if (a.type == b.type) {
      if (a.type == ENTERO || a.type == REAL) {
        res.type = a.type;
        res.isList = 0;
      } else {
        char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
        strcat(msg, getStr(a.type));
        yyerror(msg);
      }
    } else {
      char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
      yyerror(msg);
    }
  } else if (a.isList == 0 && b.isList == 0) {
    if (a.type == b.type) {
      if (a.type == ENTERO || a.type == REAL) {
        res.type = a.type;
        res.isList = 0;
      } else {
        char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
        strcat(msg, getStr(a.type));
        yyerror(msg);
      }
    } else {
      char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
      yyerror(msg);
    }
  } else {
    yyerror("[ERROR SEMÁNTICO] Ambos valores no pueden ser listas");
  }

  return res;
}

attr checkOpMultExp(attr a, attr b, attr op)
{
  attr res;
  res.type = SUS;
  res.isList = 0;

  //printf("valores: %d - %s - %d - %d\n", a.atrib, a.lexema, a.type, a.isList);
  //printf("porfa: %d\n", getTypeList(a));
  //printf("lmao: %d\n", op.atrib);
  switch (op.atrib)
  {
    case 0: // Operador /
      if (a.isList && b.isList == 0) {
        if (a.type == b.type) {
          if (a.type == ENTERO || a.type == REAL) {
            res.type = a.type;
            res.isList = 1;
          } else {
            char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
            strcat(msg, getStr(a.type));
            yyerror(msg);
          }
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
          yyerror(msg);
        }
      } else if (a.isList == 0 && b.isList == 0) {
        if (a.type == b.type) {
          if (a.type == ENTERO || a.type == REAL) {
            res.type = a.type;
            res.isList = 0;
          } else {
            char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
            strcat(msg, getStr(a.type));
            yyerror(msg);
          }
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
          yyerror(msg);
        }
      } else {
        yyerror("[ERROR SEMÁNTICO] El segundo atributo no puede ser una lista");
      }
    break;
    case 2: // Operador *
      if (a.isList == 1 && b.isList == 0) {
        if (a.type == b.type) {
          if (a.type == ENTERO || a.type == REAL) {
            res.type = a.type;
            res.isList = 1;
          } else {
            char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
            strcat(msg, getStr(a.type));
            yyerror(msg);
          }
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
          yyerror(msg);
        }
      } else if (a.isList == 0 && b.isList == 1) {
        if (a.type == b.type) {
          if (a.type == ENTERO || a.type == REAL) {
            res.type = a.type;
            res.isList = 0;
          } else {
            char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
            strcat(msg, getStr(a.type));
            yyerror(msg);
          }
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
          yyerror(msg);
        }
      } else if (a.isList == 0 && b.isList == 0) {
        if (a.type == b.type) {
          if (a.type == ENTERO || a.type == REAL) {
            res.type = a.type;
            res.isList = 0;
          } else {
            char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
            strcat(msg, getStr(a.type));
            yyerror(msg);
          }
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No coinciden los tipos ";
          yyerror(msg);
        }
      } else {
        yyerror("[ERROR SEMÁNTICO] Ambos valores no pueden ser listas");
      }
    break;
    case 3: // Operador %
      if (a.isList == 1 && b.isList == 0) {
        if (b.type == ENTERO) {
          res.type = a.type;
          res.isList = 1;
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
          strcat(msg, getStr(a.type));
          yyerror(msg);
        }
      } else if (a.isList == 0 && b.isList == 0) {
        if (a.type == ENTERO && b.type == ENTERO) {
          res.type = ENTERO;
          res.isList = 1;
        } else {
          char msg[50] = "[ERROR SEMÁNTICO] No se esperaba ";
          strcat(msg, getStr(a.type));
          yyerror(msg);
        }
      } else {
        char msg[50] = "[ERROR SEMÁNTICO] No se esperaba LISTA DE ";
        strcat(msg, getStr(b.type));
        yyerror(msg);
      }
    break;
  }
  return res;
}

attr checkDesigualdadExp(attr a, attr b)
{
  attr retVal;
  retVal.type = SUS;
  retVal.isList = 0;
  if (a.isList != 1)
  {
    if (a.type == b.type) {
      retVal.type = BOOLEANO;
    }
    else
    {
      char msg[50] = "[ERROR SEMÁNTICO] Esperado ";
      strcat(msg, getStr(a.type));
      yyerror(msg);
    }
  }
  else
  {
    yyerror("[ERROR SEMÁNTICO] No se esperaba LISTA");
  }
  return retVal;
}

dType checkBooleanExp(dType a, dType b)
{
  dType retVal = SUS;
  if (a == BOOLEANO) {
    if (b == BOOLEANO) {
      retVal = BOOLEANO;
    }
    else
    {
      yyerror("[ERROR SEMÁNTICO] Esperado BOOLEANO.");
    }
  }
  else
  {
      yyerror("[ERROR SEMÁNTICO] Esperado BOOLEANO.");
  }
  return retVal;
}

attr checkPlusPlusAtExp(attr a, attr b, attr c)
{
  attr retVal;
  
  retVal.type = SUS;
  retVal.isList = 0;
  
  if(a.isList)
  {
    if(a.type == b.type)
    {
      if(getExpType(c.type, ENTERO))
      {
        retVal.type = b.type;
        retVal.isList = 1;
      }
      else
      {
        yyerror("[ERROR SEMÁNTICO] Tercer elemento no es de tipo entero.");
      }
    }
    else
    {
      yyerror("[ERROR SEMÁNTICO] Segundo elemento no coincide con el tipo de lista.");
    }
  }
  else
  {
    yyerror("[ERROR SEMÁNTICO] Primer elemento no es tipo lista.");
  }

  return retVal;
}

attr checkMinMinExp(attr a, attr b)
{
  attr res; 
  res.isList = 0;
  res.type = SUS;

  if(a.isList)
  {
    if(getExpType(b.type, ENTERO))
    {
      res.type = a.type;
      res.isList = 1;
    }
    else
    {
      yyerror("[ERROR SEMÁNTICO] Segundo elemento no es tipo entero.");
    }
  }
  else
  {
    yyerror("[ERROR SEMÁNTICO] Primer elemento no es tipo lista.");
  }
  
  return res;
}

attr checkConcatExp(attr a, attr b)
{
  attr res;
  
  res.isList = 0; 
  res.type = SUS;
  
  if (a.isList && b.isList)
  {
    if (a.type == b.type)
    {
      res.type = a.type;
      res.isList = a.isList;
    }
    else
    {
      yyerror("[ERROR SEMÁNTICO] Concatenacion entre listas de tipos diferentes.");
    }

  }
  else
  {
    char output[MAX_SIZE_STRING] = "";
    strcat(output, "[ERROR SEMÁNTICO] Tipo inesperado en concatenación");
    yyerror(output);
  }
  return res;
}

/**
 * @brief   Función que checkea si una función existe al ser llamada.
 *          Si se encuentra, marca su posición en la pila en la variable CURRENT_FUNC_CALL.
 * @param   atrib Atributo detectado por el analizador sintáctico.
 */
void findFunctionCall(attr atrib){
  // Se incrementa contador de la pila de funciones e inicializacion
  HEADER_FUNC += 1;
  FT[HEADER_FUNC].currentFuncCall = -1;
  FT[HEADER_FUNC].nParamCheck = 0;
  FT[HEADER_FUNC].errorNParams = 0;
  FT[HEADER_FUNC].errorTypeParams = 0;
  
  // Se busca en toda la tabla de símbolos la función
  int found = 0;

  for (int i = 0; i <= HEADER && !found; i++)
    if(strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == FUNC){
      found = 1;
      FT[HEADER_FUNC].currentFuncCall = i;
    }

  // Si no se encuentra, error
  if (found)
    FT[HEADER_FUNC].nParamCheck = 1;  // Actualizar el parámetro a checkear
}

/**
 * @brief   Función que checkea los tipos y el número de parámetros de una función.
 *          Se va realizando la comparación desde el último parámetro al primero.
 * @param   atrib Atributo detectado por el analizador sintáctico.
 */
 void checkCallParameters(attr atrib){
  // Si se ha encontrado la función
  if (FT[HEADER_FUNC].currentFuncCall != -1){

    // Se comprueba si el número de parámetros es correcto
    if(FT[HEADER_FUNC].nParamCheck == TS[FT[HEADER_FUNC].currentFuncCall].params + 1){

      FT[HEADER_FUNC].errorNParams = 1;     // Num. incorrecto de parámetros
      FT[HEADER_FUNC].currentFuncCall = -1; // Funcion no definida con esos parámetros
    }
    else{

      // Comprobar tipos
      int indParam = FT[HEADER_FUNC].currentFuncCall + FT[HEADER_FUNC].nParamCheck;

      if (atrib.type == TS[indParam].dataType || 
          (atrib.isList && atrib.type == TS[indParam].listDataType && TS[indParam].dataType == LISTA)){
        // Actualizar parametro a checkear
        FT[HEADER_FUNC].nParamCheck += 1;
      }
      else{
        FT[HEADER_FUNC].errorTypeParams = 1;
        FT[HEADER_FUNC].nParamCheck += 1;
      }
    }
   }
 }

/**
 * @brief   Función que checkea el número de parámetros de una función cuando se han acabado los parámetros.
 * @param   atrib Atributo detectado por el analizador sintáctico.
 */
void endCallParameters(){
  // Si quedan parámetros por checkear, error
  int lastParams = FT[HEADER_FUNC].nParamCheck;

  if(lastParams != TS[FT[HEADER_FUNC].currentFuncCall].params + 1 && FT[HEADER_FUNC].currentFuncCall != -1)
    FT[HEADER_FUNC].errorNParams = 1; // Num. incorrecto de parámetros

  // Mostrar salida de error semántico en comprobación de parámetros
  if(FT[HEADER_FUNC].errorNParams){
    char output[MAX_SIZE_STRING];
    strcat(output, "Numero incorrecto de parametros");
    yyerror(output);
  }
  else if (FT[HEADER_FUNC].errorTypeParams){
    char output[MAX_SIZE_STRING];
    strcat(output, "Tipo incorrecto de parametros");
    yyerror(output);
  }
  
  // Actualizar tope de comprobaciones
  HEADER_FUNC -= 1;
}

/**
 * @brief   Función simple, que espera recibir un booleano en caso contrario muestra por pantalla un error
 * @param   atrib Atributo detectado por yacc.
 */
void checkBooleans(dType typ1)
{
  if(BOOLEANO != typ1){
    yyerror("Se esperaba un booleano");
  }
}

attr checkLista(attr a, attr b) {
  attr res;
  res.type = SUS;
  res.isList = 0;

  if (a.type == b.type) {
    res.type = a.type;
  } else {
    char msg[50] = "Se esperaba una variable de tipo ";
    strcat(msg, getStr(a.type));
    yyerror(msg);
  }

  return res;
}

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

programa                      : cabeceraPrograma bloque;

cabeceraPrograma              : INIPROG;

bloque                        : inicioBloque declararVariablesLocalesMulti
                                declararFuncionMulti sentencias finBloque ; 

declararFuncionMulti          : declararFuncionMulti declararFuncion
                              | /* cadena vacía */
                              ;

declararFuncion               : cabeceraFuncion {IN_FUNC = 1;}
                                bloque {IN_FUNC = 0;}
                                ;

declararVariablesLocalesMulti : marcaInicioVariable variablesLocalesMulti
                                marcaFinVar
                              | /* cadena vacía*/
                              ;

cabeceraFuncion               : tipoDato identificador {$2.type = $1.type; insertFunction($2);}
                                inicioParametros
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

variableLocal                 : tipoDato variableSolitaria identificador finSentencia { pushAttr($3); assignType($1); }
                              | error;

variableSolitaria             : variableSolitaria identificador coma { pushAttr($2); }
                              | /* cadena vacía*/
                              ;

coma                          : COMA
                              | error
                              ;

tipoDato                      : tipoElemental { $$.type = $1.type; $$.isList = 0; }
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
                              | expresion MASMENOS expresion { $$ = checkMasMenosExp($1, $3, $2); }
                              | expresion OPMULT expresion { $$ = checkOpMultExp($1, $3, $2); }
                              | expresion IGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); }
                              | expresion DESIGUALDAD expresion { $$ = checkDesigualdadExp($1, $3); }
                              | expresion AND expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | expresion OR expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | expresion XOR expresion { $$.type = checkBooleanExp($1.type, $3.type); }
                              | identificador { $$ = getTypeVar($1); }
                              | literal { $$ = $1; }
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
