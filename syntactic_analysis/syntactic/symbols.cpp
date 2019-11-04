#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum symKind
{
    NOTFOUND = -1,
    CONST,
    VAR,
};

enum symType
{
    INT,
    CHAR,
    STRING,
    FUNC,
};

class symAttr
{
public:
    string name;
    symType type;
    symKind kind;

    void SHOW_ATTR()
    {
        // cout << name << "\t" << type << "\t" << kind << endl;
        printf("%15s\t%d\t%d\n", name.c_str(), type, kind);
    }
};

class symbols
{
private:
    vector<int> indexes;
    vector<symAttr> symStack;
    symAttr NOTFOUND = {"NOTFOUND", STRING, symKind::NOTFOUND};

public:
    symbols();
    void direct();
    void redirect();
    symAttr search(string name);
    symAttr searchNowSeg(string name);
    bool founded(symAttr searchRst);
    void insert(symAttr item);

    void DEBUG_PRINT_LIST();
};

symbols::symbols()
{
    direct();
}

void symbols::direct()
{
    indexes.push_back(symStack.size());
}

void symbols::redirect()
{
    for (int i = symStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        symStack.pop_back();
    }
    indexes.pop_back();
}

symAttr symbols::search(string name)
{
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        if (symStack[i].name == name)
        {
            return symStack[i];
        }
    }
    return NOTFOUND;
}

symAttr symbols::searchNowSeg(string name)
{
    for (int i = symStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        if (symStack[i].name == name)
        {
            return symStack[i];
        }
    }
    return NOTFOUND;
}

bool symbols::founded(symAttr item)
{
    return item.kind != symKind::NOTFOUND;
}

void symbols::insert(symAttr item)
{
    if (item.kind == symKind::NOTFOUND)
    {
        printf("insert item with NOTFOUND kind!");
        exit(0);
    }
    symStack.push_back(item);
}

void symbols::DEBUG_PRINT_LIST()
{

    printf("*********************************\n");
    printf("*******DEBUG INFO : SYMBOLS******\n");
    printf("*********************************\n");
    printf("indexes:\n\n");
    for (int i = indexes.size() - 1; i >= 0; i--)
    {
        printf("%5d\n", indexes[i]);
    }
    printf("symbol list:\n");
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        cout << i << " : ";
        symStack[i].SHOW_ATTR();
    }
    printf("*********************************\n");
}

// int main(int argc, char const *argv[])
// {
//     symbols symbols;
//     symAttr attr = {"cheney", STRING, CONST};
//     symbols.insert(attr);
//     symbols.direct();
//     symAttr attr2 = {"num", INT, VAR};
//     symbols.insert(attr2);

//     symAttr ans = symbols.search("cheney");
//     if (symbols.founded(ans))
//     {
//         cout << "found: ";
//         ans.SHOW_ATTR();
//     }

//     symbols.DEBUG_PRINT_LIST();

//     ans = symbols.searchNowSeg("cheney");
//     if (symbols.founded(ans))
//     {
//         cout << "found: ";
//         ans.SHOW_ATTR();
//     }

//     ans = symbols.searchNowSeg("num");
//     if (symbols.founded(ans))
//     {
//         cout << "found: ";
//         ans.SHOW_ATTR();
//     }

//     return 0;
// }
