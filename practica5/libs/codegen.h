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

    void normalWrite(const char* something);

	void writeStartBlock();

	void writeEndBlock()

    char* getCType(dType t);

	void writeVars(attr type, attr idsMiddle, attr idFinal);

	char * concatVars(attr variousIds, attr newId);

	void controlGlobalVars();
#endif
