#include "../symbolTable.h"

#ifdef CODEGEN
    #define CODEGEN

    void openFile();

    void closeFile();

    void genHeaders();

    void writeTabs();

    void writeInit();

    void writeWithTabs(const char* something);

    void normalWrite(const char* something);

    void writeCType(dType t);

    char * temporal();

#endif
