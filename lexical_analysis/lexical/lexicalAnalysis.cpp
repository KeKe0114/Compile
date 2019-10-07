#include <sstream>
#include <cctype>
#include "lexicalAnalysis.h"

using namespace std;

token_key lexicalAnalysis::checkReservedWord(string s)
{
    int ret = reverseWord.find(s);
    return token_key(ret);
}

lexicalAnalysis::lexicalAnalysis(string filename)
{
    in.open(filename);
    pivot = symbolics.begin();
    for (int i = RESERVED_BEGIN; i < RESERVED_END; i++)
    {
        reverseWord.insert(TOKEN_VALUE[i], i);
    }
}

bool lexicalAnalysis::hasSym()
{
    if (pivot != symbolics.end())
    {
        return true;
    }
    char c = in.peek();
    while (isspace(c))
    {
        c = in.get();
        c = in.peek();
    }
    return !(in.peek() == EOF || in.eof());
}

token lexicalAnalysis::genSym()
{
    stringstream ss;
    string value;
    char c = in.get();

    // SPACE
    while (isspace(c))
    {
        c = in.get();
    }

    // IDENFR and real RESERVED_WORD
    if (isalpha(c) || c == '_')
    {
        while (isalpha(c) || c == '_' || isdigit(c))
        {
            ss << c;
            c = in.get();
        }
        in.unget();
        value = ss.str();
        token_key key = checkReservedWord(value);
        if (key)
        {
            return token(key);
        }
        return token(key, value);
    }

    // DIGIT
    else if (isdigit(c))
    {
        while (isdigit(c))
        {
            ss << c;
            c = in.get();
        }
        in.unget();
        value = ss.str();
        return token(INTCON, value);
    }

    // '' CHARCON
    else if (c == '\'')
    {
        c = in.get();
        while (c != '\'')
        {
            ss << c;
            c = in.get();
        }
        value = ss.str();
        return token(CHARCON, value);
    }

    // "" STRCON
    else if (c == '"')
    {
        c = in.get();
        while (c != '"')
        {
            ss << c;
            c = in.get();
        }
        value = ss.str();
        return token(STRCON, value);
    }

    //具体请查看token.h
    else if (isOpPrefix(c))
    {
        ss << c;
        c = in.peek();
        if (isOpSecond(c))
        {
            c = in.get();
            ss << c;
        }
        value = ss.str();
        token_key key = checkReservedWord(value);
        assert(key != IDENFR); /*find nothing*/
        return token(key);
    }

    //具体请查看token.h
    else if (isSingle(c))
    {
        ss << c;
        value = ss.str();
        token_key key = checkReservedWord(value);
        assert(key != IDENFR); /*find nothing*/
        return token(key);
    }

    // 下面的部分不可能被执行,除非有bug
    assert(false);
    return token((token_key)0, "ERROR");
}

token lexicalAnalysis::getSym()
{
    if (pivot != symbolics.end())
    {
        return *pivot++;
    }
    token tk = genSym();
    DEBUG(1, "genSym()::token::\t" + tk.getName() + "\t" + tk.getValue());
    symbolics.push_back(tk);
    pivot++;
    return tk;
}

void lexicalAnalysis::unGetSym()
{
    assert(pivot != symbolics.begin()); /*BUG outside:: reach the begin of the Syms but call the lexicalAnalysis::unGetSym()*/
    if (pivot != symbolics.begin())
    {
        pivot--;
    }
}
