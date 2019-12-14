#pragma once
#include <vector>
#include <map>
#include "symbols.h"
#include "codeSt.h"

class inlineSaver
{
private: // 变量
    int round;
    string prefix;
    string funcName;
    vector<symAttr> syms;
    vector<codeSt> codes;
    map<int, int> symMap;
    symbols &symbolist;

public: //  接口: 函数定义处
    inlineSaver(string name) : symbolist(symbols::get_instance()), prefix("$inline"), round(0) { this->funcName = name; }
    void setCodes(vector<codeSt> codes) { this->codes = codes; }
    void setSyms(vector<symAttr> syms)
    {
        for (int i = 0; i < syms.size(); i++)
        {
            syms[i].name = prefix + funcName + syms[i].name;
        }
        this->syms = syms;
    }

private: // 函数: 符号的转化
    void mergeSymsToSymbols()
    {
        if (syms.size() == 0)
            return;
        for (int i = 0; i < syms.size(); i++)
        {
            symAttr tmp = syms[i];
            int idOld = tmp.SymId;
            tmp.name = tmp.name + "R" + to_string(round);
            int idNew = symbolist.insert(tmp);
            symMap.insert(pair<int, int>(idOld, idNew));
        }
    }

    vector<codeSt> outputCodes(vector<codeSt> ans)
    {
        vector<codeSt> ansNew;
        for (int i = 0; i < ans.size(); i++)
        {
            codeSt codeTmp = ans[i];
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
            ansNew.push_back(codeTmp);
            if (codeTmp.getType() == codeSt::FunctRetWithValue)
            {
                break;
            }
        }
        return ansNew;
    }

public: //  接口: 函数使用处
    vector<codeSt> mergeInlineCode(vector<string> argsName)
    {
        round++;
        symMap.clear();
        /*organize symbolist*/
        mergeSymsToSymbols();

        /*args push*/
        map<int, int> argsMap;
        for (int i = 0; i < argsName.size(); i++)
        {
            int idOld = syms[i].SymId;
            int argsId = symbolist.get_id(argsName[i]);
            argsMap.insert(pair<int, int>(idOld, argsId));
        }
        vector<codeSt> ans;
        bool arriveRet = false;
        for (int i = 0; i < codes.size(); i++)
        {
            codeSt codeNow = codes[i];
            switch (codeNow.getType())
            {
            case codeSt::PrintExp:
            case codeSt::FunctArgsPush:
            case codeSt::FunctRetWithValue:
            case codeSt::FunctInlineRetWithValue:
            case codeSt::Condition4Num:
                if (argsMap.find(codeNow.operand1) != argsMap.end())
                    codeNow.operand1 = argsMap.find(codeNow.operand1)->second;
                break;

            case codeSt::AssignValue:
                if (argsMap.find(codeNow.operand2) != argsMap.end())
                    codeNow.operand2 = argsMap.find(codeNow.operand2)->second;
                break;

            case codeSt::ArrayValueGet:
            case codeSt::ArrayValuePut:
                if (argsMap.find(codeNow.operand2) != argsMap.end())
                    codeNow.operand2 = argsMap.find(codeNow.operand2)->second;
                if (argsMap.find(codeNow.idx) != argsMap.end())
                    codeNow.idx = argsMap.find(codeNow.idx)->second;
                break;

            case codeSt::Condition:
            case codeSt::FourYuan:
                if (argsMap.find(codeNow.operand1) != argsMap.end())
                    codeNow.operand1 = argsMap.find(codeNow.operand1)->second;
                if (argsMap.find(codeNow.operand2) != argsMap.end())
                    codeNow.operand2 = argsMap.find(codeNow.operand2)->second;
                break;

            default:
                break;
            }
            switch (codeNow.getType())
            {
            case codeSt::Scanf:
            case codeSt::FunctRetUse:
            case codeSt::AssignValue:
            case codeSt::AssignConst:
            case codeSt::ConstVarState:
            case codeSt::ArrayValueGet:
            case codeSt::ArrayValuePut:
                argsMap.erase(codeNow.operand1);
                break;
            case codeSt::FourYuan:
                argsMap.erase(codeNow.result);
                break;

            default:
                break;
            }
            ans.push_back(codeNow);
            if (arriveRet)
                break;
        }
        return outputCodes(ans);
    }
};