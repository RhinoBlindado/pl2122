#ifdef SEMANTIC
    #define SEMANTIC

    void blockStart();

    void printTS_ALL();

    void printTS(symbolTable in);

    void pushTS(typeInput input, char* name, dType dataType, uint params, dType listDataType);

    void popTS();

    void pushAttr(attr atrib);

    void assignType(attr p_dataType);

    void checkBlockFunction();

    void blockEnd();

    attr getTypeVar(attr atrib);

    dType getTypeFunc(attr atrib);

    dType getTypeList(attr atrib);

    dType getExpType(dType typ1, dType typ2);

    void insertFunction(attr atrib);

    void insertFormalParameter(attr atrib);

    void findFunctionCall(attr atrib);

    void checkCallParameters(attr atrib);

    void endCallParameters();

    int checkIfList(attr atrib);

    attr checkAsignacion(attr, attr);

    attr checkMasMenosExp(attr, attr b, attr op);

    attr checkOpMultExp(attr a, attr b, attr op);

    attr checkDesigualdadExp(attr a, attr b);

    dType checkBooleanExp(dType a, dType b);

    dType checkPlusPlusExp(attr a, attr b, attr c);

    attr checkConcatExp(attr a, attr b);

    char* getStr(dType t);

    void checkBooleans(dType typ1);

    void checkReturn(attr a);

    attr checkLista(attr a, attr b);

#endif
