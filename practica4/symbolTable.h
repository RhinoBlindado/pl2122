#include <string.h>
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