#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum symKind
{
    CONST,
    VAR,
    FUNC,
};

enum symType
{
    TYPERROR = -1,
    VOID,
    INT,
    CHAR,
    STRING,
    BOOL,
};

enum offsetRefer
{
    GLOBAL,
    SP,
};

class symAttr
{
public:
    string name;
    symType type;
    symKind kind;
    int len; /*为0表示不是数组, 大于0表示是数组*/

    int size; /*表示该单元在内存中所占空间大小*/
    int SymId;
    int offsetRel;
    offsetRefer refer;
    vector<symType> args;

    void SHOW_ATTR()
    {
        printf("%10s\t%d\t%d\t%d\n", name.c_str(), type, kind, len);
    }

    void addArgs(symType arg)
    {
        args.push_back(arg);
    }

    vector<symType> getArgs()
    {
        return args;
    }
};

class symbols
{
private:
    int idGen;
    vector<int> indexes;
    vector<symAttr *> symStack;
    vector<symAttr *> id2sym;

public:
    symbols();
    void direct();
    void redirect();

    void insert(symAttr item);

    bool has(string name);
    bool hasNowSeg(string name);
    bool hasNearFunc();
    symAttr get(string name);
    symAttr getNowSeg(string name);
    symAttr getNearFunc();
    void addArgsForNearFunc(symType arg)
    {
        for (int i = symStack.size() - 1; i >= 0; i--)
        {
            if (symStack[i]->kind == FUNC)
            {
                symStack[i]->addArgs(arg);
                return;
            }
        }
    }

    void DEBUG_PRINT_LIST();
};

#endif