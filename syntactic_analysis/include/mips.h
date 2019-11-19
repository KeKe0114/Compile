#pragma once
#include "symbols.h"
#include "midcode.h"
#include <sstream>
class mipsCollect
{
private:
    mipsCollect() {}
    mipsCollect(const mipsCollect &) = delete;
    mipsCollect &operator&(mipsCollect &) = delete;
    static vector<string> Register2Str;

public:
    static mipsCollect &get_instance()
    {
        Register2Str.push_back("$zero");
        Register2Str.push_back("$at");
        for (int i = 0; i <= 1; i++)
            Register2Str.push_back("$v" + to_string(i));
        for (int i = 0; i <= 3; i++)
            Register2Str.push_back("$a" + to_string(i));
        for (int i = 0; i <= 7; i++)
            Register2Str.push_back("$t" + to_string(i));
        for (int i = 0; i <= 7; i++)
            Register2Str.push_back("$s" + to_string(i));
        for (int i = 8; i <= 9; i++)
            Register2Str.push_back("$t" + to_string(i));
        Register2Str.push_back("$gp");
        Register2Str.push_back("$fp");
        Register2Str.push_back("$sp");
        Register2Str.push_back("$ra");
        static mipsCollect instance;
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

private:
    stringstream ss;
    void data();
    void text();

public:
    void asciiz(string name, string value); /*内含data(),text()*/
    void space(string name, int bytes);

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
};

class mipsGen
{
private:
    mipsGen() : midcode(midCodeGen::get_instance()), symbolist(symbols::get_instance()), collect(mipsCollect::get_instance()), str_prefix("%str") {}
    mipsGen(const mipsGen &) = delete;
    mipsGen &operator&(const mipsGen &) = delete;

public:
    static mipsGen &get_instance()
    {
        static mipsGen instance;
        return instance;
    }

private:
    mipsCollect &collect;
    midCodeGen &midcode;
    symbols &symbolist;
    codeSt *codeWorkNow;
    int strIdGen;
    string str_prefix;
    int spVerticalOffset;
    int functionCallInventArgLen = 16;

    string genMips_AllocStrName();
    void genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op);
    void genMips_LwFromSymTable(mipsCollect::Register reg, symAttr *attr);
    void genMips_LaFromSymTable(mipsCollect::Register reg, symAttr *attr);

    void genMipsScanf();
    void genMipsPrintStr();
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