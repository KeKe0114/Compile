#include "symbols.h"

symbols::symbols()
{
    symAttr global_data = {"GLOBAL_DATAES", VOID, CONST};
    insert(global_data);
}

void symbols::direct()
{
    indexes.push_back(idStack.size());
}

void symbols::redirect()
{
    int totSizeNow = 0;
    offsetRefer referNow;
    if (indexes.size() == 1)
    {
        referNow = GLOBAL;
    }
    else
    {
        referNow = FP;
        totSizeNow += 4 * 4; /*$31, ret_value, $fp, $sp*/
    }
    for (int i = indexes[indexes.size() - 1]; i <= idStack.size() - 1; i++)
    {
        if (id2sym[idStack[i]].kind == VAR || id2sym[idStack[i]].kind == CONST)
        {
            id2sym[idStack[i]].refer = referNow;
            id2sym[idStack[i]].offsetRel = totSizeNow;
            totSizeNow += id2sym[idStack[i]].size;
        }
    }
    for (int i = idStack.size() - 1; i >= indexes[indexes.size() - 1]; i--)
    {
        idStack.pop_back();
    }
    id2sym[idStack[idStack.size() - 1]].size = totSizeNow;
    indexes.pop_back();
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

symAttr *symbols::get_pointer(string name)
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return &id2sym[idStack[i]];
        }
    }
}

symAttr *symbols::get_pointer_by_id(int id)
{
    return &id2sym[id];
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
    item.SymId = id2sym.size();
    item.size = item.len == 0 ? 4 : item.len * 4;
    id2sym.push_back(item);
    idStack.push_back(item.SymId);
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

void symbols::addArgsForNearFunc(string name, symType arg)
{
    symAttr attr = {name, arg, VAR};
    insert(attr);
    int id = get(name).SymId;
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].kind == FUNC)
        {
            id2sym[idStack[i]].addArgs(id, arg);
            return;
        }
    }
}

void symAttr::addArgs(int id, symType arg)
{
    args.push_back(arg);
    argsId.push_back(id);
}

vector<symType> symAttr::getArgs()
{
    return args;
}

void symAttr::SHOW_ATTR()
{
    printf("%10s@%d\t%d\t%d\t%d\t", name.c_str(), SymId, type, kind, len);
    printf("%d\t%d\n", size, offsetRel);
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

void symbols::DEBUG_PRINT_ALL_SYM()
{
    for (int i = 0; i < id2sym.size(); i++)
    {
        id2sym[i].SHOW_ATTR();
    }
}