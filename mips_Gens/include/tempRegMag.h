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
    map<int, int> symId2reg;
    set<int> usedReg;

public:
    void resetLocalPool();
    bool hasFreeReg();
    void updataUsefulInfo(set<int> Useful);
    int getAFreeRegForThis(int symId);
    bool hasThisInReg(int symId);
    int getRegForThis(int symId);
};