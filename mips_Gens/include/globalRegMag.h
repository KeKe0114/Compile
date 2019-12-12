#pragma once
#include <set>
#include <map>

class globalRegMag
{
private:
    globalRegMag() : alloced(false) {}
    globalRegMag(const globalRegMag &) = delete;
    globalRegMag &operator&(const globalRegMag &) = delete;

public:
    static globalRegMag &get_instance()
    {
        static globalRegMag instance;
        return instance;
    }

private:
    bool alloced;
    int minReg = 0;
    int maxReg = 7;
    std::map<int, int> sym2reg;
    std::map<int, std::set<int>> reg2sym;
    std::map<int, std::set<int>> sym2conflict;
    std::set<int> allSyms;

private:
    int score(int symId);

public:
    // 完善冲突信息
    void addConflictFamily(std::set<int> family);

    // 离线计算: 分配寄存器
    void genAllocResult();

    // 检查可不可以为当前sym分配寄存器.
    bool hasRegForSym(int symId);

    // 如果可以,那么为当前sym分配寄存器.
    int getRegForSym(int symId);

    // 如果不可以,没办法了,那你用局部寄存器吧.并且告诉我你用了局部寄存器,方便出块儿的时候捞你上来.
    // 算了,你也不用告诉我了,你自己解决吧.

public:
    void SHOW_ALL_SYM_NO_REG();
    void SHOW_ALL_SYM_HAS_REG();
};
