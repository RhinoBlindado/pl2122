#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/semantic.h"
#include "symbolTable.h"

extern uint IN_FUNC;
extern void yyerror (const char * msg);

#define MAX_TS 500
#define MAX_SIZE_STRING 128
#define MAX_SIZE_FUNCTIONS 20

int HEADER = -1;
int HEADER_FUNC = -1;

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
  if (IN_FUNC){

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
 //printf("Fin bloque detectado\n");
 //printf("HEADER = %d\nTABLA SIMBOLOS HASTA AHORA:\n", HEADER);
 //printTS_ALL();
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
    pushTS(FUNC, atrib.lexema, atrib.type, 0, 0);
}

/**
 * @brief Inserción de un parámetro formal en la tabla de símbolos. Se incrementa
 * el número de parámetros de la función anterior.
 * @param atrib Atributo detectado.
 */
void insertFormalParameter(attr atrib){
  if (atrib.isList)
    pushTS(PARAMETER, atrib.lexema, LISTA, 0, atrib.type);
  else
    pushTS(PARAMETER, atrib.lexema, atrib.type, 0, 0);

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
	auxType.lexema = atrib.lexema;

  int found = 0;

  if (1) {

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
attr getTypeFunc(attr atrib) {
  attr aux;
  aux.type = SUS;
  int found = 0;

  for(int i = HEADER; i > 0; i--)
  {
  	if(strcmp(atrib.lexema, TS[i].name) == 0 && TS[i].input == FUNC)
  	{
      found = 1;
      if (TS[i].listDataType == 0) {
        aux.type = TS[i].dataType;
        aux.isList = 0;
      } else {
        aux.type = TS[i].listDataType;
        aux.isList = 1;
      }
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

  return aux;
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

  if (found) {
    if (a.isList == 0 && a.type != TS[entryFunc].dataType ||
        a.isList == 1 && a.type != TS[entryFunc].listDataType) {
      yyerror("Estas intentado devolver una variable de un tipo distinto al esperado");
    }  
  } else {
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
	case 1: // Operador ^
		if(!a.isList && !b.isList){
			if((a.type == ENTERO || a.type == REAL) && (b.type == ENTERO || b.type == REAL)){
				if(a.type == REAL || b.type == REAL)
					res.type = REAL;
				else
					res.type = ENTERO;
			}
			else{
				char msg[70] = "[ERROR SEMÁNTICO] Los atributos deben de ser reales o enteros";
				yyerror(msg);
			}
		}
		else{
			char msg[70] = "[ERROR SEMÁNTICO] Ninguno de los atributos puede ser una lista";
			yyerror(msg);	
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
          res.isList = 0;
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

