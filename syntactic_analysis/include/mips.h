#pragma once
#include "symbols.h"
#include "midcode.h"
#include <sstream>
#include <vector>
#include <string>
using namespace std;
class mipsCollect
{
private:
    mipsCollect() {}
    mipsCollect(const mipsCollect &) = delete;
    mipsCollect &operator&(mipsCollect &) = delete;
    vector<string> Register2Str;

public:
    static mipsCollect &get_instance()
    {
        static mipsCollect instance;

        instance.Register2Str.push_back("$zero");
        instance.Register2Str.push_back("$at");
        for (int i = 0; i <= 1; i++)
            instance.Register2Str.push_back("$v" + to_string(i));
        for (int i = 0; i <= 3; i++)
            instance.Register2Str.push_back("$a" + to_string(i));
        for (int i = 0; i <= 7; i++)
            instance.Register2Str.push_back("$t" + to_string(i));
        for (int i = 0; i <= 7; i++)
            instance.Register2Str.push_back("$s" + to_string(i));
        for (int i = 8; i <= 9; i++)
            instance.Register2Str.push_back("$t" + to_string(i));
        instance.Register2Str.push_back("$gp");
        instance.Register2Str.push_back("$sp");
        instance.Register2Str.push_back("$fp");
        instance.Register2Str.push_back("$ra");
        return instance;
    }
    enum Register
    {
        $ZERO,
        $at,
        $v0,
        $v1,
        $a0,
        $a1,
        $a2,
        $a3,
        $t0,
        $t1,
        $t2,
        $t3,
        $t4,
        $t5,
        $t6,
        $t7,
        $s0,
        $s1,
        $s2,
        $s3,
        $s4,
        $s5,
        $s6,
        $s7,
        $t8,
        $t9,
        $gp,
        $sp,
        $fp,
        $ra,
    };

public:
    string get_mips_str() { return ss.str(); }

private:
    stringstream ss;
    bool textNow = true;
    void data();
    void text();

public:
    void asciiz(string name, string value); /*内含data(),text()*/
    void space(string name, int bytes);
    void word(string name, int num);

    void sw(Register, int, Register);
    void lw(Register, int, Register);
    void lw(Register, string name);
    void la(Register, string);
    void la(Register, int, Register);
    void li(Register, int);

    void syscall(int);

    void labelLine(string label);

    void beqz(Register, string);
    void bnez(Register, string);
    void jump(string);

    void add(Register, Register, int);
    void add(Register, Register, Register);
    void sub(Register, Register, Register);
    void mul(Register, Register, Register);
    void mul(Register, Register, int);
    void div(Register, Register);
    void slt(Register, Register, Register);
    void sle(Register, Register, Register);
    void sgt(Register, Register, Register);
    void sge(Register, Register, Register);
    void seq(Register, Register, Register);
    void sne(Register, Register, Register);

    void jal(string FuncName);
    void jr(Register);
    void mflo(Register);
    void mfhi(Register);
};

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