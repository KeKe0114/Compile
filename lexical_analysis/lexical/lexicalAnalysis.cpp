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
    line = 1;
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
        if (c == '\n')
        {
            line++;
        }
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
        if (c == '\n')
        {
            line++;
        }
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
            return token(key, line);
        }
        return token(key, value, line);
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
        if (value.size() > 1 && value[0] == '0')
        {
            cout << line << " a" << endl;
        }
        return token(INTCON, value, line);
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
        if (!(isalnum(value[0]) || value[0] == '_' || value[0] == '+' || value[0] == '-'))
        {
            cout << line << " a" << endl;
        }
        if (value.size() > 1)
        {
            cout << line << " a" << endl;
        }
        return token(CHARCON, value, line);
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
        for (int i = 0; i < value.size(); i++)
        {
            if (!((35 <= value[i] && value[i] <= 126) || value[i] == 32 || value[i] == 33))
            {
                cout << line << " a" << endl;
            }
        }

        return token(STRCON, value, line);
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
        return token(key, line);
    }

    //具体请查看token.h
    else if (isSingle(c))
    {
        ss << c;
        value = ss.str();
        token_key key = checkReservedWord(value);
        assert(key != IDENFR); /*find nothing*/
        return token(key, line);
    }

    // 下面的部分不可能被执行,除非有bug
    return token(ERROR, "ERROR", line);
}

token lexicalAnalysis::getSym()
{
    assert(hasSym());
    if (pivot != symbolics.end())
    {
        return *pivot++;
    }
    token tk = genSym();
    while (tk.getKey() == ERROR)
    {
        cout << tk.getLine() << " a" << endl;
        tk = genSym();
    }
    symbolics.push_back(tk);
    //pivot++;
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

token lexicalAnalysis::peek()
{
    assert(hasSym());
    if (pivot != symbolics.end())
    {
        return *pivot;
    }
    token tk = genSym();
    while (tk.getKey() == ERROR)
    {
        cout << tk.getLine() << " a" << endl;
        tk = genSym();
    }
    symbolics.push_back(tk);
    pivot--;
    return tk;
}
