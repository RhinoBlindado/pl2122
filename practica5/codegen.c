#include "libs/codegen.h"
#include <stdio.h>

FILE *output;
FILE *funcs;
int varTemp = 0;
int etiquet = 0;
int numTabs = 0;
int globalVars = 0;
int numTabsAnt = -1;


void openFile()
{
    output = fopen("output.c", "w");
	funcs = fopen("dec_func.h", "w");
}

void closeFile()
{
    fclose(output);
	fclose(funcs);
}

char* genHeaders()
{
	char* res = malloc(300);
    sprintf(res,"/* PROGRAMA GENERADO POR YACC/LEX */\n");
    sprintf(res, "%s#include <stdio.h>\n", res);
    sprintf(res, "%s#include <stdlib.h>\n", res);
	sprintf(res, "%s#include <math.h>\n", res);
    sprintf(res, "%s#include <stdbool.h>\n", res);
	sprintf(res, "%s#include \"dec_func.h\"\n", res);
	sprintf(res, "%s#include \"dec_dat.h\"\n\n", res);
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

char * getCType(attr t)
{
    char* res = malloc(50);

    if (t.type == ENTERO)
        sprintf(res, "int ");
    else if (t.type == REAL)
        sprintf(res, "double ");
    else if (t.type == CARACTER)
        sprintf(res, "char ");
    else if (t.type == BOOLEANO)
        sprintf(res, "bool ");

	if(t.isList){
		char* res2 = malloc(60);
		sprintf(res2, "%s%s", "l_", res);
		return res2;
	}

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Determinar operador
	char* operador = malloc(10);
	if(op.atrib == 0)
		operador = "+";
	else
		operador = "-";

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(left.isList && !right.isList){
		if(op.atrib == 0){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s = %s(&%s,%s);\n\n", newVar.nameTmp, "int_sumarValor", left.nameTmp, right.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s = %s(&%s,%s);\n\n", newVar.nameTmp, "double_sumarValor", left.nameTmp, right.nameTmp);
		}
		else if(op.atrib == 1){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s(&%s,%s);int_copiar(&%s,&%s);\n\n", "int_restarLista", left.nameTmp, right.nameTmp, left.nameTmp, newVar.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s(&%s,%s);double_copiar(&%s,&%s);\n\n", "double_restarLista", left.nameTmp, right.nameTmp, left.nameTmp, newVar.nameTmp);
		}
	}
	else if(!left.isList && right.isList){
		if(op.atrib == 0){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s(&%s,%s);int_copiar(&%s,&%s);\n\n", "int_sumarLista", right.nameTmp, left.nameTmp, right.nameTmp, newVar.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s(&%s,%s);double_copiar(&%s,&%s);\n\n", "double_sumarLista", right.nameTmp, left.nameTmp, right.nameTmp, newVar.nameTmp);
		}
	}
	else
		sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getMasMenosSufExpr(attr newVar, attr op, attr val)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	
	if(left.isList && !right.isList){
		if(op.atrib == 3){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s(&%s,&%s,%s);\n\n", "int_eliminarCopiaDesdePosicion", left.nameTmp, newVar.nameTmp, right.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s(&%s,&%s,%s);\n\n", "double_eliminarCopiaDesdePosicion", left.nameTmp, newVar.nameTmp, right.nameTmp);
		}
		else if(op.atrib == 2){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s = %s(&%s,%s);\n\n", newVar.nameTmp, "int_multiplicarValor", left.nameTmp, right.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s = %s(&%s,%s);\n\n", newVar.nameTmp, "double_multiplicarValor", left.nameTmp, right.nameTmp);
		}
		else if(op.atrib == 0){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s(&%s,%s);int_copiar(&%s,&%s);\n\n", "int_dividirLista", left.nameTmp, right.nameTmp, left.nameTmp, newVar.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s(&%s,%s);double_copiar(&%s,&%s);\n\n", "double_dividirLista", left.nameTmp, right.nameTmp, left.nameTmp, newVar.nameTmp);
		}
	}
	else if(!left.isList && right.isList){
		if(op.atrib == 2){
			if(left.type == ENTERO)
				sprintf(asigIntermedia, "%s(&%s,%s);\nint_copiar(&%s,&%s);\n\n", "int_multiplicarLista", right.nameTmp, left.nameTmp, right.nameTmp, newVar.nameTmp);
			else if(left.type == REAL)
				sprintf(asigIntermedia, "%s(&%s,%s);\ndouble_copiar(&%s,&%s);\n\n", "double_multiplicarLista", right.nameTmp, left.nameTmp, right.nameTmp, newVar.nameTmp);
		}
	}
	else{
		if(op.atrib == 0 || op.atrib == 2 || op.atrib == 3)
			sprintf(asigIntermedia, "%s = %s %s %s;\n\n", newVar.nameTmp, left.nameTmp, operador, right.nameTmp);
		else
			sprintf(asigIntermedia, "%s = pow(%s, %s);\n\n", newVar.nameTmp, left.nameTmp, right.nameTmp);
	}

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getIgualdadExpr(attr newVar, attr left, attr op, attr right)
{
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

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
	char* res = malloc(1000);
	sprintf(res, "%s%s", getStartBlock(), right.gen);
	sprintf(res, "%s%s%s = %s;\n", res, getTabs(), left.lexema, right.nameTmp);
	sprintf(res, "%s%s", res, getEndBlock());
	return res;
}

char* declarateTmpVar(char* var, attr exp)
{
	char* res = malloc(1000);
	sprintf(res, "%s%s%s;\n", getTabs(), getCType(exp), var);
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

char* getWhile(attr exp, attr sent)
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
	
	// Escribir etiqueta inicial
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqIni);
	
	// Salto condicional
	sprintf(res, "%s%sif(!%s) goto %s;\n", res, getTabs(), varSalida, etiqOut);

	// Sentencias del bucle
	sprintf(res, "%s%s", res, sent.gen);

	// Actualizar variables del bucle
	if(genVar){
		sprintf(res, "%s%s", res, getStartBlock());
		sprintf(res, "%s%s", res, exp.gen);
		sprintf(res, "%s%s%s = %s;\n", res, getTabs(), varSalida, exp.nameTmp);
		sprintf(res, "%s%s", res, getEndBlock());
	}

	// Escribir salto incondicional
	sprintf(res, "%s%sgoto %s;\n", res, getTabs(), etiqIni);

	// Escribir etiqueta final
	sprintf(res, "%s%s%s:\n", res, getTabs(), etiqOut);
	sprintf(res, "%s%s", res, getStartBlock());
	sprintf(res, "%s%s", res, getEndBlock());

	return res; 
}


char* getReturn(attr exp)
{
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

	// Escribir return
	sprintf(res, "%s%sreturn %s;\n", res, getTabs(), varSalida);
	
	return res;
}

char* getScan(attr id)
{
	char* res = malloc(1000);

	if(id.type == ENTERO)
		sprintf(res, "%s%sscanf(\"%sd\", &%s);\n", res, getTabs(), "%", id.lexema);
	else if(id.type == REAL)
		sprintf(res, "%s%sscanf(\"%slf\", &%s);\n", res, getTabs(), "%", id.lexema);
	else if(id.type == CARACTER && id.isList == 0)
		sprintf(res, "%s%sscanf(\"%sc\", &%s);\n", res, getTabs(), "%", id.lexema);
	else if(id.type == BOOLEANO)
		sprintf(res, "%s%sscanf(\"%sd\", &%s);\n", res, getTabs(), "%", id.lexema);
	else if (id.type == CARACTER && id.isList == 1)
		sprintf(res, "%s%sscanf(\"%ss\", &%s);\n", res, getTabs(), "%", id.lexema);

	return res;
}

attr getParamFunc(attr exp)
{
	// Copiar atributo
	attr auxAttr;
	auxAttr.atrib = exp.atrib;
	auxAttr.lexema = exp.lexema;
	auxAttr.type = exp.type;
	auxAttr.isList = exp.isList;
	auxAttr.nameTmp = exp.nameTmp;
	auxAttr.gen = exp.gen;

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

	// Actualizar atributo
	auxAttr.nameTmp = varSalida;
	auxAttr.gen = res;

	return auxAttr;
}

char* paramConcat(attr args, attr exp)
{
	char* res = malloc(200);
	sprintf(res, "%s, %s", args.nameTmp, exp.nameTmp);
	return res;
}

char* getFuncCall(attr id, attr args)
{
	char* res = malloc(300);
	sprintf(res, "%s(%s)", id.lexema, args.nameTmp);
	return res;
}

char* getParamDec(attr type, attr id)
{
	char* res = malloc(100);
	sprintf(res, "%s %s", getCType(type), id.lexema);
	return res;
}

char* concatParamDec(attr params, attr type, attr id)
{
	char* res = malloc(200);
	sprintf(res, "%s, %s", params.gen, getParamDec(type, id));
	return res;
}

char* getCabecera(attr type, attr nameFunc, attr params)
{
	char* res = malloc(300);
	sprintf(res, "%s%s %s(%s)\n", getTabs(), getCType(type), nameFunc.lexema, params.gen);
	return res;
}

void controlStartBlockFunc()
{
	if(numTabsAnt == -1){
		numTabsAnt = numTabs;
		numTabs = 0;
	}
}

void controlEndBlockFunc()
{
	if(numTabs == 0){
		numTabs = numTabsAnt;
		numTabsAnt = -1;
	}
}

char* genHeadersFunc()
{
	char* res = malloc(300);
    sprintf(res,"/* PROGRAMA GENERADO POR YACC/LEX */\n");
	sprintf(res, "%s#ifndef FUNCIONES\n", res);
	sprintf(res, "%s#define FUNCIONES\n", res);
    sprintf(res, "%s#include <stdio.h>\n", res);
    sprintf(res, "%s#include <stdlib.h>\n", res);
	sprintf(res, "%s#include <math.h>\n", res);
	sprintf(res, "%s#include \"dec_dat.h\"\n", res);
    sprintf(res, "%s#include <stdbool.h>\n\n", res);
	return res;
}

char* getExternVars(attr type, attr idsMiddle, attr idFinal)
{
    char* res = malloc(150);
	if(globalVars)
		sprintf(res, "%s %s%s%s;\n", "extern", type.nameTmp, idsMiddle.nameTmp, idFinal.lexema);
	return res;
}

void writeFuncsFile(attr block)
{
	char* res = malloc(3000);
	sprintf(res, "%s%s%s", genHeadersFunc(), block.funcGen, "#endif");
	fprintf(funcs, "%s", res);
}

attr getCorrectBlock(attr ini, attr vars, attr funcs, attr sent, attr end, int IN_FUNC)
{
	attr block;

	char* res = malloc(5000);
	if(IN_FUNC)
		sprintf(res, "%s%s%s%s%s", ini.gen, vars.gen, funcs.gen, sent.gen, end.gen);
	else
		sprintf(res, "%s%s%s%s", ini.gen, vars.gen, sent.gen, end.gen);

	char* res2 = malloc(5000);
	if(strcmp(vars.gen, "") == 0)
		sprintf(res2, "%s", funcs.gen);
	else
		sprintf(res2, "%s\n%s", vars.funcGen, funcs.gen);

	block.gen = res;
	block.funcGen = res2;

	return block;
}

void writeProgram(attr block)
{
	fprintf(output, "%s", block.gen);
	writeFuncsFile(block);
}

char* getHashExpr(attr newVar, attr op, attr val){

	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(val.type == ENTERO)
		sprintf(asigIntermedia, "%s = int_longitud(&%s);\n\n", newVar.nameTmp, val.nameTmp);
	else if(val.type == REAL)
		sprintf(asigIntermedia, "%s = double_longitud(&%s);\n\n", newVar.nameTmp, val.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getInterrExpr(attr newVar, attr op, attr val){

	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(val.type == ENTERO)
		sprintf(asigIntermedia, "%s = int_consultar(%s);\n\n", newVar.nameTmp, val.nameTmp);
	else if(val.type == REAL)
		sprintf(asigIntermedia, "%s = double_consultar(%s);\n\n", newVar.nameTmp, val.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getAtExpr(attr newVar, attr left, attr op, attr right){
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(left.type == ENTERO)
		sprintf(asigIntermedia, "%s = int_consultarEnPosicion(%s, %s);\n\n", newVar.nameTmp, left.nameTmp, right.nameTmp);
	else if(left.type == REAL)
		sprintf(asigIntermedia, "%s = double_consultarEnPosicion(%s, %s);\n\n", newVar.nameTmp, left.nameTmp, right.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getTernExpr(attr newVar, attr expr1, attr expr2, attr expr3){
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(expr1.type == ENTERO)
		sprintf(asigIntermedia, "int_insertarCopia(&%s, &%s, %s, %s);\n\n", expr1.nameTmp, newVar.nameTmp, expr3.nameTmp, expr2.nameTmp);
	else if(expr1.type == REAL)
		sprintf(asigIntermedia, "double_insertarCopia(&%s, &%s, %s, %s);\n\n", expr1.nameTmp, newVar.nameTmp, expr3.nameTmp, expr2.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getMinMinExpr(attr newVar, attr expr1, attr expr2){
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(expr1.type == ENTERO)
		sprintf(asigIntermedia, "int_eliminarCopia(&%s, &%s, %s);\n\n", expr1.nameTmp, newVar.nameTmp, expr2.nameTmp);
	else if(expr1.type == REAL)
		sprintf(asigIntermedia, "double_eliminarCopia(&%s, &%s, %s);\n\n", expr1.nameTmp, newVar.nameTmp, expr2.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}

char* getConcatExpr(attr newVar, attr expr1, attr expr2){
	// Declarar nueva variables
	char* declaration = malloc(150);
	sprintf(declaration, "%s%s;\n", getCType(newVar), newVar.nameTmp);

	// Escribir asignación intermedia
	char* asigIntermedia = malloc(150);

	if(expr1.type == ENTERO)
		sprintf(asigIntermedia, "int_concatenarCopia(&%s, &%s, &%s);\n\n", expr1.nameTmp, expr2.nameTmp, newVar.nameTmp);
	else if(expr1.type == REAL)
		sprintf(asigIntermedia, "double_concatenarCopia(&%s, &%s, &%s);\n\n", expr1.nameTmp, expr2.nameTmp, newVar.nameTmp);

	char* res = malloc(300);
	sprintf(res, "%s\t%s%s\t%s", getTabs(), declaration, getTabs(), asigIntermedia);

	return res;
}


char* getSentIter(attr id, attr op){
	char* res = malloc(700);

	// Escribir sentencia
	if(op.atrib == 0){
		if(id.type == ENTERO)
			sprintf(res, "%sint_avanzarPuntero(&%s);\n", getTabs(), id.lexema);
		else if(id.type == REAL)
			sprintf(res, "%sdouble_avanzarPuntero(&%s);\n", getTabs(), id.lexema);
	}	
	else{
		if(id.type == ENTERO)
			sprintf(res, "%sint_retrocederPuntero(&%s);\n", getTabs(), id.lexema);
		else if(id.type == REAL)
			sprintf(res, "%sdouble_retrocederPuntero(&%s);\n", getTabs(), id.lexema);
	}

	
	return res;
}

char* getIniIter(attr id){

	char* res = malloc(700);	

	if(id.type == ENTERO)
		sprintf(res, "%sint_punteroAInicio(&%s);\n", getTabs(), id.lexema);
	else if(id.type == REAL)
		sprintf(res, "%sdouble_punteroAInicio(&%s);\n", getTabs(), id.lexema);

	return res;
}
