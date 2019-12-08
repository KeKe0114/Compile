#include "tempRegMag.h"
#include "debug.h"
void tempRegMag::resetLocalPool()
{
    symId2reg.clear();
    usedReg.clear();
}

bool tempRegMag::hasFreeReg()
{
    return usedReg.size() < maxReg - minReg + 1;
}

int tempRegMag::getAFreeRegForThis(int name)
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
    symId2reg.insert(pair<int, int>(name, regFree));
    return regFree;
}

bool tempRegMag::hasThisInReg(int name)
{
    return symId2reg.find(name) != symId2reg.end();
}

int tempRegMag::getRegForThis(int name)
{
    assert(hasThisInReg(name));
    map<int, int>::iterator iter;
    iter = symId2reg.find(name);
    return iter->second;
}

void tempRegMag::updataUsefulInfo(set<int> Useful)
{
    set<int> shouldRemove;

    map<int, int>::iterator iter;
    for (iter = symId2reg.begin(); iter != symId2reg.end(); iter++)
    {
        if (Useful.find(iter->first) == Useful.end())
        {
            shouldRemove.insert(iter->first);
            usedReg.erase(iter->second);
        }
    }
    set<int>::iterator itst;
    for (itst = shouldRemove.begin(); itst != shouldRemove.end(); itst++)
    {
        symId2reg.erase(*itst);
    }
}