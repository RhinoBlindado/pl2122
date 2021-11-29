#include <string.h>
#ifndef SYMBOL_TABLE

    #define SYMBOL_TABLE

    typedef enum
    {
        EMPTY,
        BLOCK_START,   
        FUNC,
        VARIABLE,
        PARAMETER,
    } typeInput;

    typedef enum
    {
        SUS,
        ENTERO,
        REAL,
        CARACTER,
        BOOLEANO,
        LISTA,
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