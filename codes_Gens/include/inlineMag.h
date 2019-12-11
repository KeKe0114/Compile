#pragma once
#include <vector>
#include <map>
#include "symbols.h"
#include "codeSt.h"

class inlineSaver
{
private:
    string prefix;
    string funcName;
    vector<symAttr> syms;
    vector<symAttr> para;
    vector<codeSt> codes;
    map<int, int> symMap;
    symbols &symbolist;

public:
    inlineSaver(string name) : symbolist(symbols::get_instance()), prefix("$inline") { this->funcName = name; }
    void setCodes(vector<codeSt> codes) { this->codes = codes; }
    void setFormalPara(vector<symAttr> para) { this->para = para; }
    void setSyms(vector<symAttr> syms)
    {
        for (int i = 0; i < syms.size(); i++)
        {
            syms[i].name = prefix + funcName + syms[i].name;
        }
        this->syms = syms;
    }

private:
    void mergeSymsToSymbols()
    {
        if (syms.size() == 0)
            return;
        if (symbolist.hasNowSeg(syms[0].name))
            return;
        for (int i = 0; i < syms.size(); i++)
        {
            int idOld = syms[i].SymId;
            int idNew = symbolist.insert(syms[i]);
            symMap.insert(pair<int, int>(idOld, idNew));
        }
    }

    vector<codeSt> outputCodes()
    {
        vector<codeSt> ans;
        for (int i = 0; i < codes.size(); i++)
        {
            codeSt codeTmp = codes[i];
            assert(codeTmp.getType() != codeSt::FunctState);
            if (codeTmp.getType() == codeSt::FunctRetWithoutValue)
            {
                break;
            }
            if (codeTmp.getType() == codeSt::FunctRetWithValue)
            {
                codeTmp.setInlineRet();
            }
            if (symMap.find(codeTmp.operand1) != symMap.end())
            {
                codeTmp.operand1 = symMap[codeTmp.operand1];
            }
            if (symMap.find(codeTmp.operand2) != symMap.end())
            {
                codeTmp.operand2 = symMap[codeTmp.operand2];
            }
            if (symMap.find(codeTmp.result) != symMap.end())
            {
                codeTmp.result = symMap[codeTmp.result];
            }
            if (symMap.find(codeTmp.idx) != symMap.end())
            {
                codeTmp.idx = symMap[codeTmp.idx];
            }
            ans.push_back(codeTmp);
            if (codeTmp.getType() == codeSt::FunctRetWithValue)
            {
                break;
            }
        }
        return ans;
    }

public:
    vector<codeSt> mergeInlineCode(vector<string> argsName)
    {
        symMap.clear();
        for (int i = 0; i < argsName.size(); i++)
        {
            int idOld = para[i].SymId;
            int idNew = symbolist.get(argsName[i]).SymId;
            symMap.insert(pair<int, int>(idOld, idNew));
        }
        mergeSymsToSymbols();
        return outputCodes();
    }
};