#pragma once
#include <string>
#include "symbols.h"
#include "codeSt.h"
#include "inlineMag.h"

using namespace std;

class midCodeGen
{
private: // 单例模式
    midCodeGen() : symbolist(symbols::get_instance()), label_prefix("Label"), tmp_prefix("$tmp"), inlineSwitch(true)
    {
        workSpace = &codes;
    }
    midCodeGen(const midCodeGen &) = delete;
    midCodeGen &operator&(const midCodeGen &) = delete;

public: //  单例模式
    static midCodeGen &get_instance()
    {
        static midCodeGen instance;
        return instance;
    }

private: // 变量: 生成中间代码
    symbols &symbolist;
    vector<codeSt> codes;
    int labelGen;
    int tmpGen;
    string label_prefix;
    string tmp_prefix;

private: // 函数: 生成中间代码
    string genMid_AllocTmp(symType type);

public: //  接口: 生成中间代码
    void genMidFuncDef(string funcName);

    void genMidArgsPush(string paraName);
    void genMidFuncCall(string func);
    void genMidFuncRet(string name);
    string genMidFuncRetUse(string funcName);

    void genMidVarState(string name);
    void genMidConstState(string name);

    string genMid_AllocLabel();
    void genMidLabelLine(string Label);
    string genMidConstTmp(symType type, string value);
    string genMidExpress(string operand1, token_key op, string operand2);
    string genMidToINT(string operand1);
    string genMidValueGet(string name);
    void genMidValuePut(string name, string value);
    string genMidArrayValueGet(string array, string idx);
    void genMidArrayValuePut(string array, string idx, string value);

    void genMidCondition(string operand1, token_key op, string operand2);
    void genMidCondition4Num(string operand1);
    void genMidGoto(string Label);
    void genMidBNZ(string Label);
    void genMidBZ(string Label);

    void genMidScanf(string name);
    void genMidPrintfStr(string str);
    void genMidPrintfStrNoNewLine(string str);

    void genMidPrintfExp(string name);

private: // 变量: 函数内联
    bool inlineSwitch;
    map<string, inlineSaver> func2Saver;

    bool funcWorkCanInline;
    /*均为[start, end)*/
    int funcCodeStart;
    int funcCodeEnd;
    int symLocalStart;
    int symLocalEnd;

private: // 函数: 函数内联
    void resetInlineRecord()
    {
        funcWorkCanInline = true;
        funcCodeStart = -1;
        funcCodeEnd = -1;
        symLocalStart = -1;
        symLocalEnd = -1;
    }

    void SetCantInlineFunc()
    {
        funcWorkCanInline = false;
    }

public: //  接口: 函数内联
    void inlinefuncCodeStart() { funcCodeStart = codes.size(); }
    void inlinefuncCodeEnd() { funcCodeEnd = codes.size(); }

    void funcLocalStart() { symLocalStart = symbolist.stackLocation(); }
    void funcLocalEnd() { symLocalEnd = symbolist.stackLocation(); }

    void startInlineSaver(string funcName)
    {
        resetInlineRecord();
        funcLocalStart();
    }

    void endInlineSaver(string funcName)
    {
        inlinefuncCodeEnd();
        funcLocalEnd();
        if (inlineSwitch && funcWorkCanInline && funcCodeStart > 0)
        {
            inlineSaver saver(funcName);
            vector<codeSt> codeTmp;
            for (int i = funcCodeStart; i < funcCodeEnd; i++)
            {
                codeTmp.push_back(codes[i]);
            }
            saver.setCodes(codeTmp);
            saver.setSyms(symbolist.getStackSlice(symLocalStart, symLocalEnd));
            func2Saver.insert(pair<string, inlineSaver>(funcName, saver));
        }
        resetInlineRecord();
    }
    bool checkCanInline(string funcName)
    {
        return func2Saver.find(funcName) != func2Saver.end() && inlineSwitch;
    }
    string genInlineMidCode(string funcName, vector<string> args)
    {
        if (checkCanInline(funcName))
        {
            vector<codeSt> shouldInsert = func2Saver.find(funcName)->second.mergeInlineCode(args);
            for (int i = 0; i < shouldInsert.size(); i++)
            {
                if (i == shouldInsert.size() - 1)
                {
                    codeSt codeFin = shouldInsert[i];
                    if (codeFin.isInlineRet())
                    {
                        return codeFin.getOperand1()->name;
                    }
                    else
                    {
                        codes.push_back(shouldInsert[i]);
                    }
                }
                else
                {
                    codes.push_back(shouldInsert[i]);
                }
            }
        }
        return "NO_INLINE_RET";
    }

private: // 变量: 循环语句
    vector<codeSt> temp;
    vector<codeSt> *workSpace;

public: //  接口: 循环语句
    void useTempBegin()
    {
        assert(workSpace != &temp);
        workSpace = &temp;
    }
    void useTempEnd()
    {
        assert(workSpace != &codes);
        workSpace = &codes;
    }
    vector<codeSt> getTempValue()
    {
        vector<codeSt> ret = temp;
        vector<codeSt>().swap(temp);
        return ret;
    }
    void transportTemp(vector<codeSt> tempCode) { codes.insert(codes.end(), tempCode.begin(), tempCode.end()); }

public: //  接口: 访问接口
    codeSt *get_midCode_by_idx(int idx) { return &codes[idx]; }
    int midCode_size() { return codes.size(); }
    vector<codeSt> getCodesVector() { return codes; }
    void insert(codeSt code) { codes.push_back(code); }

private: // 接口: 内部访问接口
    bool hasLastCode() { return workSpace->size() > 0; }
    codeSt getLastCode() { return (*workSpace)[workSpace->size() - 1]; }
    bool isTmp(string name) { return name.find(tmp_prefix) == 0; }
};