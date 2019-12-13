#pragma once
#include "symbols.h"
#include "dataFlow.h"
#include "mipsCollect.h"
#include "tempRegMag.h"
#include "globalRegMag.h"
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class mipsGraphGen
{
private:
    mipsGraphGen() : tempReg(tempRegMag::get_instance()), globalReg(globalRegMag::get_instance()), flowGraph(blockFlowGraph::get_instance()), symbolist(symbols::get_instance()), collect(mipsCollect::get_instance()), str_prefix("$str")
    {
        newLine = genMips_AllocStrName();
        collect.asciiz(newLine, "\\n");
        string align_use = genMips_AllocStrName();
        collect.space(align_use, 2);
        collect.add(collect.$fp, collect.$ZERO, collect.$sp);
        collect.jal("main");
        collect.syscall(10);
    }
    mipsGraphGen(const mipsGraphGen &) = delete;
    mipsGraphGen &operator&(const mipsGraphGen &) = delete;

public:
    static mipsGraphGen &get_instance()
    {
        static mipsGraphGen instance;
        return instance;
    }

private:
    void handOutToSolve(int blockNum, int codeNum);

public:
    void gen_mips_code();
    string get_mips_str() { return collect.get_mips_str(); }

private:
    mipsCollect &collect;
    blockFlowGraph &flowGraph;
    tempRegMag &tempReg;
    globalRegMag &globalReg;
    symbols &symbolist;
    string funcNameWorkNow;
    block *blockWorkNow;
    codeSt *codeWorkNow;
    int strIdGen;
    string str_prefix;
    int spVerticalOffset;
    string newLine;
    int functionCallInventArgLen = 16;

    // CHEN: 完成全局变量向局部变量转换的平滑过渡.
    void maintainConsistencyForNowBlock();

    string genMips_AllocStrName();

    void flushToMem(int symId, mipsCollect::Register reg);
    void flushToMemIfGlobal(int symId, mipsCollect::Register reg);

    void loadFromMem(int symId, mipsCollect::Register reg);

    mipsCollect::Register GetConditionReg() { return mipsCollect::getSeriesV(1); }

    mipsCollect::Register AllocAPureFreeReg() { return mipsCollect::getSeriesV(1); }

    mipsCollect::Register GetOrAllocRegisterToSym(symAttr *attr);

    mipsCollect::Register LoadSymRealValueToRegister(symAttr *attr);

    void LoadSymToRegisterTold(symAttr *attr, mipsCollect::Register told);

    //判断一个attr是否是const
    // CHEN: genConstState
    bool IsAConstSym(int SymId);
    int getConstValue(int SymId);
    int getConstType(int SymId);

private:
    void genMipsScanf();
    void genMipsPrintStr();
    void genMipsPrintStrNoNewLine();
    void genMipsPrintExp();

    void genMipsConstState();
    void genMipsVarState();

    void genMipsFunctState();
    void genMipsFunctRetWithValue();
    void genMipsFunctRetWithoutValue();

    void genMipsFunctArgsPush();
    void genMipsFunctCall();
    void genMipsFunctRetUse(); /*CHEN: retUse 和 ret 约定寄存器用v0*/

    void genMipsBNZ();
    void genMipsBZ();
    void genMipsJUMP();
    void genMipsLabelLine();

    void genMipsAssignValue();
    void genMipsAssignConst();
    void genMipsArrayValueGet();
    void genMipsArrayValuePut();
    void genMipsCondition(); /*CHEN: condition系列 和 BNZ系列约定寄存器用v1*/
    void genMipsCondition4Num();

    void genMipsFourYuan();

    void genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op);
};