#pragma once
#include <set>
#include <map>
#include <vector>
#include "debug.h"

class funcScopeRegMag
{
private:
    bool alloced = false;
    int minReg = 0;
    int maxReg = 7;
    std::map<int, int> sym2reg;
    std::map<int, std::set<int>> reg2sym;
    std::map<int, std::set<int>> sym2conflict;
    std::set<int> allSyms;
    std::vector<int> regUsed;

private:
    int score(int symId);

public:
    // work
    // 完善冲突信息
    void addConflictFamily(std::set<int> family);

    // 离线计算: 分配寄存器
    void genAllocResult();

    // use
    //检查该变量是否可以使用全局寄存器的变量
    bool VarPassBlocks(int symId) { return allSyms.find(symId) != allSyms.end(); }

    // 检查可不可以为当前sym分配寄存器.
    bool hasRegForSym(int symId);

    // 如果可以,那么为当前sym分配寄存器.
    int getRegForSym(int symId);

    // 如果不可以,没办法了,那你用局部寄存器吧.并且告诉我你用了局部寄存器,方便出块儿的时候捞你上来.
    // 算了,你也不用告诉我了,你自己解决吧.

    // 返回所有当前函数需要使用的全局寄存器.
    std::vector<int> getAllRegsUseThisScope()
    {
        if (regUsed.empty())
        {
            for (auto item : sym2reg)
            {
                regUsed.push_back(item.second);
            }
        }
        return regUsed;
    }

public:
    void SHOW_ALL_SYM_NO_REG();
    void SHOW_ALL_SYM_HAS_REG();
};

class globalRegMag
{
private:
    globalRegMag() {}
    globalRegMag(const globalRegMag &) = delete;
    globalRegMag &operator&(const globalRegMag &) = delete;

public:
    static globalRegMag &get_instance()
    {
        static globalRegMag instance;
        return instance;
    }

private:
    std::map<std::string, int> regMagsId;
    std::vector<funcScopeRegMag> regMags;

public:
    void checkInToBeMag(std::string funcName, funcScopeRegMag regMag)
    {
        int id = regMags.size();
        regMags.push_back(regMag);
        regMagsId.insert(make_pair(funcName, id));
    }

    funcScopeRegMag getRegMag(std::string funcName)
    {
        if (regMagsId.find(funcName) != regMagsId.end())
        {
            int num = regMagsId.find(funcName)->second;
            return regMags[num];
        }
        assert(false);
        return regMags[0];
    }
};
