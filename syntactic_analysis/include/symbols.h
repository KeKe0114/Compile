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
    int value;
    vector<symType> args;

    void SHOW_ATTR()
    {
        printf("%10s@%d\t%d\t%d\t%d\t", name.c_str(), SymId, type, kind, len);
        printf("%d\t%d\n", size, offsetRel);
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
    vector<int> idStack;
    vector<symAttr> id2sym;

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
        for (int i = idStack.size() - 1; i >= 0; i--)
        {
            if (id2sym[idStack[i]].kind == FUNC)
            {
                id2sym[idStack[i]].addArgs(arg);
                return;
            }
        }
    }

    void DEBUG_PRINT_LIST();
    void DEBUG_PRINT_ALL_SYM()
    {
        for (int i = 0; i < id2sym.size(); i++)
        {
            id2sym[i].SHOW_ATTR();
        }
    }
};

#endif