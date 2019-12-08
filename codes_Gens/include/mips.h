#pragma once
#include "symbols.h"
#include "midcode.h"
#include "mipsCollect.h"
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class mipsGen
{
private:
    mipsGen() : midcode(midCodeGen::get_instance()), symbolist(symbols::get_instance()), collect(mipsCollect::get_instance()), str_prefix("$str")
    {
        newLine = genMips_AllocStrName();
        collect.asciiz(newLine, "\\n");
        string align_use = genMips_AllocStrName();
        collect.space(align_use, 2);
        collect.add(collect.$fp, collect.$ZERO, collect.$sp);
        collect.jal("main");
        collect.syscall(10);
    }
    mipsGen(const mipsGen &) = delete;
    mipsGen &operator&(const mipsGen &) = delete;

public:
    static mipsGen &get_instance()
    {
        static mipsGen instance;
        return instance;
    }

public:
    void gen_mips_code();
    string get_mips_str() { return collect.get_mips_str(); }

private:
    mipsCollect &collect;
    midCodeGen &midcode;
    symbols &symbolist;
    codeSt *codeWorkNow;
    int strIdGen;
    string str_prefix;
    int spVerticalOffset;
    string newLine;
    int functionCallInventArgLen = 16;

    string genMips_AllocStrName();
    void genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op);
    void genMips_LwFromSymTable(mipsCollect::Register reg, symAttr *attr);
    void genMips_LaFromSymTable(mipsCollect::Register reg, symAttr *attr);
    void genMips_SwToSymTable(mipsCollect::Register reg, symAttr *attr);

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
    void genMipsCondition(); /*CHEN: condition系列 和 BNZ系列约定寄存器用t9*/
    void genMipsCondition4Num();

    void genMipsFourYuan();
};