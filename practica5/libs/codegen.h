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

	char* getIf(attr exp, attr sent);
	
	char* getIfElse(attr exp, attr sentIf, attr sentElse);
        
	char* getForAsig(attr asig, attr expr, attr sentido, attr sent);

	char* getFor(attr exp1, attr exp2, attr sentido, attr sent);

	char* getWhile(attr exp, attr sent);

	char* getReturn(attr exp);

	char* getScan(attr id);

	attr getParamFunc(attr exp);

	char* paramConcat(attr args, attr exp);

	char* getFuncCall(attr id, attr args);

	char* getParamDec(attr type, attr id);
        
	char* concatParamDec(attr params, attr type, attr id);

	char* getCabecera(attr type, attr nameFunc, attr params);

	void controlStartBlockFunc();

	void controlEndBlockFunc();

	char* genHeadersFunc();

	char* getExternVars(attr type, attr idsMiddle, attr idFinal);

	void writeFuncsFile(attr block);

	attr getCorrectBlock(attr ini, attr vars, attr funcs, attr sent, attr end);

	void writeProgram(attr block);

#endif
