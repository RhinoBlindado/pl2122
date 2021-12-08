#include "../symbolTable.h"

#ifdef CODEGEN
    #define CODEGEN

    void openFile();

    void closeFile();

    char* genHeaders();

	char* temporal();

	char* etiqueta();

    char* getInit();

    char* getWithTabs(const char* something);

	char* getTabs();

	char* getStartBlock();

	char* getEndBlock()

    char* getCType(dType t);

	char* getVars(attr type, attr idsMiddle, attr idFinal);

	char* concatVars(attr variousIds, attr newId);

	char* controlGlobalVars();

	char* getMasMenosExpr(attr newVar, attr left, attr op, attr right);

	char* getMasMenosSufExpr(attr newVar, attr op, attr val);

	char* getNotExpr(attr newVar, attr op, attr val);

	char* getOpMultExpr(attr newVar, attr left, attr op, attr right);

	char* getIgualdadExpr(attr newVar, attr left, attr op, attr right);

	char* getDesigualExpr(attr newVar, attr left, attr op, attr right);

	char* getAndExpr(attr newVar, attr left, attr op, attr right);
	
	char* getOrExpr(attr newVar, attr left, attr op, attr right);

	char* getXorExpr(attr newVar, attr left, attr op, attr right);

	char* equivalentCLexema(attr atributo);

	char* concatGen(char* left, char* right);

	char* declarateTmpVar(char* var);

	char* getAsig(attr left, attr right);

	char* getPrint(attr exp, attr sent);

	char* getIf(attr exp);

	void writeProgram(attr block);

#endif
