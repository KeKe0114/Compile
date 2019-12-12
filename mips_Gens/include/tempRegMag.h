#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class flushSt
{
private:
    int symId;
    int RegId;

public:
    flushSt(int symId, int RegId);
    int getSymId();
    int getRegId();
};

class tempRegMag
{
private:
    tempRegMag() {}
    tempRegMag(const tempRegMag &) = delete;
    tempRegMag &operator&(const tempRegMag &) = delete;

public:
    static tempRegMag &get_instance()
    {
        static tempRegMag instance;
        return instance;
    }

private:
    const int minReg = 0;
    const int maxReg = 9;
    map<int, int> symId2reg;
    map<int, int> reg2SymId;
    set<int> usedReg;

private:
    void removeSymAndFreeHisReg(int symId);

public:
    void resetLocalPool();

    // 判断是否有可用寄存器
    bool hasFreeReg();

    // 一条指令结束后,还有哪些变量还活着.清除死变量.
    // 输入: 这条指令的活跃变量的out集合.
    void updataUsefulInfo(set<int> Useful);

    // 在没有free寄存器情况下, 选择一个这条指令不需要用到的寄存器
    // 输入: 这条指令里需要用到的reg
    flushSt flushASymNotUseNow(set<int> SymUse);

    // 在有free的寄存器情况下, 分配一个新的寄存器出来
    int getAFreeRegForThis(int symId);

    // 申请一个寄存器,但是不把该寄存器标记为使用了.
    // 即: 该寄存器的生命期到下一次寄存器申请时就结束!!!
    // 注: 考虑到整体架构, 并没有实现该函数.
    int getAPureFreeReg() { assert(false); }

    // 判断寄存器池中是否有直接可以用的,有的话就直接用
    bool hasThisInReg(int symId);
    int getRegForThis(int symId);

    // 查看所有还会被使用的寄存器
    set<int> askAllUsedReg(set<int> Useful);
};