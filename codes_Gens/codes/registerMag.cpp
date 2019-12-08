#include "tempRegMag.h"
#include "debug.h"
void tempRegMag::resetLocalPool()
{
    name2reg.clear();
    usedReg.clear();
}

bool tempRegMag::hasFreeReg()
{
    return usedReg.size() < maxReg - minReg + 1;
}

int tempRegMag::getAFreeRegForThis(string name)
{
    assert(hasFreeReg());
    int regFree = -1;
    for (int i = minReg; i <= maxReg; i++)
    {
        if (usedReg.find(i) == usedReg.end())
        {
            regFree = i;
            break;
        }
    }
    usedReg.insert(regFree);
    name2reg.insert(pair<string, int>(name, regFree));
    return regFree;
}

bool tempRegMag::hasThisInReg(string name)
{
    return name2reg.find(name) != name2reg.end();
}

int tempRegMag::getRegForThis(string name)
{
    assert(hasThisInReg(name));
    map<string, int>::iterator iter;
    iter = name2reg.find(name);
    return iter->second;
}

void tempRegMag::updataUsefulInfo(set<string> Useful)
{
    set<string> shouldRemove;

    map<string, int>::iterator iter;
    for (iter = name2reg.begin(); iter != name2reg.end(); iter++)
    {
        if (Useful.find(iter->first) == Useful.end())
        {
            shouldRemove.insert(iter->first);
            usedReg.erase(iter->second);
        }
    }
    set<string>::iterator itst;
    for (itst = shouldRemove.begin(); itst != shouldRemove.end(); itst++)
    {
        name2reg.erase(*itst);
    }
}

string tempRegMag::randomFlushOneReg()
{
    assert(!hasFreeReg());
    map<string, int>::iterator it;
    string name = it->first;
    usedReg.erase(it->second);
    name2reg.erase(name);
    return name;
}