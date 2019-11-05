#ifndef SYNTACTIC_ANALYSIS_H
#define SYNTACTIC_ANALYSIS_H
#include "debug.h"
#include "lexicalAnalysis.h"
#include "symbols.h"
#include <set>
#include <string>
#include <fstream>

using namespace std;

const set<token_key> typeIdentifier = {INTTK, CHARTK};
const set<token_key> addOp = {PLUS, MINU};
const set<token_key> mulOp = {MULT, DIV};
const set<token_key> relOp = {LSS, LEQ, GRE, GEQ, EQL, NEQ};
const set<token_key> chars = {CHARCON};
const set<token_key> statementPrefix = {SEMICN, IFTK, WHILETK, FORTK, DOTK, LBRACE, IDENFR, SCANFTK, RETURNTK, PRINTFTK};

class syntacticAnalysis
{
private:
    symbols symbolist;
    lexicalAnalysis lexical;
    token sym;

    set<string> funcWithRet;
    set<string> funcWithoutRet;
    ofstream out;

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
    void unsignedInteger();
    void integer();
    void character();
    string stateHead();
    void variableState();
    void variableDefine();

    void funcWithReturn();
    void funcWithoutReturn();
    void compoundStatement();
    void argumentList(string funcName);
    void mainFunc();
    void expression();
    void term();
    void factor();
    void statement();
    void conditionalStatement();
    void assignmentStatement();
    void condition();
    void loopStatement();
    void stepLength();
    void invokeFuncWithReturn();
    void invokeFuncWithoutReturn();
    void valueArgumentList(string funcName);
    void statementList();
    void readStatement();
    void writeStatement();
    void returnStatement();

public:
    syntacticAnalysis(string filename, string outfile);
    void procedureCheck();
};
#endif