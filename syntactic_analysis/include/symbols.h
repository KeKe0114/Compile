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
    VOID,
    INT,
    CHAR,
    STRING,
};

class symAttr
{
public:
    string name;
    symType type;
    symKind kind;
    int dim;

    void SHOW_ATTR()
    {
        // cout << name << "\t" << type << "\t" << kind << endl;
        printf("%10s\t%d\t%d\t%d\n", name.c_str(), type, kind, dim);
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
    symAttr get(string name);
    symAttr getNowSeg(string name);

    void DEBUG_PRINT_LIST();
};

#endif