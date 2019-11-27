#include "syntacticAnalysis.h"

inline symType transferKeyToType(token_key key)
{
    if (key == INTTK)
        return INT;
    else if (key == CHARTK)
        return CHAR;
    assert(false); /* 如果执行到此处说明有bug */
    return TYPERROR;
}

syntacticAnalysis::syntacticAnalysis(string filename, string outfile) : lexical(filename), sym(lexical.getSym()), out(outfile), symbolist(symbols::get_instance()), errmag(errorMags::get_instance()), midcode(midCodeGen::get_instance()) {}

void syntacticAnalysis::printToken(token key)
{
    cout << key.getName() << " " << key.getValue() << " " << key.getLine() << endl;
}

void syntacticAnalysis::printLine(string s)
{
    cout << s << endl;
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
    symbolist.direct();
    if (sym.getKey() == CONSTTK)
    {
        constState();
    }
    if (isTypeIdentifier(sym))
    {
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);

        sym = lexical.getSym();

        if (sym.getKey() == LPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            funcWithReturn();
        }
        // else if (sym.getKey() == COMMA || sym.getKey() == LBRACK || sym.getKey() == SEMICN)
        else
        {
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            variableState();
        }
        // else
        // {
        //     assert(false); /*程序到达此处说明有BUG*/
        // }
    }

    while (!(sym.getKey() == VOIDTK && lexical.peek().getKey() == MAINTK))
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
    //assert(!lexical.hasSym()); /*main 函数后不应该有其他token*/
    printLine("<程序>");
    symbolist.redirect();
    symbolist.DEBUG_PRINT_ALL_SYM();
}

void syntacticAnalysis::strConCheck()
{
    assert(sym.getKey() == STRCON);
    printToken(sym);
    sym = lexical.getSym();
    printLine("<字符串>");
}

void syntacticAnalysis::constState()
{
    while (sym.getKey() == CONSTTK)
    {
        printToken(sym);
        sym = lexical.getSym();
        constDefine();
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.K);
            sym = lexical.getSym();
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym();
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

            sym = lexical.getSym();
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            if (symbolist.hasNowSeg(sym.getValue()))
            {
                // symbolist.DEBUG_PRINT_LIST();
                errmag.errPut(sym.getLine(), errmag.B);

                sym = lexical.getSym();
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym();
                if (!(sym.getKey() == INTCON || isAddOp(sym)))
                {
                    errmag.errPut(sym.getLine(), errmag.O);
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym();
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
                string intConstName = sym.getValue();

                sym = lexical.getSym();
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym();
                if (!(sym.getKey() == INTCON || isAddOp(sym)))
                {
                    errmag.errPut(sym.getLine(), errmag.O);
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym();
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
                symAttr attr(intConstName, symType::INT, symKind::CONST);
                attr.value = to_string(integer());
                symbolist.insert(attr);
                midcode.genMidConstState(attr.name);
            }

        } while (sym.getKey() == COMMA);
    }
    else if (sym.getKey() == CHARTK)
    {
        do
        {
            printToken(sym);

            sym = lexical.getSym();
            assert(sym.getKey() == IDENFR);
            printToken(sym);

            if (symbolist.hasNowSeg(sym.getValue()))
            {
                errmag.errPut(sym.getLine(), errmag.B);
                sym = lexical.getSym();
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym();
                if (!(isCharacter(sym)))
                {
                    errmag.errPut(sym.getLine(), errmag.O);
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym();
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
                string charConstName = sym.getValue();

                sym = lexical.getSym();
                assert(sym.getKey() == ASSIGN);
                printToken(sym);

                sym = lexical.getSym();
                if (!(isCharacter(sym)))
                {
                    errmag.errPut(sym.getLine(), errmag.O);
                    while (!(sym.getKey() == SEMICN || sym.getKey() == COMMA))
                    {
                        sym = lexical.getSym();
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
                symAttr attr(charConstName, symType::CHAR, symKind::CONST);
                string ch = "a";
                ch[0] = character();
                attr.value = ch;
                symbolist.insert(attr);
                midcode.genMidConstState(attr.name);
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

    sym = lexical.getSym();
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
        sym = lexical.getSym();
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
    char ret;
    ss >> ret;

    sym = lexical.getSym();

    return ret;
}

string syntacticAnalysis::stateHead()
{
    assert(isTypeIdentifier(sym));
    printToken(sym);

    symType symtype = transferKeyToType(sym.getKey());

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    string name = sym.getValue();
    printToken(sym);

    if (symbolist.hasNowSeg(name))
    {
        errmag.errPut(sym.getLine(), errmag.B);
    }
    else
    {
        // cout << "symType into func list:\t " << symtype << endl;
        symAttr attr(name, symtype, FUNC);
        symbolist.insert(attr);
        // cout << "get symType after insert:\t " << symbolist.getNearFunc().type << endl;
    }

    sym = lexical.getSym();
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
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.K);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        if (!isTypeIdentifier(sym))
        {
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym();
        if (sym.getKey() != IDENFR)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            printLine("<变量说明>");
            return;
        }
        sym = lexical.getSym();
        if (!(sym.getKey() == LBRACK || sym.getKey() == SEMICN || sym.getKey() == COMMA))
        {
            //  CHEN::走到这里的两种情况 1.这是一个有返回值的函数的定义;2.这句变量声明缺分号了.
            if (sym.getKey() != LPARENT)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym();
                continue;
            }
            lexical.unGetSym();
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            printLine("<变量说明>");
            return;
        }
        lexical.unGetSym();
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
    } while (true);
}

void syntacticAnalysis::variableDefine()
{
    assert(isTypeIdentifier(sym));

    symType symtype = transferKeyToType(sym.getKey());

    do
    {
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);

        string symname = sym.getValue();
        if (symbolist.hasNowSeg(symname))
        {
            errmag.errPut(sym.getLine(), errmag.B);
        }

        sym = lexical.getSym();
        if (sym.getKey() == LBRACK)
        {
            printToken(sym);

            sym = lexical.getSym();
            int len = unsignedInteger();

            symAttr attr(symname, symtype, symKind::VAR, len);
            symbolist.insert(attr);
            midcode.genMidVarState(attr.name);

            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym();
                errmag.errPut(sym.getLine(), errmag.M);
                sym = lexical.getSym();
            }
            else
            {
                printToken(sym);
                sym = lexical.getSym();
            }
        }
        else
        {
            symAttr attr(symname, symtype, symKind::VAR);
            symbolist.insert(attr);
            midcode.genMidVarState(attr.name);
        }
    } while (sym.getKey() == COMMA);
    printLine("<变量定义>");
}

void syntacticAnalysis::funcWithReturn()
{
    string name = stateHead();
    funcWithRet.insert(name);

    symbolist.direct();

    assert(sym.getKey() == LPARENT);
    printToken(sym);
    sym = lexical.getSym();
    argumentList(name);
    midcode.genMidFuncDef(name);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym();
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<有返回值函数定义>");

    midcode.genMidFuncRet("");
    symbolist.redirect();
}

void syntacticAnalysis::funcWithoutReturn()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    funcWithoutRet.insert(sym.getValue());
    printToken(sym);

    string name = sym.getValue();

    if (symbolist.hasNowSeg(sym.getValue()))
    {
        errmag.errPut(sym.getLine(), errmag.B);
    }
    else
    {
        symAttr attr(sym.getValue(), VOID, FUNC);
        symbolist.insert(attr);
    }
    symbolist.direct();

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    argumentList(name);
    midcode.genMidFuncDef(name);

    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym();
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<无返回值函数定义>");

    midcode.genMidFuncRet("");
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

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    if (symbolist.hasNowSeg(sym.getValue()))
    {
        errmag.errPut(sym.getLine(), errmag.B);
    }
    else
    {
        symbolist.addArgsForNearFunc(sym.getValue(), arg1Type);
    }

    sym = lexical.getSym();
    while (sym.getKey() == COMMA)
    {
        printToken(sym);

        sym = lexical.getSym();
        assert(isTypeIdentifier(sym));
        printToken(sym);
        arg1Type = transferKeyToType(sym.getKey());

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (symbolist.hasNowSeg(sym.getValue()))
        {
            errmag.errPut(sym.getLine(), errmag.B);
        }
        else
        {
            symbolist.addArgsForNearFunc(sym.getValue(), arg1Type);
        }

        sym = lexical.getSym();
    }
    printLine("<参数表>");
}

void syntacticAnalysis::mainFunc()
{
    assert(sym.getKey() == VOIDTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == MAINTK);
    printToken(sym);

    if (symbolist.hasNowSeg(sym.getValue()))
    {
        errmag.errPut(sym.getLine(), errmag.B);
    }
    else
    {
        symAttr attr(sym.getValue(), VOID, FUNC);
        symbolist.insert(attr);
        midcode.genMidFuncDef(attr.name);
    }
    symbolist.direct();

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LBRACE);
    printToken(sym);

    sym = lexical.getSym();
    compoundStatement();
    assert(sym.getKey() == RBRACE);
    printToken(sym);
    printLine("<主函数>");
    midcode.genMidFuncRet("");
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
        token_key op = sym.getKey();
        printToken(sym);
        sym = lexical.getSym();
        ret = INT;
        termRet = term();
        expTmp = midcode.genMidConstTmp(INT, "0");
        expTmp = midcode.genMidExpress(expTmp, op, termRet.tmp4val);
        //CHEN: 临时变量
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
        token_key op = sym.getKey();
        printToken(sym);
        sym = lexical.getSym();
        termRet = term();
        expTmp = midcode.genMidExpress(expTmp, op, termRet.tmp4val);
        //CHEN: 临时变量
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
        token_key op = sym.getKey();
        printToken(sym);
        sym = lexical.getSym();
        expRet temp = factor();
        termTmpRet = midcode.genMidExpress(termTmpRet, op, temp.tmp4val);
        //CHEN: 临时变量
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
        if (lexical.peek().getKey() == LPARENT)
        {
            if (symbolist.has(sym.getValue()))
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            factorTmpRet = invokeFuncWithReturn();
        }
        else if (lexical.peek().getKey() == LBRACK)
        {
            if (!symbolist.has(sym.getValue()))
            {
                errmag.errPut(sym.getLine(), errmag.C);
            }
            else
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            printToken(sym);
            sym = lexical.getSym();
            printToken(sym);
            sym = lexical.getSym();
            expRet idxTmp = expression();
            symType idxType = idxTmp.type;
            if (idxType != INT)
            {
                errmag.errPut(sym.getLine(), errmag.I);
            }
            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym();
                errmag.errPut(sym.getLine(), errmag.M);
                sym = lexical.getSym();
            }
            else
            {
                printToken(sym);
                sym = lexical.getSym();
                factorTmpRet = midcode.genMidArrayValueGet(identifyName, idxTmp.tmp4val);
                //临时变量
            }
        }
        else
        {
            if (!symbolist.has(sym.getValue()))
            {
                errmag.errPut(sym.getLine(), errmag.C);
            }
            else
            {
                ret = symbolist.get(sym.getValue()).type;
                factorTmpRet = midcode.genMidValueGet(identifyName);
                //CHEN: 临时变量
            }
            printToken(sym);
            sym = lexical.getSym();
        }
    }
    else if (sym.getKey() == LPARENT)
    {
        printToken(sym);

        sym = lexical.getSym();
        factorTmpRet = expression().tmp4val;
        factorTmpRet = midcode.genMidToINT(factorTmpRet);
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.L);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        ret = INT;
    }
    else if (isCharacter(sym))
    {
        char charValue = character();
        stringstream ss;
        ss << charValue;
        factorTmpRet = midcode.genMidConstTmp(CHAR, ss.str());
        //CHEN: 临时变量
        ret = CHAR;
    }
    else if (isAddOp(sym) || sym.getKey() == INTCON)
    {
        int intValue = integer();
        stringstream ss;
        ss << intValue;
        factorTmpRet = midcode.genMidConstTmp(INT, ss.str());
        //CHEN: 临时变量
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
        sym = lexical.getSym();
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
        sym = lexical.getSym();
        statementList();
        assert(sym.getKey() == RBRACE);
        printToken(sym);

        sym = lexical.getSym();
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
            else if (lexical.peek().getKey() == ASSIGN || lexical.peek().getKey() == LBRACK)
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
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.K);
            sym = lexical.getSym();
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym();
        }
    }
    printLine("<语句>");
}

void syntacticAnalysis::conditionalStatement()
{
    assert(sym.getKey() == IFTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    condition();
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }
    string label_not = midcode.genMid_AllocLabel();
    midcode.genMidBZ(label_not);
    sym = lexical.getSym();
    statement();
    if (sym.getKey() == ELSETK)
    {
        /*if后面有else语句*/
        string label_out = midcode.genMid_AllocLabel();
        midcode.genMidGoto(label_out);
        midcode.genMidLabelLine(label_not);
        printToken(sym);
        sym = lexical.getSym();
        statement();
        midcode.genMidLabelLine(label_out);
    }
    else
    {
        /*if后面没有else语句*/
        midcode.genMidLabelLine(label_not);
    }

    printLine("<条件语句>");
}

void syntacticAnalysis::assignmentStatement()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string leftvalue = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        errmag.errPut(sym.getLine(), errmag.C);
    }
    else
    {
        symAttr findRst = symbolist.get(sym.getValue());
        if (findRst.kind == CONST)
        {
            errmag.errPut(sym.getLine(), errmag.J);
        }
    }

    sym = lexical.getSym();
    if (sym.getKey() == LBRACK)
    {
        printToken(sym);

        sym = lexical.getSym();
        expRet expret = expression();
        symType idxType = expret.type;
        if (idxType != INT)
        {
            errmag.errPut(sym.getLine(), errmag.I);
        }
        // assert(sym.getKey() == RBRACK);
        if (sym.getKey() != RBRACK)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.M);
            sym = lexical.getSym();
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym();
        }

        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        expRet expret2 = expression();
        midcode.genMidArrayValuePut(leftvalue, expret.tmp4val, expret2.tmp4val);
    }
    else
    {
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();

        expRet expret2 = expression();
        midcode.genMidValuePut(leftvalue, expret2.tmp4val);
    }

    printLine("<赋值语句>");
}

void syntacticAnalysis::condition()
{
    expRet expret = expression();
    symType symtype = expret.type;
    if (symtype != INT)
    {
        errmag.errPut(sym.getLine(), errmag.F);
    }
    if (isRelOp(sym))
    {
        token_key op = sym.getKey();
        printToken(sym);
        sym = lexical.getSym();
        expRet expret2 = expression();
        symtype = expret2.type;
        if (symtype != INT)
        {
            errmag.errPut(sym.getLine(), errmag.F);
        }
        midcode.genMidCondition(expret.tmp4val, op, expret2.tmp4val);
    }
    else
    {
        midcode.genMidCondition4Num(expret.tmp4val);
    }
    printLine("<条件>");
}

void syntacticAnalysis::loopStatement()
{
    if (sym.getKey() == WHILETK)
    {
        string label_in = midcode.genMid_AllocLabel();
        string label_out = midcode.genMid_AllocLabel();
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        midcode.genMidLabelLine(label_in);
        sym = lexical.getSym();
        condition();
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.L);
        }
        else
        {
            printToken(sym);
        }
        midcode.genMidBZ(label_out);

        sym = lexical.getSym();
        statement();
        midcode.genMidGoto(label_in);
        midcode.genMidLabelLine(label_out);
    }
    else if (sym.getKey() == DOTK)
    {
        printToken(sym);
        sym = lexical.getSym();
        string label_loopBegin = midcode.genMid_AllocLabel();
        midcode.genMidLabelLine(label_loopBegin);
        statement();
        // assert(sym.getKey() == WHILETK);
        if (sym.getKey() != WHILETK)
        {
            errmag.errPut(sym.getLine(), errmag.N);
            while (sym.getKey() != SEMICN)
            {
                sym = lexical.getSym();
            }
            return;
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym();
        condition();
        midcode.genMidBNZ(label_loopBegin);
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.L);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
    }
    else if (sym.getKey() == FORTK)
    {
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == LPARENT);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify1_name = sym.getValue();
        if (!symbolist.has(sym.getValue()))
        {
            errmag.errPut(sym.getLine(), errmag.C);
        }
        else
        {
            symAttr findRst = symbolist.get(sym.getValue());
            if (findRst.kind == CONST)
            {
                errmag.errPut(sym.getLine(), errmag.J);
            }
        }

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        expRet expret_for_begin = expression();
        midcode.genMidValuePut(identify1_name, expret_for_begin.tmp4val);

        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.K);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        string label_in = midcode.genMid_AllocLabel();
        string label_out = midcode.genMid_AllocLabel();
        midcode.genMidLabelLine(label_in);
        condition();
        midcode.genMidBZ(label_out);
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.K);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify2_name = sym.getValue();
        if (!symbolist.has(sym.getValue()))
        {
            errmag.errPut(sym.getLine(), errmag.C);
        }
        else
        {
            symAttr findRst = symbolist.get(sym.getValue());
            if (findRst.kind == CONST)
            {
                errmag.errPut(sym.getLine(), errmag.J);
            }
        }

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        string identify3_name = sym.getValue();

        if (!symbolist.has(sym.getValue()))
        {
            errmag.errPut(sym.getLine(), errmag.C);
        }

        sym = lexical.getSym();
        assert(isAddOp(sym));
        printToken(sym);
        token_key op4 = sym.getKey();

        sym = lexical.getSym();
        int value = stepLength();
        stringstream ss;
        ss << value;
        string value5 = ss.str();

        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.L);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        statement();
        string step_tmp = midcode.genMidConstTmp(INT, value5);
        string erpress_end = midcode.genMidExpress(identify3_name, op4, step_tmp);
        //CHEN: 临时变量

        midcode.genMidValuePut(identify2_name, erpress_end);
        midcode.genMidGoto(label_in);
        midcode.genMidLabelLine(label_out);
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
        errmag.errPut(sym.getLine(), errmag.C);
        while (sym.getKey() != RPARENT)
        {
            sym = lexical.getSym();
        }
        sym = lexical.getSym();
        return "";
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    vector<string> argsShouldPush = valueArgumentList(funcName);
    for (int i = 0; i < argsShouldPush.size(); i++)
    {
        midcode.genMidArgsPush(argsShouldPush[i]);
    }

    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    printLine("<有返回值函数调用语句>");
    midcode.genMidFuncCall(funcName);
    string ret = midcode.genMidFuncRetUse(funcName);
    //CHEN: 临时变量
    return ret;
}

void syntacticAnalysis::invokeFuncWithoutReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string funcName = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        errmag.errPut(sym.getLine(), errmag.C);
        while (sym.getKey() != RPARENT)
        {
            sym = lexical.getSym();
        }
        sym = lexical.getSym();
        return;
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    vector<string> argsShouldPush = valueArgumentList(funcName);
    for (int i = 0; i < argsShouldPush.size(); i++)
    {
        midcode.genMidArgsPush(argsShouldPush[i]);
    }
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    printLine("<无返回值函数调用语句>");
    midcode.genMidFuncCall(funcName);
}

// 注:此处使用了外部信息,即参数表后必须有")"
vector<string> syntacticAnalysis::valueArgumentList(string funcName)
{
    vector<string> argsShouldPush;
    symAttr funcAttr = symbolist.get(funcName);
    // cout << "funcAttr.name:\t" << funcAttr.name << " args size:\t" << funcAttr.getArgs().size() << endl;
    vector<symType> argsTypes = funcAttr.getArgs();
    int count = 0;
    if (argsTypes.size() == 0 && sym.getKey() != RPARENT)
    {
        errmag.errPut(sym.getLine(), errmag.L);
    }
    if (sym.getKey() == RPARENT)
    {
        if (count != argsTypes.size())
        {
            errmag.errPut(sym.getLine(), errmag.D);
        }
        printLine("<值参数表>");
        return argsShouldPush;
    }
    expRet expret = expression();
    argsShouldPush.push_back(expret.tmp4val);
    symType symtype = expret.type;
    if (count < argsTypes.size() && argsTypes[count] != symtype)
    {
        errmag.errPut(sym.getLine(), errmag.E);
    }
    count++;
    while (sym.getKey() == COMMA)
    {
        printToken(sym);
        sym = lexical.getSym();
        expRet expret = expression();
        argsShouldPush.push_back(expret.tmp4val);
        symtype = expret.type;

        if (count < argsTypes.size() && argsTypes[count] != symtype)
        {
            errmag.errPut(sym.getLine(), errmag.E);
        }
        count++;
    }
    if (count != argsTypes.size())
    {
        errmag.errPut(sym.getLine(), errmag.D);
    }
    printLine("<值参数表>");
    return argsShouldPush;
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

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);

    do
    {
        printToken(sym);
        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (!symbolist.has(sym.getValue()))
        {
            errmag.errPut(sym.getLine(), errmag.C);
        }
        else
        {
            midcode.genMidScanf(sym.getValue());
        }
        sym = lexical.getSym();
    } while (sym.getKey() == COMMA);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    printLine("<读语句>");
}

void syntacticAnalysis::writeStatement()
{
    assert(sym.getKey() == PRINTFTK);
    printToken(sym);

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    if (sym.getKey() == STRCON)
    {
        string str4print = sym.getValue();
        strConCheck();
        if (sym.getKey() == COMMA)
        {
            printToken(sym);
            sym = lexical.getSym();
            expRet expret = expression();
            midcode.genMidPrintfStrNoNewLine(str4print);
            midcode.genMidPrintfExp(expret.tmp4val);
        }
        else
        {
            midcode.genMidPrintfStr(str4print);
        }
    }
    else
    {
        expRet expret = expression();
        midcode.genMidPrintfExp(expret.tmp4val);
    }
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        errmag.errPut(sym.getLine(), errmag.L);
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
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
    sym = lexical.getSym();
    if (sym.getKey() == LPARENT)
    {
        printToken(sym);
        sym = lexical.getSym();
        expRet expret = expression();
        midcode.genMidFuncRet(expret.tmp4val);
        getType = expret.type;
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            errmag.errPut(sym.getLine(), errmag.L);
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
    }
    else
    {
        midcode.genMidFuncRet("");
    }

    if (chkType != getType)
    {
        if (chkType == VOID)
        {
            errmag.errPut(sym.getLine(), errmag.G);
        }
        else
        {
            errmag.errPut(sym.getLine(), errmag.H);
        }
    }
    printLine("<返回语句>");
}