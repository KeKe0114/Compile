#include "symbols.h"

symbols::symbols()
{
    idGen = 0;
    direct();
}

void symbols::direct()
{
    indexes.push_back(idStack.size());
}

void symbols::redirect()
{
    int totSizeNow = 0;
    offsetRefer referNow;
    if (indexes[indexes.size() - 1] == 0)
    {
        referNow = GLOBAL;
        for (int i = idStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
        {
            id2sym[idStack[i]].refer = referNow;
            id2sym[idStack[i]].offsetRel = totSizeNow;
            totSizeNow += id2sym[idStack[i]].size;
            idStack.pop_back();
        }
        indexes.pop_back();
    }
    else
    {
        referNow = SP;
        for (int i = idStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
        {
            totSizeNow -= id2sym[idStack[i]].size;
            id2sym[idStack[i]].refer = referNow;
            id2sym[idStack[i]].offsetRel = totSizeNow;
            idStack.pop_back();
        }
        id2sym[idStack[idStack.size() - 1]].size = -(totSizeNow - 8); /*ret_val  sp*/
        id2sym[idStack[idStack.size() - 1]].offsetRel = totSizeNow - 8;
        id2sym[idStack[idStack.size() - 1]].refer = referNow;
        indexes.pop_back();
    }
}

bool symbols::has(string name)
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return true;
        }
    }
    return false;
}

symAttr symbols::get(string name)
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return id2sym[idStack[i]];
        }
    }
}

bool symbols::hasNowSeg(string name)
{
    for (int i = idStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return true;
        }
    }
    return false;
}

symAttr symbols::getNowSeg(string name)
{
    for (int i = idStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return id2sym[idStack[i]];
        }
    }
}

void symbols::insert(symAttr item)
{
    item.SymId = idGen++;
    item.size = item.len == 0 ? 4 : item.len * 4;
    id2sym.push_back(item);
    idStack.push_back(item.SymId);
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
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        std::cout << i << " : ";
        id2sym[idStack[i]].SHOW_ATTR();
    }
    printf("*********************************\n");
}

symAttr symbols::getNearFunc()
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].kind == FUNC)
        {
            return id2sym[idStack[i]];
        }
    }
}

bool symbols::hasNearFunc()
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].kind == FUNC)
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
