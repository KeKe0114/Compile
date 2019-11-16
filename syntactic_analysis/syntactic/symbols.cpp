#include "symbols.h"

symbols::symbols()
{
    idGen = 0;
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

bool symbols::has(string name)
{
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        if (symStack[i].name == name)
        {
            return true;
        }
    }
    return false;
}

symAttr symbols::get(string name)
{
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        if (symStack[i].name == name)
        {
            return symStack[i];
        }
    }
}

bool symbols::hasNowSeg(string name)
{
    for (int i = symStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        if (symStack[i].name == name)
        {
            return true;
        }
    }
    return false;
}

symAttr symbols::getNowSeg(string name)
{
    for (int i = symStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        if (symStack[i].name == name)
        {
            return symStack[i];
        }
    }
}

void symbols::insert(symAttr item)
{
    item.SymId = idGen++;
    id2sym[item.SymId] = item;
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
        std::cout << i << " : ";
        symStack[i].SHOW_ATTR();
    }
    printf("*********************************\n");
}

symAttr symbols::getNearFunc()
{
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        if (symStack[i].kind == FUNC)
        {
            return symStack[i];
        }
    }
}

bool symbols::hasNearFunc()
{
    for (int i = symStack.size() - 1; i >= 0; i--)
    {
        if (symStack[i].kind == FUNC)
        {
            return true;
        }
    }
    return false;
}

// int main(int argc, char const *argv[])
// {
//     symbols symbols;
//     symAttr attr = {"cheney", STRING, CONST};
//     symbols.insert(attr);
//     symbols.direct();
//     symAttr attr2 = {"num", INT, VAR};
//     symbols.insert(attr2);

//     symAttr ans = symbols.get("cheney");

//     symbols.DEBUG_PRINT_LIST();
//     return 0;
// }
