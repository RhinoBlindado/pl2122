#include "libs/codegen.h"
#include <stdio.h>

FILE *output;
int varTemp = 0;
int numTabs = 0;
int globalVars = 0;


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
    fprintf(output, "#include <stdbool.h>\n\n");
}

char * temporal()
{
    char* res = malloc(50);
    sprintf(res, "tmp%i", varTemp);
    varTemp += 1;
    return res;
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

void writeStartBlock()
{
    if(!globalVars){
        writeWithTabs("{\n");
        numTabs += 1;
    }
}

void writeEndBlock()
{
	numTabs -= 1;
	writeWithTabs("}\n");
}

char * getCType(dType t)
{
    char* res = malloc(50);

    if (t == ENTERO)
        sprintf(res, "int ");
    else if (t == REAL)
        sprintf(res, "double ");
    else if (t == CARACTER)
        sprintf(res, "char ");
    else if (t == BOOLEANO)
        sprintf(res, "bool ");

    return res;
}

void writeVars(attr type, attr idsMiddle, attr idFinal)
{
    char* res = malloc(150);
	sprintf(res, "%s%s%s;\n", type.nameTmp, idsMiddle.nameTmp, idFinal.lexema);
	writeWithTabs(res);
}

char * concatVars(attr variousIds, attr newId)
{
	char* res = malloc(150);
	sprintf(res, "%s%s, ", variousIds.nameTmp, newId.lexema);
	return res;
}

void controlGlobalVars()
{
	if(globalVars){
		writeInit();
		globalVars = 0;
		writeStartBlock();
	}
}

