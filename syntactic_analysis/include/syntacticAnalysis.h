#ifndef SYNTACTIC_ANALYSIS_H
#define SYNTACTIC_ANALYSIS_H
#include "debug.h"
#include "lexicalAnalysis.h"
#include "symbols.h"
#include <set>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const set<token_key> typeIdentifier = {INTTK, CHARTK};
const set<token_key> addOp = {PLUS, MINU};
const set<token_key> mulOp = {MULT, DIV};
const set<token_key> relOp = {LSS, LEQ, GRE, GEQ, EQL, NEQ};
const set<token_key> chars = {CHARCON};
const set<token_key> statementPrefix = {SEMICN, IFTK, WHILETK, FORTK, DOTK, LBRACE, IDENFR, SCANFTK, RETURNTK, PRINTFTK};

struct expRet
{
    symType type;
    string tmp4val;
};

class syntacticAnalysis
{
private:
    symbols symbolist;
    lexicalAnalysis lexical;
    token sym;

    set<string> funcWithRet;
    set<string> funcWithoutRet;
    ofstream out;
    ofstream midFile;
    int label_idx;
    int temp_idx;

    void ERROR_PRINT(int line, string err_code);
    void printToken(token key);
    void printLine(string s);

    bool isAddOp(token key);
    bool isMulOp(token key);
    bool isRelOp(token key);
    bool isTypeIdentifier(token key);
    bool isFuncWithRet(token key);
    bool isStatementPrefix(token key);
    bool isFuncWithoutRet(token key);
    bool isCharacter(token key);

    void strConCheck();
    void constState();
    void constDefine();
    int unsignedInteger();
    int integer();
    char character();
    string stateHead();
    void variableState();
    void variableDefine();

    void funcWithReturn();
    void funcWithoutReturn();
    void compoundStatement();
    void argumentList(string funcName);
    void mainFunc();
    expRet expression();
    expRet term();
    expRet factor();
    void statement();
    void conditionalStatement();
    void assignmentStatement();
    void condition();
    void loopStatement();
    int stepLength();
    string invokeFuncWithReturn();
    void invokeFuncWithoutReturn();
    void valueArgumentList(string funcName);
    void statementList();
    void readStatement();
    void writeStatement();
    void returnStatement();

    /*中间代码生成*/
    void genMidFuncDef(string funcName);
    void genMidFuncPara(string paraName);

    void genMidArgsPush(string paraName); /*标识符或常量*/
    void genMidFuncCall(string func);
    void genMidFuncRet(string name);
    string genMidFuncRetUse();

    void genMidVarState(string name);
    void genMidConstState(string name);

    string genMid_AllocLabel();
    void genMidLabelLine(string Label);
    void genMid_ResetTmp();
    string genMid_AllocTmp();
    string genMidExpress(string operand1, string op, string operand2);
    string genMidValueGet(string name);
    void genMidValuePut(string name, string value);
    string genMidArrayValueGet(string array, string idx);
    void genMidArrayValuePut(string array, string idx, string value);

    void genMidCondition(string operand1, string op, string operand2);
    void genMidCondition4Num(string operand1);
    void genMidGoto(string Label);
    void genMidBNZ(string Label);
    void genMidBZ(string Label);

    void genMidScanf(string name);
    void genMidPrintfStr(string str);
    void genMidPrintfExp(symType type, string name);

    void genMidFuncLabel(string funcName);

public:
    syntacticAnalysis(string filename, string outfile, string midfile);
    void procedureCheck();
};
#endif