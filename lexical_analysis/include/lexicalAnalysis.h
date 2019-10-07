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
    list<token>::iterator pivot; /*指向下一次get时应该return出去的元素.若指向end()表示需要解析了.*/
    token genSym();
    token_key checkReservedWord(string s);

public:
    lexicalAnalysis(string filename);
    bool hasSym();
    token getSym();
    void unGetSym();
};
#endif