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
	fprintf(output, "#include <math.h>\n");
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

char* getTabs()
{
	char* res = malloc(20);
	for(int i = 0; i < numTabs + 1; i++)
		res[i] = '\t';
	res[numTabs + 1] = '\0';

	return res;
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
	else
		sprintf(res, "mierda ");

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


void writeAsig(attr left, attr right)
{
	writeStartBlock();
	normalWrite(right.gen);
	char* res = malloc(150);
	sprintf(res, "%s = %s;\n", left.lexema, right.nameTmp);
	writeWithTabs(res);
	writeEndBlock();
}

char* getMasMenosExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "+";
	else
		operador = "-";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getMasMenosSufExpr(attr newVar, attr op, attr val)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "+";
	else
		operador = "-";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s;\n\n", newVar.nameTmp, operador, val.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getNotExpr(attr newVar, attr op, attr val)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s;\n\n", newVar.nameTmp, "!", val.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getOpMultExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "/";
	else if(op.atrib == 2)
		operador = "*";
	else if(op.atrib == 3)
		operador = "%";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	
	if(op.atrib == 0 || op.atrib == 2 || op.atrib == 3)
		sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);
	else
		sprintf(asigIntermedia, "%s = pow(%s, %s);\n\n", newVar.nameTmp, left.nameTmp, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getIgualdadExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "==";
	else
		operador = "!=";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getDesigualExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "<";
	else if(op.atrib == 1)
		operador = "<=";
	else if(op.atrib == 2)
		operador = ">";
	else
		operador = ">=";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getAndExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, "&&", right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getOrExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, "||", right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getXorExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variable
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar.type), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);
	sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, "!=", right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s%s%s%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* equivalentCLexema(attr atributo)
{
	char* lex = malloc(150);
	
	if(atributo.type == BOOLEANO)
		if(strcmp(atributo.lexema, "Verdadero") == 0)
			sprintf(lex, "%s", "true");
		else
			sprintf(lex, "%s", "false");
	else
		sprintf(lex, "%s", atributo.lexema);

	return lex;
}

char* concatGen(char* left, char* right)
{
	char* res = malloc(2000);
	sprintf(res, "%s%s", left, right);
	return res;
}

char* declarateTmpVar(attr exp)
{
	char* var = temporal();

	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(exp.type), var);
	writeWithTabs(declaration);
	writeStartBlock();
	normalWrite(exp.gen);
	char * assign = malloc(150);
	sprintf(assign, "%s = %s;\n", var, exp.nameTmp);
	writeWithTabs(assign);
	writeEndBlock();

	return var;
}

void writePrint(attr exp)
{
	// Comprobar si hay que generar variable
	int genVar = strcmp(exp.gen, "") != 0;
	char* varSalida;

	if(genVar)
		varSalida = declarateTmpVar(exp);
	else
		varSalida = exp.nameTmp;

	char* salida = malloc(150);

	if(exp.type == ENTERO){
		sprintf(salida, "printf(\"%sd\\n\", %s);\n", "%", varSalida); 
		writeWithTabs(salida);
	}
	else if(exp.type == REAL){
		sprintf(salida, "printf(\"%slf\\n\", %s);\n", "%", varSalida); 
		writeWithTabs(salida);
	}
	else if(exp.type == CARACTER){
		sprintf(salida, "printf(\"%sc\\n\", %s);\n", "%", varSalida); 
		writeWithTabs(salida);
	}
	else if(exp.type == BOOLEANO){
		sprintf(salida, "printf(\"%sd\\n\", %s);\n", "%", varSalida); 
		writeWithTabs(salida);
	}
}




