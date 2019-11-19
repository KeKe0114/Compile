#include "symbols.h"
#include "debug.h"

symbols::symbols()
{
    symAttr global_data("GLOBAL_DATAES", VOID, CONST);
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
    assert(false);
    symAttr attr("ERROR_get", STRING, CONST);
    return attr;
}

int symbols::get_id(string name)
{
    for (int i = idStack.size() - 1; i >= 0; i--)
    {
        if (id2sym[idStack[i]].name == name)
        {
            return id2sym[idStack[i]].SymId;
        }
    }
    cout << "DENUG:" << name << endl;
    assert(false);
    return -1;
}

symAttr symbols::get_by_id(int id)
{
    return id2sym[id];
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
    assert(false);
    symAttr attr("ERROR_get_now_seg", STRING, CONST);
    return attr;
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
    assert(false);
    symAttr attr("ERROR_get_func", STRING, CONST);
    return attr;
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
    symAttr attr(name, arg, VAR);
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
    string type_str;
    if (type == INT)
        type_str = "INT";
    else if (type == CHAR)
        type_str = "CHAR";
    else if (type == VOID)
        type_str = "VOID";
    else if (type == STRING)
        type_str = "STRING";
    else if (type == BOOL)
        type_str = "BOOL";
    else
        type_str = "TYPERROR";
    string kind_str;
    if (kind == CONST)
        kind_str = "CONST";
    else if (kind == VAR)
        kind_str = "VAR";
    else if (kind == FUNC)
        kind_str = "FUNC";
    else
        kind_str = "KINDERROR";
    string is_array = "NOT_ARRAY";
    if (len > 0)
    {
        is_array = to_string(len);
    }
    string refer_str = refer == GLOBAL ? "GLOBAL" : "FP";
    printf("%15s@%d\t%5s\t%5s\t%5s\t", name.c_str(), SymId, type_str.c_str(), kind_str.c_str(), is_array.c_str());
    printf("%d\t%d\t%6s\t%s\n", size, offsetRel, refer_str.c_str(), value.c_str());
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