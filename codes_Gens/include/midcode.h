#pragma once
#include <string>
#include "symbols.h"
#include "codeSt.h"
#include "inlineMag.h"

using namespace std;

class midCodeGen
{
private:
    midCodeGen() : symbolist(symbols::get_instance()), label_prefix("Label"), tmp_prefix("$tmp"), inlineSwitch(true)
    {
        workSpace = &codes;
    }
    midCodeGen(const midCodeGen &) = delete;
    midCodeGen &operator&(const midCodeGen &) = delete;

public:
    static midCodeGen &get_instance()
    {
        static midCodeGen instance;
        return instance;
    }

private:
    symbols &symbolist;
    vector<codeSt> codes;
    vector<codeSt> temp;
    vector<codeSt> *workSpace;
    int labelGen;
    int tmpGen;
    string label_prefix;
    string tmp_prefix;

    string genMid_AllocTmp(symType type);

public:
    codeSt *get_midCode_by_idx(int idx) { return &codes[idx]; }
    int midCode_size() { return codes.size(); }
    vector<codeSt> getCodesVector() { return codes; }
    void insert(codeSt code) { codes.push_back(code); }

private:
    bool inlineSwitch;
    map<string, inlineSaver> func2Saver;

    bool funcWorkCanInline;
    /*均为[start, end)*/
    int funcCodeStart;
    int funcCodeEnd;
    int symLocalStart;
    int symLocalEnd;

private:
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

public:
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
    void genInlineMidCode(string funcName, vector<string> args)
    {
        if (checkCanInline(funcName))
        {
            vector<codeSt> shouldInsert = func2Saver.find(funcName)->second.mergeInlineCode(args);
            for (int i = 0; i < shouldInsert.size(); i++)
                codes.push_back(shouldInsert[i]);
        }
    }

public:
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

public:
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
};