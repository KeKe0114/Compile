#include "syntacticAnalysis.h"

void ERROR_PRINT(int line, string err_code)
{
    cout << line << " " << err_code << endl;
}

inline symType transferKeyToType(token_key key)
{
    if (key == INTTK)
        return INT;
    else if (key == CHARTK)
        return CHAR;
    assert(false); /* 如果执行到此处说明有bug */
}

syntacticAnalysis::syntacticAnalysis(string filename, string outfile) : lexical(filename), sym(lexical.getSym()), out(outfile)
{
}

void syntacticAnalysis::printToken(token key)
{
    cout << key.getName() << " " << key.getValue() << endl;
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
    printLine("<程序>");
    //assert(!lexical.hasSym()); /*main 函数后不应该有其他token*/
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
            ERROR_PRINT(sym.getLine(), "k");
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
                symbolist.DEBUG_PRINT_LIST();
                ERROR_PRINT(sym.getLine(), "b");
            }
            else
            {
                symAttr attr = {sym.getValue(), symType::INT, symKind::CONST};
                symbolist.insert(attr);
            }

            sym = lexical.getSym();
            assert(sym.getKey() == ASSIGN);
            printToken(sym);

            sym = lexical.getSym();
            integer();
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
                ERROR_PRINT(sym.getLine(), "b");
            }
            else
            {
                symAttr attr = {sym.getValue(), symType::CHAR, symKind::CONST};
                symbolist.insert(attr);
            }

            sym = lexical.getSym();
            assert(sym.getKey() == ASSIGN);
            printToken(sym);

            sym = lexical.getSym();
            character();
        } while (sym.getKey() == COMMA);
    }
    else
    {
        assert(false); /*得到的类型标识符不是int,char*/
    }
    printLine("<常量定义>");
}

void syntacticAnalysis::unsignedInteger()
{
    assert(sym.getKey() == INTCON);
    printToken(sym);

    sym = lexical.getSym();
    printLine("<无符号整数>");
}

void syntacticAnalysis::integer()
{
    if (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
    }
    unsignedInteger();
    printLine("<整数>");
}

void syntacticAnalysis::character()
{
    assert(isCharacter(sym));
    printToken(sym);
    sym = lexical.getSym();
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
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {name, symtype, FUNC};
        symbolist.insert(attr);
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
            ERROR_PRINT(sym.getLine(), "k");
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
            ERROR_PRINT(sym.getLine(), "b");
        }

        sym = lexical.getSym();
        if (sym.getKey() == LBRACK)
        {
            printToken(sym);

            sym = lexical.getSym();
            unsignedInteger();

            //TODO: 数组类型和相应的属性
            symAttr attr = {symname, symtype, symKind::VAR, 1};
            symbolist.insert(attr);

            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym();
                ERROR_PRINT(sym.getLine(), "m");
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
            symAttr attr = {symname, symtype, symKind::VAR};
            symbolist.insert(attr);
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
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
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

    string name = sym.getName();

    if (symbolist.hasNowSeg(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), VOID, FUNC};
        symbolist.insert(attr);
    }
    symbolist.direct();

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    argumentList(name);

    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
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

    sym = lexical.getSym();
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    if (symbolist.hasNowSeg(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), VOID, FUNC};
        symbolist.insert(attr);
        symAttr funcAttr = symbolist.get(funcName);
        funcAttr.addArgs(transferKeyToType(sym.getKey()));
    }

    sym = lexical.getSym();
    while (sym.getKey() == COMMA)
    {
        printToken(sym);

        sym = lexical.getSym();
        assert(isTypeIdentifier(sym));
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (symbolist.hasNowSeg(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "b");
        }
        else
        {
            symAttr attr = {sym.getValue(), VOID, FUNC};
            symbolist.insert(attr);
            symAttr funcAttr = symbolist.get(funcName);
            funcAttr.addArgs(transferKeyToType(sym.getKey()));
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
        ERROR_PRINT(sym.getLine(), "b");
    }
    else
    {
        symAttr attr = {sym.getValue(), VOID, FUNC};
        symbolist.insert(attr);
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
        ERROR_PRINT(sym.getLine(), "l");
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

    symbolist.redirect();
}

symType syntacticAnalysis::expression()
{
    symType ret = ERROR;
    if (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        ret = INT;
    }
    symType symType1 = term();
    if (ret != INT)
    {
        ret = symType1;
    }
    if (isAddOp(sym))
    {
        ret = INT;
    }
    while (isAddOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        term();
    }
    printLine("<表达式>");
    return ret;
}

symType syntacticAnalysis::term()
{
    symType ret = factor();
    if (isMulOp(sym))
    {
        ret = INT;
    }
    while (isMulOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        factor();
    }
    printLine("<项>");
    return ret;
}

symType syntacticAnalysis::factor()
{
    symType ret = ERROR;
    if (sym.getKey() == IDENFR)
    {
        if (lexical.peek().getKey() == LPARENT)
        {
            if (symbolist.has(sym.getValue()))
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            invokeFuncWithReturn();
        }
        else if (lexical.peek().getKey() == LBRACK)
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
            sym = lexical.getSym();
            printToken(sym);
            sym = lexical.getSym();
            symType idxType = expression();
            if (idxType != INT)
            {
                ERROR_PRINT(sym.getLine(), "i");
            }
            // assert(sym.getKey() == RBRACK);
            if (sym.getKey() != RBRACK)
            {
                lexical.unGetSym();
                lexical.unGetSym();
                sym = lexical.getSym();
                ERROR_PRINT(sym.getLine(), "m");
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
            if (!symbolist.has(sym.getValue()))
            {
                ERROR_PRINT(sym.getLine(), "c");
            }
            else
            {
                ret = symbolist.get(sym.getValue()).type;
            }
            printToken(sym);
            sym = lexical.getSym();
        }
    }
    else if (sym.getKey() == LPARENT)
    {
        printToken(sym);

        sym = lexical.getSym();
        expression();
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "l");
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
        character();
        ret = CHAR;
    }
    else if (isAddOp(sym) || sym.getKey() == INTCON)
    {
        integer();
        ret = INT;
    }
    else
    {
        assert(false);
    }
    printLine("<因子>");
    return ret;
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
            ERROR_PRINT(sym.getLine(), "k");
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
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    statement();
    if (sym.getKey() == ELSETK)
    {
        printToken(sym);
        sym = lexical.getSym();
        statement();
    }
    printLine("<条件语句>");
}

void syntacticAnalysis::assignmentStatement()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
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

    sym = lexical.getSym();
    if (sym.getKey() == LBRACK)
    {
        int lbkLine = sym.getLine();
        printToken(sym);

        sym = lexical.getSym();
        symType idxType = expression();
        if (idxType != INT)
        {
            ERROR_PRINT(sym.getLine(), "i");
        }
        // assert(sym.getKey() == RBRACK);
        if (sym.getKey() != RBRACK)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "m");
            sym = lexical.getSym();
        }
        else
        {
            printToken(sym);
            sym = lexical.getSym();
        }
    }
    assert(sym.getKey() == ASSIGN);
    printToken(sym);

    sym = lexical.getSym();
    expression();
    printLine("<赋值语句>");
}

void syntacticAnalysis::condition()
{
    symType symtype = expression();
    if (symtype != INT)
    {
        ERROR_PRINT(sym.getLine(), "f");
    }
    if (isRelOp(sym))
    {
        printToken(sym);
        sym = lexical.getSym();
        symtype = expression();
        if (symtype != INT)
        {
            ERROR_PRINT(sym.getLine(), "f");
        }
    }
    printLine("<条件>");
}

void syntacticAnalysis::loopStatement()
{
    if (sym.getKey() == WHILETK)
    {
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
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        statement();
    }
    else if (sym.getKey() == DOTK)
    {
        printToken(sym);
        sym = lexical.getSym();
        statement();
        assert(sym.getKey() == WHILETK);
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
            ERROR_PRINT(sym.getLine(), "l");
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

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        expression();
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "k");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        condition();
        // assert(sym.getKey() == SEMICN);
        if (sym.getKey() != SEMICN)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "k");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
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

        sym = lexical.getSym();
        assert(sym.getKey() == ASSIGN);
        printToken(sym);

        sym = lexical.getSym();
        assert(sym.getKey() == IDENFR);
        printToken(sym);
        if (!symbolist.has(sym.getValue()))
        {
            ERROR_PRINT(sym.getLine(), "c");
        }

        sym = lexical.getSym();
        assert(isAddOp(sym));
        printToken(sym);

        sym = lexical.getSym();
        stepLength();

        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
        statement();
    }
    else
    {
        assert(false);
    }
    printLine("<循环语句>");
}

void syntacticAnalysis::stepLength()
{
    unsignedInteger();
    printLine("<步长>");
}

void syntacticAnalysis::invokeFuncWithReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string funcName = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "c");
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    valueArgumentList(funcName);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    printLine("<有返回值函数调用语句>");
}

void syntacticAnalysis::invokeFuncWithoutReturn()
{
    assert(sym.getKey() == IDENFR);
    printToken(sym);
    string funcName = sym.getValue();
    if (!symbolist.has(sym.getValue()))
    {
        ERROR_PRINT(sym.getLine(), "c");
    }

    sym = lexical.getSym();
    assert(sym.getKey() == LPARENT);
    printToken(sym);

    sym = lexical.getSym();
    valueArgumentList(funcName);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
    }
    else
    {
        printToken(sym);
    }

    sym = lexical.getSym();
    printLine("<无返回值函数调用语句>");
}

// 注:此处使用了外部信息,即参数表后必须有")"
void syntacticAnalysis::valueArgumentList(string funcName)
{
    symAttr funcAttr = symbolist.get(funcName);
    vector<symType> argsTypes = funcAttr.getArgs();
    int count = 0;
    if (argsTypes.size == 0 && sym.getKey() != RPARENT)
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
    symType symtype = expression();
    if (argsTypes[count] != symtype)
    {
        ERROR_PRINT(sym.getLine(), "e");
    }
    count++;
    while (sym.getKey() == COMMA)
    {
        printToken(sym);
        sym = lexical.getSym();
        symtype = expression();
        if (argsTypes[count] != symtype)
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
            ERROR_PRINT(sym.getLine(), "c");
        }

        sym = lexical.getSym();
    } while (sym.getKey() == COMMA);
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
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
        strConCheck();
        if (sym.getKey() == COMMA)
        {
            printToken(sym);
            sym = lexical.getSym();
            expression();
        }
    }
    else
    {
        expression();
    }
    // assert(sym.getKey() == RPARENT);
    if (sym.getKey() != RPARENT)
    {
        lexical.unGetSym();
        lexical.unGetSym();
        sym = lexical.getSym();
        ERROR_PRINT(sym.getLine(), "l");
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
    symType chkType = ERROR;
    symType getType = VOID;
    if (!symbolist.hasNearFunc())
    {
        DEBUG(1, "NO FUNC Near");
    }
    else
    {
        chkType = symbolist.getNearFunc().type;
    }
    sym = lexical.getSym();
    if (sym.getKey() == LPARENT)
    {
        printToken(sym);
        sym = lexical.getSym();
        getType = expression();
        // assert(sym.getKey() == RPARENT);
        if (sym.getKey() != RPARENT)
        {
            lexical.unGetSym();
            lexical.unGetSym();
            sym = lexical.getSym();
            ERROR_PRINT(sym.getLine(), "l");
        }
        else
        {
            printToken(sym);
        }

        sym = lexical.getSym();
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
    syntacticAnalysis syntactic("testfile.txt", "output.txt");
    syntactic.procedureCheck();
    return 0;
}
