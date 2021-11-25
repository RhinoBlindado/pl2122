#include <string.h>
#ifndef SYMBOL_TABLE

    #define SYMBOL_TABLE

    typedef enum
    {
        MARCA,   
        FUNCION,
        VARIABLE,
        PARAMETER
    } typeInput;


    typedef enum
    {
        ENTERO,
        REAL,
        CARACTER,
        BOOLEANO,
        LISTA,
        SUS,
        NO_ASIGNADO
    } dType;

    typedef struct
    {
        typeInput input;
        char* name;
        dType dataType;
        uint params;
        uint dimens;
        int tamDimen;
    } symbolTable;


#endif