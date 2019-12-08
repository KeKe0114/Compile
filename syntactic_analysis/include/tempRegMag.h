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
    map<string, int> name2reg;
    set<int> usedReg;
    
public:
    void resetLocalPool();
    bool hasFreeReg();
    void updataUsefulInfo(set<string> Useful);
    string randomFlushOneReg();
    int getAFreeRegForThis(string name);
    bool hasThisInReg(string name);
    int getRegForThis(string name);
};