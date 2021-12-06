#include "libs/codegen.h"
#include <stdio.h>

FILE *output;
int varTemp = 0;
int numTabs = 0;


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
    fprintf(output, "#include <stdbool.h>\n");
}


void writeff()
{
    fprintf(output, "a\n");
}


void writeTabs()
{
    for(int i = 0; i < numTabs; i++)
        fprintf(output, "\t");
}

void writeInit()
{
    fprintf(output, "\nint main()\n");
}

void writeWithTabs(const char* something)
{
    writeTabs();
    fprintf(output, "%s", something);
}

void normalWrite(const char* something)
{
    fprintf(output, "%s", something);
}

void writeCType(dType t)
{
    if (t == ENTERO)
        writeWithTabs("int ");
    else if (t == REAL)
        writeWithTabs("double ");
    else if (t == CARACTER)
        writeWithTabs("char ");
    else if (t == BOOLEANO)
        writeWithTabs("bool ");
}

char * temporal()
{
    char* res = malloc(50);
    sprintf(res, "tmp%i", varTemp);
    varTemp += 1;
    return res;
}

