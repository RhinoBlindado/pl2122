#include "libs/codegen.h"
#include <stdio.h>

FILE *output;

void test()
{
    printf("Hola\n");
}

void openFile()
{
    output = fopen("output.c", "w");
}

void closeFile()
{
    fclose(output);
}

void genHeaders()
{
    fprintf(output,"/* PROGRAMA GENERADO POR YACC/LEX */\n");
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#include <stdlib.h>\n");
}


void writeff()
{
    fprintf(output, "a\n");
}
