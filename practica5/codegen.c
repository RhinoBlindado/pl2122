#include "libs/codegen.h"
#include <stdio.h>

FILE *output;
int varTemp = 0;
int numTabs = 0;
int globalVars = 0;
int specialBlock = 0;


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

void controlStartSpecialBlock()
{
	if(!specialBlock)
	{
		writeStartBlock();
		specialBlock = 1;
	}
}

void controlEndSpecialBlock()
{
	if(specialBlock)
	{
		writeEndBlock();
		specialBlock = 0;
	}
}

void writeFinalAsig(attr left, attr right)
{
	char* res = malloc(150);
	sprintf(res, "%s = %s;\n", left.lexema, right.nameTmp);
	writeWithTabs(res);
}

void writeMasMenosExpr(attr newVar, attr left, attr op, attr right)
{
	// Control de la escritura del bloque de inicio
	controlStartSpecialBlock();

	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);
	writeWithTabs(declaration);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "+";
	else
		operador = "-";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);
	writeWithTabs(asigIntermedia);
}
