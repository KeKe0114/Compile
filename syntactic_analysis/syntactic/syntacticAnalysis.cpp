#include "syntacticAnalysis.h"

void syntacticAnalysis::genMidFuncDef(string funcName)
{
    symAttr funcAttr = symbolist.get(funcName);
    string type_str;
    if (funcAttr.type == VOID)
    {
        type_str = "void";
    }
    else if (funcAttr.type == INT)
    {
        type_str = "int";
    }
    else if (funcAttr.type == CHAR)
    {
        type_str = "char";
    }
    else
    {
        type_str = "UNKNOWN_ERROR";
    }
    midFile << type_str << " " << funcAttr.name << "()" << endl;
}
void syntacticAnalysis::genMidFuncPara(string paraName)
{
    symAttr paraAttr = symbolist.get(paraName);
    string type_str;
    if (paraAttr.type == VOID)
    {
        type_str = "void";
    }
    else if (paraAttr.type == INT)
    {
        type_str = "int";
    }
    else if (paraAttr.type == CHAR)
    {
        type_str = "char";
    }
    else
    {
        type_str = "UNKNOWN_ERROR";
    }
    midFile << "para " << type_str << " " << paraAttr.name << endl;
}

void syntacticAnalysis::genMidArgsPush(string paraName) /*标识符或常量*/
{
    midFile << "push " << paraName << endl;
}
void syntacticAnalysis::genMidFuncCall(string func)
{
    midFile << "call " << func << endl; /*TODO:func Label*/
}
void syntacticAnalysis::genMidFuncRet(string name)
{
    midFile << "ret " << name << endl;
}
string syntacticAnalysis::genMidFuncRetUse()
{
    string temp_val = genMid_AllocTmp();
    midFile << temp_val << "=RET" << endl;
    return temp_val;
}

void syntacticAnalysis::genMidVarState(string name)
{
    symAttr varAttr = symbolist.get(name);
    string type_str;
    if (varAttr.type == VOID)
    {
        type_str = "void";
    }
    else if (varAttr.type == INT)
    {
        type_str = "int";
    }
    else if (varAttr.type == CHAR)
    {
        type_str = "char";
    }
    else
    {
        type_str = "UNKNOWN_ERROR";
    }
    if (varAttr.len = 0)
    {
        midFile << "var" << type_str << name << "%" << varAttr.SymId << endl;
    }
    else
    {
        midFile << "var" << type_str << name << "[" << varAttr.len << "]"
                << "%" << varAttr.SymId << endl;
    }
}
void syntacticAnalysis::genMidConstState(string name)
{
    symAttr varAttr = symbolist.get(name);
    string type_str;
    if (varAttr.type == VOID)
    {
        type_str = "void";
    }
    else if (varAttr.type == INT)
    {
        type_str = "int";
    }
    else if (varAttr.type == CHAR)
    {
        type_str = "char";
    }
    else
    {
        type_str = "UNKNOWN_ERROR";
    }
    midFile << "const" << type_str << name << "%" << varAttr.SymId << endl;
}

string syntacticAnalysis::genMid_AllocLabel()
{
    string ret = "Label" + to_string(label_idx);
    label_idx++;
    return ret;
}
void syntacticAnalysis::genMidLabelLine(string Label)
{
    midFile << Label << ":" << endl;
}
void syntacticAnalysis::genMid_ResetTmp()
{
    temp_idx = 0;
}
string syntacticAnalysis::genMid_AllocTmp()
{
    string ret = "_tmp" + to_string(temp_idx);
    temp_idx++;
    return ret;
}
string syntacticAnalysis::genMidExpress(string operand1, string op, string operand2)
{
    string ret = genMid_AllocTmp();
    string p1;
    string p2;
    if (symbolist.has(operand1))
    {
        symAttr pAttr = symbolist.get(operand1);
        p1 = operand1 + "%" + to_string(pAttr.SymId);
    }
    else
    {
        p1 = operand1;
    }
    if (symbolist.has(operand2))
    {
        symAttr pAttr = symbolist.get(operand2);
        p2 = operand2 + "%" + to_string(pAttr.SymId);
    }
    else
    {
        p2 = operand2;
    }
    midFile << ret << "=" << p1 << op << p2 << endl;
    return ret;
}
string syntacticAnalysis::genMidValueGet(string name)
{
    string ret = genMid_AllocTmp();
    symAttr nameAttr = symbolist.get(name);
    midFile << ret << "=" << name << "%" << nameAttr.SymId << endl;
    return ret;
}
void syntacticAnalysis::genMidValuePut(string name, string value)
{
    symAttr nameAttr = symbolist.get(name);
    midFile << name << "%" << nameAttr.SymId << "=" << value << endl;
}
string syntacticAnalysis::genMidArrayValueGet(string array, string idx)
{
    string ret = genMid_AllocTmp();
    symAttr arrayAttr = symbolist.get(array);
    midFile << ret << "=" << array << "%" << arrayAttr.SymId << "[" << idx
            << "]" << endl;
    return ret;
}
void syntacticAnalysis::genMidArrayValuePut(string array, string idx, string value)
{
    symAttr arrayAttr = symbolist.get(array);
    midFile << array << "%" << arrayAttr.SymId << "[" << idx << "]"
            << "=" << value << endl;
}

void syntacticAnalysis::genMidCondition(string operand1, string op, string operand2)
{
    midFile << operand1 << op << operand2 << endl;
}
void syntacticAnalysis::genMidCondition4Num(string operand1)
{
    midFile << operand1 << "!=0" << endl;
}
void syntacticAnalysis::genMidGoto(string Label)
{
    midFile << "goto " << Label << endl;
}
void syntacticAnalysis::genMidBNZ(string Label)
{
    midFile << "BNZ " << Label << endl;
}
void syntacticAnalysis::genMidBZ(string Label)
{
    midFile << "BZ" << Label << endl;
}

void syntacticAnalysis::genMidScanf(string name)
{
    symAttr scanfAttr = symbolist.get(name);
    midFile << "sacnf " << scanfAttr.name << "%" << scanfAttr.SymId;
}
void syntacticAnalysis::genMidPrintfStr(string str)
{
    midFile << "printStr:" << str << endl;
}
void syntacticAnalysis::genMidPrintfExp(symType type, string name)
{
    midFile << "printExp:" << name << "@" << type << endl;
}

void syntacticAnalysis::ERROR_PRINT(int line, string err_code)
{
    out << line << " " << err_code << endl;
}

inline symType transferKeyToType(token_key key)
{
    if (key == INTTK)
        return INT;
    else if (key == CHARTK)
        return CHAR;
    assert(false); /* 如果执行到此处说明有bug */
    return TYPERROR;
}

syntacticAnalysis::syntacticAnalysis(string filename, string outfile, string midfile) : lexical(filename), sym(lexical.getSym(out)), out(outfile), midFile(midfile)
{
}

void syntacticAnalysis::printToken(token key)
{
    // cout << key.getName() << " " << key.getValue() << endl;
}

void syntacticAnalysis::printLine(string s)
{
    // cout << s << endl;
}

bool syntacticAnalysis::isTypeIdentifier(token key)
{
    return typeIdentifier.find(key.getKey()) != typeIdentifier.end();
}

bool syntacticAnalysis::isFuncWithRet(token key)
{
    return funcWithRet.find(key.getValue()) != funcWithRet.end();
}

bool syntacticAnalysis::isFuncWithoutRet(token key)
{
    return funcWithoutRet.find(key.getValue()) != funcWithoutRet.end();
}

bool syntacticAnalysis::isAddOp(token key)
{
    return addOp.find(key.getKey()) != addOp.end();
}

bool syntacticAnalysis::isMulOp(token key)
{
    return mulOp.find(key.getKey()) != mulOp.end();
}

bool syntacticAnalysis::isRelOp(token key)
{
    return relOp.find(key.getKey()) != relOp.end();
}

bool syntacticAnalysis::isCharacter(token key)
{
    return chars.find(key.getKey()) != chars.end();
}

void syntacticAnalysis::procedureCheck()
{
    if (sym.getKey() == CONSTTK)
    {
        constState();
    }
    if (isTypeIdentifier(sym))
    {
        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);

        sym = lexical.getSym(out);

        if (sym.getKey() == LPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            funcWithReturn();
        }
        // else if (sym.getKey() == COMMA || sym.getKey() == LBRACK || sym.getKey() == SEMICN)
        else
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            variableState();
        }
        // else
        // {
        //     assert(false); /*程序到达此处说明有BUG*/
        // }
    }

    while (!(sym.getKey() == VOIDTK && lexical.peek(out).getKey() == MAINTK))
    {
        if (sym.getKey() == VOIDTK)
        {
            funcWithoutReturn();
        }
        else if (isTypeIdentifier(sym))
        {
            funcWithReturn();
        }
        else
        {
            assert(false); /*程序到达此处说明有BUG*/
        }
    }
    mainFunc();
    printLine("<程序>");
    //assert(!lexical.hasSym()); /*main 函数后不应该有其他token*/
}

void syntacticAnalysis::strConCheck()
{
    assert(sym.getKey() == STRCON);
    printToken(sym);
    sym = lexical.getSym(out);
    printLine("<字符串>");
}

void syntacticAnalysis::constState()
{
    while (sym.getKey() == CONSTTK)
    {
        printToken(sym);
        sym = lexical.getSym(out);
        constDefine();
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "k");
            sym = lexical.getSym(out);
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym(out);
        }
    }
    printLine("<常量说明>");
}

void syntacticAnalysis::constDefine()
{
    if (sym.getKey() == INTTK)
    {
        do
        {
            printToken(sym);

            sym = lexical.getSym(out);
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            if (symbolist.hasNowSeg(sym.getValue()))
            {
                // symbolist.DEBUG_PRINT_LIST();
                ERROR_PRINT(sym.getLine(), "b");

                sym = lexical.getSym(out);
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym(out);
                if (!(sym.getKey() == INTCON || isAddOp(sym)))
                {
                    ERROR_PRINT(sym.getLine(), "o");
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym(out);
                    }
                    if (sym.getKey() == SEMICN)
                    {
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
                integer();
            }
            else
            {
                symAttr attr = {sym.getValue(), symType::INT, symKind::CONST};
                symbolist.insert(&attr);

                sym = lexical.getSym(out);
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym(out);
                if (!(sym.getKey() == INTCON || isAddOp(sym)))
                {
                    ERROR_PRINT(sym.getLine(), "o");
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym(out);
                    }
                    if (sym.getKey() == SEMICN)
                    {
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
                attr.value = integer();
                genMidConstState(attr.name);
            }

        } while (sym.getKey() == COMMA);
    }
    else if (sym.getKey() == CHARTK)
    {
        do
        {
            printToken(sym);

            sym = lexical.getSym(out);
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            if (symbolist.hasNowSeg(sym.getValue()))
            {
                ERROR_PRINT(sym.getLine(), "b");
                sym = lexical.getSym(out);
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym(out);
                if (!(isCharacter(sym)))
                {
                    ERROR_PRINT(sym.getLine(), "o");
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym(out);
                    }
                    if (sym.getKey() == SEMICN)
                    {
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
                character();
            }
            else
            {
                symAttr attr = {sym.getValue(), symType::CHAR, symKind::CONST};
                symbolist.insert(&attr);

                sym = lexical.getSym(out);
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym(out);
                if (!(isCharacter(sym)))
                {
                    ERROR_PRINT(sym.getLine(), "o");
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym(out);
                    }
                    if (sym.getKey() == SEMICN)
                    {
                        return;
                    }
                    else
                    {
                        continue;
                    }
                }
                attr.value = character();
                genMidConstState(attr.name);
            }

        } while (sym.getKey() == COMMA);
    }
    else
    {
        assert(false); /*得到的类型标识符不是int,char*/
    }
    printLine("<常量定义>");
}

int syntacticAnalysis::unsignedInteger()
{
    assert(sym.getKey() == INTCON);
    printToken(sym);

    stringstream ss;
    ss << sym.getValue();
    int ret;
    ss >> ret;

    sym = lexical.getSym(out);
    printLine("<无符号整数>");

    return ret;
}

int syntacticAnalysis::integer()
{
    int OpFlag = 1;
    if (isAddOp(sym))
    {
        if (sym.getKey() == MINU)
        {
            OpFlag = -1;
        }
        printToken(sym);
        sym = lexical.getSym(out);
    }
    int value = unsignedInteger();
    printLine("<整数>");
    return OpFlag * value;
}

char syntacticAnalysis::character()
{
    assert(isCharacter(sym));
    printToken(sym);

    stringstream ss;
    ss << sym.getValue();
    int ret;
    ss >> ret;

    sym = lexical.getSym(out);

    return ret;
}

string syntacticAnalysis::stateHead()
{
    assert(isTypeIdentifier(sym));
    printToken(sym);

    symType symtype = transferKeyToType(sym.getKey());

    sym = lexical.getSym(out);
    assert(sym.getKey() == IDENFR);
    string name = sym.getValue();
    printToken(sym);

    if (symbolist.hasNowSeg(name))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        // cout << "symType into func list:\t " << symtype << endl;
        symAttr attr = {name, symtype, FUNC};
        symbolist.insert(&attr);
        // cout << "get symType after insert:\t " << symbolist.getNearFunc().type << endl;
    }

    sym = lexical.getSym(out);
    printLine("<声明头部>");
    return name;
}

void syntacticAnalysis::variableState()
{
    do
    {
        variableDefine();
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "k");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        if (!isTypeIdentifier(sym))
        {
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym(out);
        if (sym.getKey() != IDENFR)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym(out);
        if (!(sym.getKey() == LBRACK || sym.getKey() == SEMICN || sym.getKey() == COMMA))
        {
            //  CHEN::走到这里的两种情况 1.这是一个有返回值的函数的定义;2.这句变量声明缺分号了.
            if (sym.getKey() != LPARENT)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym(out);
                continue;
            }
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            printLine("<变量说明>");
            return;
        }
        lexical.unGetSym();
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
    } while (true);
}

void syntacticAnalysis::variableDefine()
{
    assert(isTypeIdentifier(sym));

    symType symtype = transferKeyToType(sym.getKey());

    do
    {
        printToken(sym);
        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        string symname = sym.getValue();
        if (symbolist.hasNowSeg(symname))
        {
            ERROR_PRINT(sym.getLine(), "b");
        }

        sym = lexical.getSym(out);
        if (sym.getKey() == LBRACK)
        {
            printToken(sym);

            sym = lexical.getSym(out);
            int len = unsignedInteger();

            //TODO: 数组类型和相应的属性
            symAttr attr = {symname, symtype, symKind::VAR, len};
            symbolist.insert(&attr);
            genMidVarState(attr.name);

            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym(out);
                ERROR_PRINT(sym.getLine(), "m");
                sym = lexical.getSym(out);
            }
            else
            {
                printToken(sym);
                sym = lexical.getSym(out);
            }
        }
        else
        {
            symAttr attr = {symname, symtype, symKind::VAR};
            symbolist.insert(&attr);
            genMidVarState(attr.name);
        }
    } while (sym.getKey() == COMMA);
    printLine("<变量定义>");
}

void syntacticAnalysis::funcWithReturn()
{
    string name = stateHead();
    funcWithRet.insert(name);
    genMidFuncDef(name);

    symbolist.direct();

    assert(sym.getKey() == LPARENT);
    printToken(sym);
    sym = lexical.getSym(out);
    argumentList(name);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym(out);
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym(out);
    printLine("<有返回值函数定义>");

    genMidFuncRet("");
    symbolist.redirect();
}

void syntacticAnalysis::funcWithoutReturn()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym(out);
    assert(sym.getKey() == IDENFR);
    funcWithoutRet.insert(sym.getValue());
    printToken(sym);

    string name = sym.getValue();

    if (symbolist.hasNowSeg(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), VOID, FUNC};
        symbolist.insert(&attr);
        genMidFuncDef(attr.name);
    }
    symbolist.direct();

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    argumentList(name);

    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym(out);
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym(out);
    printLine("<无返回值函数定义>");

    genMidFuncRet("");
    symbolist.redirect();
}

void syntacticAnalysis::compoundStatement()
{
    if (sym.getKey() == CONSTTK)
    {
        constState();
    }
    if (isTypeIdentifier(sym))
    {
        variableState();
    }
    statementList();
    printLine("<复合语句>");
}

void syntacticAnalysis::argumentList(string funcName)
{
    if (!isTypeIdentifier(sym))
    {
        printLine("<参数表>");
        return;
    }

    assert(isTypeIdentifier(sym));
    printToken(sym);
    symType arg1Type = transferKeyToType(sym.getKey());

    symbolist.addArgsForNearFunc(transferKeyToType(sym.getKey()));
    symAttr funcAttr = symbolist.get(funcName);
    // funcAttr.addArgs(transferKeyToType(sym.getKey()));
    // cout << "funcName:\t" << funcName << " args size:\t" << funcAttr.getArgs().size() << endl;

    sym = lexical.getSym(out);
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    if (symbolist.hasNowSeg(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), arg1Type, VAR};
        symbolist.insert(&attr);
        genMidFuncPara(attr.name);
    }

    sym = lexical.getSym(out);
    while (sym.getKey() == COMMA)
    {
        printToken(sym);

        sym = lexical.getSym(out);
        assert(isTypeIdentifier(sym));
        printToken(sym);
        arg1Type = transferKeyToType(sym.getKey());

        symAttr funcAttr = symbolist.get(funcName);
        // cout << "funcName:\t" << funcName << " args size:\t" << funcAttr.getArgs().size() << endl;
        symbolist.addArgsForNearFunc(transferKeyToType(sym.getKey()));
        // funcAttr.addArgs(transferKeyToType(sym.getKey()));
        // cout << "funcName:\t" << funcName << " args size:\t" << funcAttr.getArgs().size() << endl;

        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (symbolist.hasNowSeg(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "b");
        }
        else
        {
            symAttr attr = {sym.getValue(), arg1Type, VAR};
            symbolist.insert(&attr);
            genMidFuncPara(attr.name);
        }

        sym = lexical.getSym(out);
    }
    printLine("<参数表>");
}

void syntacticAnalysis::mainFunc()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym(out);
    assert(sym.getKey() == MAINTK);
    printToken(sym);

    if (symbolist.hasNowSeg(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), VOID, FUNC};
        symbolist.insert(&attr);
        genMidFuncDef(attr.name);
    }
    symbolist.direct();

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym(out);
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);
    printLine("<主函数>");

    symbolist.redirect();
}

expRet syntacticAnalysis::expression()
{
    string expTmp;
    symType ret = TYPERROR;
    expRet termRet;
    symType symType1;
    if (isAddOp(sym))
    {
        string op = sym.getValue();
        printToken(sym);
        sym = lexical.getSym(out);
        ret = INT;
        termRet = term();
        expTmp = genMidExpress(0, op, termRet.tmp4val);
    }
    else
    {
        termRet = term();
        ret = termRet.type;
        expTmp = termRet.tmp4val;
    }
    if (isAddOp(sym))
    {
        ret = INT;
    }
    while (isAddOp(sym))
    {
        string op = sym.getValue();
        printToken(sym);
        sym = lexical.getSym(out);
        termRet = term();
        expTmp = genMidExpress(expTmp, op, termRet.tmp4val);
    }
    printLine("<表达式>");
    expRet expTmpRet = {ret, expTmp};
    return expTmpRet;
}

expRet syntacticAnalysis::term()
{
    expRet expRet1 = factor();
    string termTmpRet = expRet1.tmp4val;
    symType ret = expRet1.type;
    if (isMulOp(sym))
    {
        ret = INT;
    }
    while (isMulOp(sym))
    {
        string op = sym.getValue();
        printToken(sym);
        sym = lexical.getSym(out);
        expRet temp = factor();
        termTmpRet = genMidExpress(termTmpRet, op, temp.tmp4val);
    }
    printLine("<项>");
    expRet termRet = {ret, termTmpRet};
    return termRet;
}

expRet syntacticAnalysis::factor()
{
    string factorTmpRet = "";
    symType ret = TYPERROR;
    if (sym.getKey() == IDENFR)
    {
        string identifyName = sym.getValue();
        if (lexical.peek(out).getKey() == LPARENT)
        {
            if (symbolist.has(sym.getValue()))
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            factorTmpRet = invokeFuncWithReturn();
        }
        else if (lexical.peek(out).getKey() == LBRACK)
        {
            if (!symbolist.has(sym.getValue()))
            {
                ERROR_PRINT(sym.getLine(), "c");
            }
            else
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            printToken(sym);
            sym = lexical.getSym(out);
            printToken(sym);
            sym = lexical.getSym(out);
            expRet idxTmp = expression();
            symType idxType = idxTmp.type;
            if (idxType != INT)
            {
                ERROR_PRINT(sym.getLine(), "i");
            }
            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym(out);
                ERROR_PRINT(sym.getLine(), "m");
                sym = lexical.getSym(out);
            }
            else
            {
                printToken(sym);
                sym = lexical.getSym(out);
                factorTmpRet = genMidArrayValueGet(identifyName, idxTmp.tmp4val);
            }
        }
        else
        {
            if (!symbolist.has(sym.getValue()))
            {
                ERROR_PRINT(sym.getLine(), "c");
            }
            else
            {
                ret = symbolist.get(sym.getValue()).type;
                factorTmpRet = genMidValueGet(identifyName);
            }
            printToken(sym);
            sym = lexical.getSym(out);
        }
    }
    else if (sym.getKey() == LPARENT)
    {
        printToken(sym);

        sym = lexical.getSym(out);
        factorTmpRet = expression().tmp4val;
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        ret = INT;
    }
    else if (isCharacter(sym))
    {
        char charValue = character();
        stringstream ss;
        ss << charValue;
        factorTmpRet = ss.str();
        ret = CHAR;
    }
    else if (isAddOp(sym) || sym.getKey() == INTCON)
    {
        int intValue = integer();
        stringstream ss;
        ss << intValue;
        factorTmpRet = ss.str();
        ret = INT;
    }
    else
    {
        assert(false);
    }
    printLine("<因子>");
    expRet expret = {ret, factorTmpRet};
    return expret;
}

bool syntacticAnalysis::isStatementPrefix(token key)
{
    return statementPrefix.find(key.getKey()) != statementPrefix.end();
}

void syntacticAnalysis::statement()
{
    if (sym.getKey() == SEMICN)
    {
        printToken(sym);
        sym = lexical.getSym(out);
    }
    else if (sym.getKey() == IFTK)
    {
        conditionalStatement();
    }
    else if (sym.getKey() == WHILETK || sym.getKey() == FORTK || sym.getKey() == DOTK)
    {
        loopStatement();
    }
    else if (sym.getKey() == LBRACE)
    {
        printToken(sym);
        sym = lexical.getSym(out);
        statementList();
        assert(sym.getKey() == RBRACE);
        printToken(sym);

        sym = lexical.getSym(out);
    }
    else
    {
        if (sym.getKey() == IDENFR)
        {
            if (isFuncWithRet(sym))
            {
                invokeFuncWithReturn();
            }
            else if (isFuncWithoutRet(sym))
            {
                invokeFuncWithoutReturn();
            }
            else if (lexical.peek(out).getKey() == ASSIGN || lexical.peek(out).getKey() == LBRACK)
            {
                assignmentStatement();
            }
            else
            {
                assert(false);
            }
        }
        else if (sym.getKey() == SCANFTK)
        {
            readStatement();
        }
        else if (sym.getKey() == PRINTFTK)
        {
            writeStatement();
        }
        else if (sym.getKey() == RETURNTK)
        {
            returnStatement();
        }
        else
        {
            assert(false);
        }
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "k");
            sym = lexical.getSym(out);
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym(out);
        }
    }
    printLine("<语句>");
}

void syntacticAnalysis::conditionalStatement()
{
    assert(sym.getKey() == IFTK);
    printToken(sym);

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    condition();
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }
    string label_not = genMid_AllocLabel();
    genMidBZ(label_not);
    sym = lexical.getSym(out);
    statement();
    if (sym.getKey() == ELSETK)
    {
        /*if后面有else语句*/
        string label_out = genMid_AllocLabel();
        genMidGoto(label_out);
        printToken(sym);
        sym = lexical.getSym(out);
        statement();
        genMidLabelLine(label_out);
    }
    else
    {
        /*if后面没有else语句*/
        genMidLabelLine(label_not);
    }

    printLine("<条件语句>");
}

void syntacticAnalysis::assignmentStatement()
{
    genMid_ResetTmp();
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string leftvalue = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "c");
    }
    else
    {
        symAttr findRst = symbolist.get(sym.getValue());
        if (findRst.kind == CONST)
        {
            ERROR_PRINT(sym.getLine(), "j");
        }
    }

    sym = lexical.getSym(out);
    if (sym.getKey() == LBRACK)
    {
        printToken(sym);

        sym = lexical.getSym(out);
        expRet expret = expression();
        symType idxType = expret.type;
        if (idxType != INT)
        {
            ERROR_PRINT(sym.getLine(), "i");
        }
        // assert(sym.getKey() == RBRACK);
        if (sym.getKey() != RBRACK)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "m");
            sym = lexical.getSym(out);
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym(out);
        }

        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym(out);
        expRet expret2 = expression();
        genMidArrayValuePut(leftvalue, expret.tmp4val, expret2.tmp4val);
    }
    else
    {
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym(out);

        expRet expret2 = expression();
        genMidValuePut(leftvalue, expret2.tmp4val);
    }

    printLine("<赋值语句>");
}

void syntacticAnalysis::condition()
{
    genMid_ResetTmp();
    expRet expret = expression();
    symType symtype = expret.type;
    if (symtype != INT)
    {
        ERROR_PRINT(sym.getLine(), "f");
    }
    if (isRelOp(sym))
    {
        string op = sym.getValue();
        printToken(sym);
        sym = lexical.getSym(out);
        expRet expret2 = expression();
        symtype = expret2.type;
        if (symtype != INT)
        {
            ERROR_PRINT(sym.getLine(), "f");
        }
        genMidCondition(expret.tmp4val, op, expret2.tmp4val);
    }
    else
    {
        genMidCondition4Num(expret.tmp4val);
    }
    printLine("<条件>");
}

void syntacticAnalysis::loopStatement()
{
    if (sym.getKey() == WHILETK)
    {
        string label_in = genMid_AllocLabel();
        string label_out = genMid_AllocLabel();
        printToken(sym);
        sym = lexical.getSym(out);
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        genMidLabelLine(label_in);
        sym = lexical.getSym(out);
        condition();
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }
        genMidBNZ(label_out);

        sym = lexical.getSym(out);
        statement();
        genMidGoto(label_in);
        genMidLabelLine(label_out);
    }
    else if (sym.getKey() == DOTK)
    {
        printToken(sym);
        sym = lexical.getSym(out);
        string label_loopBegin = genMid_AllocLabel();
        genMidLabelLine(label_loopBegin);
        statement();
        // assert(sym.getKey() == WHILETK);
        if (sym.getKey() != WHILETK)
        {
            ERROR_PRINT(sym.getLine(), "n");
            while (sym.getKey() != SEMICN)
            {
                sym = lexical.getSym(out);
            }
            return;
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym(out);
        condition();
        genMidBNZ(label_loopBegin);
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
    }
    else if (sym.getKey() == FORTK)
    {
        printToken(sym);

        sym = lexical.getSym(out);
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify1_name = sym.getValue();
        if (!symbolist.has(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "c");
        }
        else
        {
            symAttr findRst = symbolist.get(sym.getValue());
            if (findRst.kind == CONST)
            {
                ERROR_PRINT(sym.getLine(), "j");
            }
        }

        sym = lexical.getSym(out);
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym(out);
        expRet expret_for_begin = expression();
        genMidValuePut(identify1_name, expret_for_begin.tmp4val);

        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "k");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        string label_in = genMid_AllocLabel();
        string label_out = genMid_AllocLabel();
        genMidLabelLine(label_in);
        condition();
        genMidBNZ(label_out);
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "k");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify2_name = sym.getValue();
        if (!symbolist.has(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "c");
        }
        else
        {
            symAttr findRst = symbolist.get(sym.getValue());
            if (findRst.kind == CONST)
            {
                ERROR_PRINT(sym.getLine(), "j");
            }
        }

        sym = lexical.getSym(out);
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify3_name = sym.getValue();

        if (!symbolist.has(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "c");
        }

        sym = lexical.getSym(out);
        assert(isAddOp(sym));
        printToken(sym);
        string op4 = sym.getValue();

        sym = lexical.getSym(out);
        int value = stepLength();
        stringstream ss;
        ss << value;
        string value5 = ss.str();

        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
        statement();
        string erpress_end = genMidExpress(identify3_name, op4, value5);
        genMidValuePut(identify2_name, erpress_end);
        genMidGoto(label_in);
        genMidLabelLine(label_out);
    }
    else
    {
        assert(false);
    }
    printLine("<循环语句>");
}

int syntacticAnalysis::stepLength()
{
    int ret = unsignedInteger();
    printLine("<步长>");
    return ret;
}

string syntacticAnalysis::invokeFuncWithReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string funcName = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "c");
        while (sym.getKey() != RPARENT)
        {
            sym = lexical.getSym(out);
        }
        sym = lexical.getSym(out);
        return;
    }

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    valueArgumentList(funcName);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    printLine("<有返回值函数调用语句>");
    genMidFuncCall(funcName);
    return genMidFuncRetUse();
}

void syntacticAnalysis::invokeFuncWithoutReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string funcName = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "c");
        while (sym.getKey() != RPARENT)
        {
            sym = lexical.getSym(out);
        }
        sym = lexical.getSym(out);
        return;
    }

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    valueArgumentList(funcName);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    printLine("<无返回值函数调用语句>");
    genMidFuncCall(funcName);
}

// 注:此处使用了外部信息,即参数表后必须有")"
void syntacticAnalysis::valueArgumentList(string funcName)
{
    symAttr funcAttr = symbolist.get(funcName);
    // cout << "funcAttr.name:\t" << funcAttr.name << " args size:\t" << funcAttr.getArgs().size() << endl;
    vector<symType> argsTypes = funcAttr.getArgs();
    int count = 0;
    if (argsTypes.size() == 0 && sym.getKey() != RPARENT)
    {
        ERROR_PRINT(sym.getLine(), "l");
    }
    if (sym.getKey() == RPARENT)
    {
        if (count != argsTypes.size())
        {
            ERROR_PRINT(sym.getLine(), "d");
        }
        printLine("<值参数表>");
        return;
    }
    genMid_ResetTmp();
    expRet expret = expression();
    genMidArgsPush(expret.tmp4val);
    symType symtype = expret.type;
    if (count < argsTypes.size() && argsTypes[count] != symtype)
    {
        ERROR_PRINT(sym.getLine(), "e");
    }
    count++;
    while (sym.getKey() == COMMA)
    {
        printToken(sym);
        sym = lexical.getSym(out);
        genMid_ResetTmp();
        expRet expret = expression();
        genMidArgsPush(expret.tmp4val);
        symtype = expret.type;

        if (count < argsTypes.size() && argsTypes[count] != symtype)
        {
            ERROR_PRINT(sym.getLine(), "e");
        }
        count++;
    }
    if (count != argsTypes.size())
    {
        ERROR_PRINT(sym.getLine(), "d");
    }
    printLine("<值参数表>");
}

void syntacticAnalysis::statementList()
{
    while (isStatementPrefix(sym))
    {
        statement();
    }
    printLine("<语句列>");
}

void syntacticAnalysis::readStatement()
{
    assert(sym.getKey() == SCANFTK);
    printToken(sym);

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);

    do
    {
        printToken(sym);
        sym = lexical.getSym(out);
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (!symbolist.has(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "c");
        }
        else
        {
            genMidScanf(sym.getValue());
        }
        sym = lexical.getSym(out);
    } while (sym.getKey() == COMMA);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    printLine("<读语句>");
}

void syntacticAnalysis::writeStatement()
{
    genMid_ResetTmp();
    assert(sym.getKey() == PRINTFTK);
    printToken(sym);

    sym = lexical.getSym(out);
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym(out);
    if (sym.getKey() == STRCON)
    {
        strConCheck();
        genMidPrintfStr(sym.getValue());
        if (sym.getKey() == COMMA)
        {
            printToken(sym);
            sym = lexical.getSym(out);
            expRet expret = expression();
            genMidPrintfExp(expret.type, expret.tmp4val);
        }
    }
    else
    {
        expRet expret = expression();
        genMidPrintfExp(expret.type, expret.tmp4val);
    }
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym(out);
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym(out);
    printLine("<写语句>");
}

void syntacticAnalysis::returnStatement()
{
    assert(sym.getKey() == RETURNTK);
    printToken(sym);
    symType chkType = TYPERROR;
    symType getType = VOID;
    if (!symbolist.hasNearFunc())
    {
        DEBUG(1, "NO FUNC Near");
    }
    else
    {
        // cout << "near function name:\t" << symbolist.getNearFunc().name << endl;
        chkType = symbolist.getNearFunc().type;
    }
    sym = lexical.getSym(out);
    if (sym.getKey() == LPARENT)
    {
        printToken(sym);
        sym = lexical.getSym(out);
        expRet expret = expression();
        genMidFuncRet(expret.tmp4val);
        getType = expret.type;
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym(out);
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym(out);
    }
    else
    {
        genMidFuncRet("");
    }

    if (chkType != getType)
    {
        if (chkType == VOID)
        {
            ERROR_PRINT(sym.getLine(), "g");
        }
        else
        {
            ERROR_PRINT(sym.getLine(), "h");
        }
    }
    printLine("<返回语句>");
}

int main(int argc, char const *argv[])
{
    syntacticAnalysis syntactic("testfile.txt", "error.txt", "midfile.txt");
    syntactic.procedureCheck();
    return 0;
}
