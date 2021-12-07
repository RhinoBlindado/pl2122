#include <string.h>
#include <stdlib.h>
#ifndef SYMBOL_TABLE

    #define SYMBOL_TABLE

    typedef enum
    {
        EMPTY,
        BLOCK_START,   // Marca para señalar los inicios de un nuevo bloque o cambio de scope
        FUNC,          // Indica que la entrada a la tabla de simbolos es una funcion
        VARIABLE,      // Indica que la entrada a la tabla de simbolos es una variable
        PARAMETER      // Indica que la entrada a la tabla de simbolos es un parámetro
    } typeInput;

    typedef enum
    {
        SUS,         // = null. Valor por defecto
        ENTERO,
        REAL,
        CARACTER,
        BOOLEANO,
        LISTA,
        NO_ASIGNADO  // Variable sin tipo asignado (declaración)
    } dType;

    typedef struct 
    {
        int atrib;
        char* lexema;
        dType type;
        int isList;
        char* nameTmp;
        char* gen;
    } attr;


    char* getStr(dType t) {
        char* res = malloc(50);

        switch (t) {
            case (SUS):
                strcpy(res, "SUS");
            break;
            case (ENTERO):
                strcpy(res, "ENTERO");
            break;
            case (REAL):
                strcpy(res, "REAL");
            break;
            case (CARACTER):
                strcpy(res, "CARACTER");
            break;
            case (BOOLEANO):
                strcpy(res, "BOOLEANO");
            break;
            case (LISTA):
                strcpy(res, "LISTA");
            break;
            case (NO_ASIGNADO):
                strcpy(res, "NO_ASIGNADO");
            break;
    }

    return res;
    }

    typedef struct
    {
        // Tipo de entrada en la tabla de símbolos
        typeInput input;
        // Nombre del lexema
        char* name;
        // Tipo de datos
        dType dataType;
        // Numero de datos de la funcion
        uint params;
        // Elementos para lista
        dType listDataType;
    } symbolTable;


#endif
