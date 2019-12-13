#include "globalRegMag.h"
#include "debug.h"
#include <vector>
#include <algorithm>

using namespace std;

int funcScopeRegMag::score(int symId)
{
    return sym2conflict[symId].size();
}

void funcScopeRegMag::addConflictFamily(std::set<int> family)
{
    assert(!alloced);
    allSyms.insert(family.begin(), family.end());
    for (auto member : family)
    {
        if (sym2conflict.find(member) == sym2conflict.end())
        {
            sym2conflict.insert(pair<int, set<int>>(member, set<int>()));
        }
    }
    for (auto member_i : family)
        for (auto member_j : family)
            if (member_i != member_j)
            {
                sym2conflict.find(member_i)->second.insert(member_j);
                sym2conflict.find(member_j)->second.insert(member_i);
            }
}

void funcScopeRegMag::genAllocResult()
{
    assert(!alloced);
    alloced = true;
    struct cmp
    {
        bool operator()(const pair<int, int> &p1, const pair<int, int> &p2)
        {
            return p1.second > p2.second;
        }
    };

    vector<pair<int, int>> vt;
    for (auto member : sym2conflict)
    {
        vt.push_back(pair<int, int>(member.first, score(member.first)));
    }
    sort(vt.begin(), vt.end(), cmp());

    for (int i = 0; i < vt.size(); i++)
    {
        vt[i].second = 1;
    }

    for (int i = minReg; i <= maxReg; i++)
    {
        set<int> iRegFamily;
        for (int j = 0; j < vt.size(); j++)
        {
            if (vt[j].second)
            {
                bool conflict = false;
                for (auto member : iRegFamily)
                {
                    if (sym2conflict[member].find(vt[j].first) != sym2conflict[member].end())
                    {
                        conflict = true;
                    }
                }
                if (!conflict)
                {
                    iRegFamily.insert(vt[j].first);
                    vt[j].second = 0;
                }
            }
        }

        for (auto member : iRegFamily)
        {
            sym2reg.insert(make_pair(member, i));
        }
        reg2sym.insert(make_pair(i, iRegFamily));
    }
}

bool funcScopeRegMag::hasRegForSym(int symId)
{
    assert(alloced);
    return sym2reg.find(symId) != sym2reg.end();
}

int funcScopeRegMag::getRegForSym(int symId)
{
    assert(alloced);
    assert(hasRegForSym(symId));
    return sym2reg.find(symId)->second;
}

void funcScopeRegMag::SHOW_ALL_SYM_NO_REG()
{
    assert(alloced);
    set<int> ans;
    for (auto member : allSyms)
    {
        if (sym2reg.find(member) == sym2reg.end())
        {
            ans.insert(member);
        }
    }
    cout << "SYMS_NO_REG :\t";
    for (auto member : allSyms)
    {
        cout << " " << member << " ";
    }
    cout << endl;
}
void funcScopeRegMag::SHOW_ALL_SYM_HAS_REG()
{
    assert(alloced);
    cout << "SYMS_HAS_REG:\t";
    for (auto item : reg2sym)
    {
        cout << "reg" << item.first << ":\t ";
        for (auto member : item.second)
        {
            cout << " " << member << " ";
        }
        cout << endl;
    }
}
