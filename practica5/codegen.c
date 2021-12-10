#include "libs/codegen.h"
#include <stdio.h>

FILE *output;
int varTemp = 0;
int etiquet = 0;
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

char* genHeaders()
{
	char* res = malloc(300);
    sprintf(res,"/* PROGRAMA GENERADO POR YACC/LEX */\n");
    sprintf(res, "%s#include <stdio.h>\n", res);
    sprintf(res, "%s#include <stdlib.h>\n", res);
	sprintf(res, "%s#include <math.h>\n", res);
    sprintf(res, "%s#include <stdbool.h>\n\n", res);
	return res;
}

char * temporal()
{
    char* res = malloc(50);
    sprintf(res, "tmp%i", varTemp);
    varTemp += 1;
    return res;
}

char * etiqueta()
{
    char* res = malloc(50);
    sprintf(res, "etiqueta%i", etiquet);
    etiquet += 1;
    return res;
}

char* getInit()
{
	char* res = malloc(50);
    sprintf(res, "\nint main()\n");
	return res;
}

char* getTabs()
{
	char* res = malloc(20);
	for(int i = 0; i < numTabs; i++)
		res[i] = '\t';
	res[numTabs] = '\0';

	return res;
}

char* getWithTabs(const char* something)
{
	char* res = malloc(400);
    sprintf(res, "%s%s", getTabs(), something);
	return res;
}

char* getStartBlock()
{
	char* res = malloc(30);
	res[0] = '\0';
    if(!globalVars){
        sprintf(res, "%s{\n", getTabs());
		numTabs += 1;
	}
	return res;
}

char* getEndBlock()
{
	char* res = malloc(30);
	res[0] = '\0';
	numTabs -= 1;
    sprintf(res, "%s}\n", getTabs());
	return res;
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

char* getVars(attr type, attr idsMiddle, attr idFinal)
{
    char* res = malloc(150);
	sprintf(res, "%s%s%s%s;\n", getTabs(), type.nameTmp, idsMiddle.nameTmp, idFinal.lexema);
	return res;
}

char * concatVars(attr variousIds, attr newId)
{
	char* res = malloc(150);
	sprintf(res, "%s%s, ", variousIds.nameTmp, newId.lexema);
	return res;
}

char* controlGlobalVars()
{
	char* res = malloc(500);
	res[0] = '\0';
	if(globalVars){
		globalVars = 0;
		sprintf(res, "%s%s", getInit(), getStartBlock());
	}
	return res;
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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

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
	char* res = malloc(5000);
	sprintf(res, "%s%s", left, right);
	return res;
}


char* getAsig(attr left, attr right)
{
	char* res = malloc(300);
	sprintf(res, "%s%s", getStartBlock(), right.gen);
	sprintf(res, "%s%s%s = %s;\n", res, getTabs(), left.lexema, right.nameTmp);
	sprintf(res, "%s%s", res, getEndBlock());
	return res;
}

char* declarateTmpVar(char* var, attr exp)
{
	char* res = malloc(300);
	sprintf(res, "%s%s%s;\n", getTabs(), getCType(exp.type), var);
	sprintf(res, "%s%s%s", res, getStartBlock(), exp.gen);
	sprintf(res, "%s%s%s = %s;\n", res, getTabs(), var, exp.nameTmp);
	sprintf(res, "%s%s", res, getEndBlock());  

	return res;
}

char* getPrint(attr exp)
{
	char* res = malloc(1000);

	// Comprobar si hay que generar variable
	int genVar = strcmp(exp.gen, "") != 0;
	char* varSalida;

	if(genVar)
		varSalida = temporal();
	else
		varSalida = exp.nameTmp;

	// Si hay que generar variable, concatenar expresión
	if(genVar)
		res = declarateTmpVar(varSalida, exp);
		

	if(exp.type == ENTERO)
		sprintf(res, "%s%sprintf(\"%sd\", %s);\n", res, getTabs(), "%", varSalida);
	else if(exp.type == REAL)
		sprintf(res, "%s%sprintf(\"%slf\", %s);\n", res, getTabs(), "%", varSalida);
	else if(exp.type == CARACTER && exp.isList == 0)
		sprintf(res, "%s%sprintf(\"%sc\", %s);\n", res, getTabs(), "%", varSalida);
	else if(exp.type == BOOLEANO)
		sprintf(res, "%s%sprintf(\"%sd\", %s);\n", res, getTabs(), "%", varSalida);
	else if (exp.type == CARACTER && exp.isList == 1)
		sprintf(res, "%s%sprintf(\"%ss\", %s);\n", res, getTabs(), "%", varSalida);

	return res;
}

char* getIf(attr exp, attr sent)
{
	char* etiqSig = etiqueta();
	
	char* res = malloc(700);

	// Comprobar si hay que generar variable
	int genVar = strcmp(exp.gen, "") != 0;
	char* varSalida;

	if(genVar)
		varSalida = temporal();
	else
		varSalida = exp.nameTmp;

	// Si hay que generar variable, concatenar expresión
	if(genVar)
		res = declarateTmpVar(varSalida, exp);

	// Concatenar expresion
	sprintf(res, "%s%sif(!%s) goto %s;\n", res, getTabs(), varSalida, etiqSig);
	sprintf(res, "%s%s", res, sent.gen);
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqSig);
	sprintf(res, "%s%s", res, getStartBlock());
	sprintf(res, "%s%s", res, getEndBlock());

	return res;
}

char* getIfElse(attr exp, attr sentIf, attr sentElse)
{
	char* etiqElse = etiqueta();
	char* etiqSig = etiqueta();

	char* res = malloc(700);

	// Comprobar si hay que generar variable
	int genVar = strcmp(exp.gen, "") != 0;
	char* varSalida;

	if(genVar)
		varSalida = temporal();
	else
		varSalida = exp.nameTmp;

	// Si hay que generar variable, concatenar expresión
	if(genVar)
		res = declarateTmpVar(varSalida, exp);

	// Concatenar expresion
	sprintf(res, "%s%sif(!%s) goto %s;\n", res, getTabs(), varSalida, etiqElse);
	sprintf(res, "%s%s", res, sentIf.gen);
	sprintf(res, "%s%sgoto %s;\n", res, getTabs(), etiqSig);
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqElse);
	sprintf(res, "%s%s", res, sentElse.gen);
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqSig);
	sprintf(res, "%s%s", res, getStartBlock());
	sprintf(res, "%s%s", res, getEndBlock());

	return res;
}

char* getForAsig(attr asig, attr exp, attr sentido, attr sent)
{
	char* etiqIni = etiqueta();
	char* etiqOut = etiqueta();

	char* res = malloc(700);

	// Comprobar si hay que generar variable
	int genVar = strcmp(exp.gen, "") != 0;
	char* varSalida;

	if(genVar)
		varSalida = temporal();
	else
		varSalida = exp.nameTmp;

	// Si hay que generar variable, concatenar expresión
	if(genVar)
		res = declarateTmpVar(varSalida, exp);

	// Asignación inicial
	sprintf(res, "%s%s", res, asig.gen);
	
	// Escribir etiqueta inicial
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqIni);
	
	// Concatenar según el sentido
	if(sentido.atrib == 0)
		sprintf(res, "%s%sif(%s > %s) goto %s;\n", res, getTabs(), asig.nameTmp, varSalida, etiqOut);
	else
		sprintf(res, "%s%sif(%s < %s) goto %s;\n", res, getTabs(), asig.nameTmp, varSalida, etiqOut);

	// Sentencias del bucle
	sprintf(res, "%s%s", res, sent.gen);

	// Actualizar variables del bucle
	if(genVar){
		sprintf(res, "%s%s", res, getStartBlock());
		sprintf(res, "%s%s", res, exp.gen);
		sprintf(res, "%s%s%s = %s;\n", res, getTabs(), varSalida, exp.nameTmp);
		sprintf(res, "%s%s", res, getEndBlock());
	}

	// Aumentar o decrementar variable control
	if(sentido.atrib == 0)
		sprintf(res, "%s%s%s += 1;\n", res, getTabs(), asig.nameTmp);
	else
		sprintf(res, "%s%s%s -= 1;\n", res, getTabs(), asig.nameTmp);

	// Escribir salto incondicional
	sprintf(res, "%s%sgoto %s;\n", res, getTabs(), etiqIni);

	// Escribir etiqueta final
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqOut);
	sprintf(res, "%s%s", res, getStartBlock());
	sprintf(res, "%s%s", res, getEndBlock());

	return res; 
}

char* getFor(attr exp1, attr exp2, attr sentido, attr sent)
{
	char* etiqIni = etiqueta();
	char* etiqOut = etiqueta();

	char* res = malloc(700);

	// Comprobar si hay que generar variable para expresión izquierda
	char* varLeft = temporal();
	res = declarateTmpVar(varLeft, exp1);

	// Comprobar si hay que generar variable para expresión izquierda
	int genVarRight = strcmp(exp2.gen, "") != 0;
	char* varRight;

	if(genVarRight)
		varRight = temporal();
	else
		varRight = exp2.nameTmp;

	// Si hay que generar variable, concatenar expresión
	if(genVarRight)
		sprintf(res, "%s%s", res, declarateTmpVar(varRight, exp2));
	
	// Escribir etiqueta inicial
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqIni);
	
	// Concatenar según el sentido
	if(sentido.atrib == 0)
		sprintf(res, "%s%sif(%s > %s) goto %s;\n", res, getTabs(), varLeft, varRight, etiqOut);
	else
		sprintf(res, "%s%sif(%s < %s) goto %s;\n", res, getTabs(), varLeft, varRight, etiqOut);

	// Sentencias del bucle
	sprintf(res, "%s%s", res, sent.gen);

	// Actualizar variables del bucle
	if(genVarRight){
		sprintf(res, "%s%s", res, getStartBlock());
		sprintf(res, "%s%s", res, exp2.gen);
		sprintf(res, "%s%s%s = %s;\n", res, getTabs(), varRight, exp2.nameTmp);
		sprintf(res, "%s%s", res, getEndBlock());
	}

	// Aumentar o decrementar variable control
	if(sentido.atrib == 0)
		sprintf(res, "%s%s%s += 1;\n", res, getTabs(), varLeft);
	else
		sprintf(res, "%s%s%s -= 1;\n", res, getTabs(), varLeft);

	// Escribir salto incondicional
	sprintf(res, "%s%sgoto %s;\n", res, getTabs(), etiqIni);

	// Escribir etiqueta final
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqOut);
	sprintf(res, "%s%s", res, getStartBlock());
	sprintf(res, "%s%s", res, getEndBlock());

	return res; 
}


void writeProgram(attr block)
{
	fprintf(output, "%s", block.gen);
}




