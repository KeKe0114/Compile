#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

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
    const int fireFighter1 = 8;
    const int fireFighter2 = 9;
    map<int, int> symId2reg;
    map<int, int> reg2SymId;
    set<int> usedReg;

private:
    bool hasFreeReg();
    void updataUsefulInfo(set<int> Useful);

public:
    void resetLocalPool();

    // 判断是否有可用寄存器
    bool askIfHasFreeReg(set<int> Useful);

    // 分配一个新的寄存器出来
    int getAFreeRegForThis(int symId);

    // 判断寄存器池中是否有直接可以用的,有的话就直接用
    bool hasThisInReg(int symId);
    int getRegForThis(int symId);

    // 查看所有还会被使用的寄存器
    set<int> askAllUsedReg(set<int> Useful);
};