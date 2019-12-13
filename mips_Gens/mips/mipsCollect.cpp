#include "mipsCollect.h"
#include "debug.h"

//mipsCollect
void mipsCollect::data()
{
    if (textNow)
    {
        ss << ".data" << endl;
        textNow = false;
    }
}
void mipsCollect::text()
{
    if (!textNow)
    {
        ss << ".text" << endl;
        textNow = true;
    }
}

void mipsCollect::comment(string comments)
{
    ss << " #  " << comments << endl;
}

void mipsCollect::asciiz(string name, string value) /*内坫data(),text()*/
{
    data();
    ss << name << ": .asciiz \"" << value << "\"" << endl;
}
void mipsCollect::space(string name, int bytes)
{
    data();
    ss << name << ": .space " << bytes << endl;
}
void mipsCollect::word(string name, int num)
{
    data();
    ss << name << ": .word " << num << endl;
}

void mipsCollect::sw(Register reg1, int offset, Register reg2)
{
    text();
    ss << "sw " << Register2Str[reg1] << ", " << offset << "(" << Register2Str[reg2] << ")" << endl;
}

void mipsCollect::sw(Register reg1, string label)
{
    text();
    ss << "sw " << Register2Str[reg1] << ", " << label << endl;
}
void mipsCollect::sw(Register reg1, string name, Register reg2)
{
    text();
    ss << "sw " << Register2Str[reg1] << ", " << name << "(" << Register2Str[reg2] << ")" << endl;
}

void mipsCollect::lw(Register reg1, int offset, Register reg2)
{
    text();
    ss << "lw " << Register2Str[reg1] << ", " << offset << "(" << Register2Str[reg2] << ")" << endl;
}
void mipsCollect::lw(Register reg1, string name)
{
    text();
    ss << "lw " << Register2Str[reg1] << ", " << name << endl;
}
void mipsCollect::lw(Register reg1, string name, Register reg2)
{
    text();
    ss << "lw " << Register2Str[reg1] << ", " << name << "(" << Register2Str[reg2] << ")" << endl;
}
void mipsCollect::la(Register reg1, string name)
{
    text();
    ss << "la " << Register2Str[reg1] << ", " << name << endl;
}
void mipsCollect::la(Register reg1, int offset, Register reg2)
{
    text();
    ss << "la " << Register2Str[reg1] << ", " << offset << "(" << Register2Str[reg2] << ")" << endl;
}
void mipsCollect::li(Register reg1, int num)
{
    text();
    ss << "li " << Register2Str[reg1] << ", " << num << endl;
}

void mipsCollect::syscall(int num)
{
    text();
    li(Register::$v0, num);
    ss << "syscall" << endl;
}

void mipsCollect::labelLine(string label)
{
    text();
    ss << label << ":" << endl;
}

void mipsCollect::beqz(Register reg1, string label)
{
    text();
    ss << "beqz " << Register2Str[reg1] << ", " << label << endl;
}
void mipsCollect::bnez(Register reg1, string label)
{
    text();
    ss << "bnez " << Register2Str[reg1] << ", " << label << endl;
}
void mipsCollect::jump(string label)
{
    text();
    ss << "j " << label << endl;
}
void mipsCollect::move(Register reg1, Register reg2)
{
    text();
    ss << "move " << Register2Str[reg1] << ", " << Register2Str[reg2] << endl;
}
void mipsCollect::add(Register reg1, Register reg2, int num)
{
    text();
    ss << "add " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << num << endl;
}
void mipsCollect::add(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "add " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::sub(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "sub " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::mul(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "mul " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::mul(Register reg1, Register reg2, int num)
{
    text();
    ss << "mul " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << num << endl;
}
void mipsCollect::div(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "div " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
    mflo(reg1);
}
void mipsCollect::slt(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "slt " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::sle(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "sle " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::sgt(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "sgt " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::sge(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "sge " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::seq(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "seq " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}
void mipsCollect::sne(Register reg1, Register reg2, Register reg3)
{
    text();
    ss << "sne " << Register2Str[reg1] << ", " << Register2Str[reg2] << ", " << Register2Str[reg3] << endl;
}

void mipsCollect::jal(string FuncName)
{
    text();
    ss << "jal " << FuncName << endl;
}
void mipsCollect::jr(Register reg1)
{
    text();
    ss << "jr " << Register2Str[reg1] << endl;
}
void mipsCollect::mflo(Register reg1)
{
    text();
    ss << "mflo " << Register2Str[reg1] << endl;
}
void mipsCollect::mfhi(Register reg1)
{
    text();
    ss << "mfhi " << Register2Str[reg1] << endl;
}
