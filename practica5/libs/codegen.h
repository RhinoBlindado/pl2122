#include "../symbolTable.h"

#ifdef CODEGEN
    #define CODEGEN

    void openFile();

    void closeFile();

    void genHeaders();

	char * temporal();

    void writeTabs();

    void writeInit();

    void writeWithTabs(const char* something);

	char* getTabs();

    void normalWrite(const char* something);

	void writeStartBlock();

	void writeEndBlock()

    char* getCType(dType t);

	void writeVars(attr type, attr idsMiddle, attr idFinal);

	char* concatVars(attr variousIds, attr newId);

	void controlGlobalVars();

	void writeAsig(attr left, attr right);

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

	char* declarateTmpVar(attr exp);

	void writePrint(attr exp);

#endif
