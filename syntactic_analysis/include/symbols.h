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
    ERROR = -1,
    VOID,
    INT,
    CHAR,
    STRING,
    BOOL,
};

class symAttr
{
public:
    string name;
    symType type;
    symKind kind;
    int dim;
    vector<symType> args;

    void SHOW_ATTR()
    {
        // cout << name << "\t" << type << "\t" << kind << endl;
        printf("%10s\t%d\t%d\t%d\n", name.c_str(), type, kind, dim);
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
    vector<int> indexes;
    vector<symAttr> symStack;

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

    void DEBUG_PRINT_LIST();
};

#endif