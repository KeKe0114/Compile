#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include <fstream>
#include <string>
#include <list>

#include "token.h"
#include "trieTree.h"
#include "debug.h"
using namespace std;

class lexicalAnalysis
{
private:
    ifstream in;
    trieTree reverseWord;
    list<token> symbolics;
    token genSym();
    token_key checkReservedWord(string s);

public:
    lexicalAnalysis(string filename);
    bool hasSym();
    token getSym();
    list<token> getAllSym();
};
#endif