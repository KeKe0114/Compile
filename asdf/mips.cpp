#include "mips.h"
#include "debug.h"

void mipsGen::genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op)
{
    if (op == codeSt::op_PLUS)
        collect.add(target, operand1, operand2);
    else if (op == codeSt::op_MINU)
        collect.sub(target, operand1, operand2);
    else if (op == codeSt::op_MULT)
        collect.mul(target, operand1, operand2);
    else if (op == codeSt::op_DIV)
    {
        collect.div(operand1, operand2);
        collect.mflo(target);
    }
    else if (op == codeSt::op_LSS)
        collect.slt(target, operand1, operand2);
    else if (op == codeSt::op_LEQ)
        collect.sle(target, operand1, operand2);
    else if (op == codeSt::op_GRE)
        collect.sgt(target, operand1, operand2);
    else if (op == codeSt::op_GEQ)
        collect.sge(target, operand1, operand2);
    else if (op == codeSt::op_EQL)
        collect.seq(target, operand1, operand2);
    else if (op == codeSt::op_NEQ)
        collect.sne(target, operand1, operand2);
    else
        assert(false);
}

void mipsGen::genMips_LwFromSymTable(mipsCollect::Register reg, symAttr *attr)
{
    if (attr->refer == GLOBAL)
    {
        collect.lw(reg, attr->name);
    }
    else if (attr->refer == FP)
    {
        collect.lw(reg, attr->offsetRel, collect.$fp);
    }
    else
    {
        assert(false);
    }
}

void mipsGen::genMips_LaFromSymTable(mipsCollect::Register reg, symAttr *attr)
{
    if (attr->refer == GLOBAL)
    {
        collect.la(reg, attr->name);
    }
    else if (attr->refer == FP)
    {
        collect.la(reg, attr->offsetRel, collect.$fp);
    }
    else
    {
        assert(false);
    }
}

string mipsGen::genMips_AllocStrName()
{
    strIdGen++;
    return str_prefix + to_string(strIdGen);
}

void mipsGen::genMipsScanf()
{
    symAttr *target = codeWorkNow->getOperand1();
    if (target->type == INT)
    {
        collect.syscall(5);
    }
    else if (target->type == CHAR)
    {
        collect.syscall(12);
    }
    else
    {
        assert(false);
    }
    genMips_LaFromSymTable(collect.$t0, target);
    collect.sw(collect.$v0, 0, collect.$t0);
}

void mipsGen::genMipsPrintStr()
{
    string strName = genMips_AllocStrName();
    string value = codeWorkNow->getValue();
    collect.asciiz(strName, value);
    collect.la(collect.$a0, strName);
    collect.syscall(4);
    collect.la(collect.$a0, newLine);
    collect.syscall(4);
}

void mipsGen::genMipsPrintStrNoNewLine()
{
    string strName = genMips_AllocStrName();
    string value = codeWorkNow->getValue();
    collect.asciiz(strName, value);
    collect.la(collect.$a0, strName);
    collect.syscall(4);
}


void mipsGen::genMipsPrintExp()
{
    symAttr *exp = codeWorkNow->getOperand1();
    genMips_LwFromSymTable(collect.$a0, exp);
    if (exp->type == INT)
    {
        collect.syscall(1);
    }
    else if (exp->type == CHAR)
    {
        collect.syscall(11);
    }
    else
    {
        assert(false);
    }
    collect.la(collect.$a0, newLine);
    collect.syscall(4);
}

void mipsGen::genMipsConstState()
{
    symAttr *attr = codeWorkNow->getOperand1();
    string value = attr->value;
    int num;
    if (attr->type == INT)
    {
        stringstream stream;
        stream << value;
        stream >> num;
    }
    else if (attr->type == CHAR)
    {
        num = (int)value.c_str()[0];
    }
    else
    {
        assert(false);
    }
    if (attr->refer == GLOBAL)
    {
        collect.word(attr->name, num);
    }
    else if (attr->refer == FP)
    {
        genMips_LaFromSymTable(collect.$t0, attr);
        collect.li(collect.$t1, num);
        collect.sw(collect.$t1, 0, collect.$t0);
    }
    else
    {
        assert(false);
    }
}

void mipsGen::genMipsVarState()
{
    symAttr *attr = codeWorkNow->getOperand1();
    if (attr->refer == GLOBAL)
    {
        collect.space(attr->name, attr->size);
    }
}

void mipsGen::genMipsFunctState()
{
    symAttr *attr = codeWorkNow->getOperand1();
    collect.labelLine(attr->name);
    collect.sw(collect.$ra, 4, collect.$sp);
    collect.sw(collect.$fp, 8, collect.$sp);
    collect.sw(collect.$sp, 12, collect.$sp);
    collect.add(collect.$fp, collect.$sp, collect.$ZERO);
    collect.add(collect.$sp, collect.$sp, attr->size);
}

void mipsGen::genMipsFunctRetWithValue()
{
    symAttr *attr = codeWorkNow->getOperand1();
    genMips_LwFromSymTable(collect.$v0, attr);
    genMipsFunctRetWithoutValue();
}

void mipsGen::genMipsFunctRetWithoutValue()
{
    collect.lw(collect.$sp, 12, collect.$fp);
    collect.lw(collect.$fp, 8, collect.$sp);
    collect.lw(collect.$ra, 4, collect.$sp);
    collect.jr(collect.$ra);
}
void mipsGen::genMipsFunctArgsPush()
{
    symAttr *attr = codeWorkNow->getOperand1();
    genMips_LwFromSymTable(collect.$t0, attr);
    collect.sw(collect.$t0, functionCallInventArgLen + spVerticalOffset, collect.$sp);
    spVerticalOffset += 4;
}
void mipsGen::genMipsFunctCall()
{
    spVerticalOffset = 0;
    symAttr *attr = codeWorkNow->getOperand1();
    collect.jal(attr->name);
}

void mipsGen::genMipsFunctRetUse()
{
    symAttr *attr = codeWorkNow->getOperand1();
    genMips_LaFromSymTable(collect.$t0, attr);
    collect.sw(collect.$v0, 0, collect.$t0);
}

void mipsGen::genMipsBNZ()
{
    collect.bnez(collect.$t9, codeWorkNow->getValue());
}
void mipsGen::genMipsBZ()
{
    collect.beqz(collect.$t9, codeWorkNow->getValue());
}
void mipsGen::genMipsJUMP()
{
    collect.jump(codeWorkNow->getValue());
}

void mipsGen::genMipsLabelLine()
{
    collect.labelLine(codeWorkNow->getValue());
}

void mipsGen::genMipsAssignValue()
{
    symAttr *left = codeWorkNow->getOperand1();
    symAttr *right = codeWorkNow->getOperand2();
    genMips_LwFromSymTable(collect.$t0, right);
    genMips_LaFromSymTable(collect.$t1, left);
    collect.sw(collect.$t0, 0, collect.$t1);
}
void mipsGen::genMipsAssignConst()
{
    symAttr *attr = codeWorkNow->getOperand1();
    genMips_LaFromSymTable(collect.$t0, attr);
    string value = codeWorkNow->getValue();
    int num;
    if (attr->type == INT)
    {
        stringstream stream;
        stream << value;
        stream >> num;
    }
    else if (attr->type == CHAR)
    {
        num = (int)value.c_str()[0];
    }
    else
    {
        assert(false);
    }
    collect.li(collect.$t1, num);
    collect.sw(collect.$t1, 0, collect.$t0);
}

void mipsGen::genMipsArrayValueGet()
{
    symAttr *array = codeWorkNow->getOperand2();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand1();
    genMips_LaFromSymTable(collect.$t0, array);
    genMips_LwFromSymTable(collect.$t1, idx);
    collect.mul(collect.$t1, collect.$t1, array->align);
    collect.add(collect.$t0, collect.$t0, collect.$t1);
    collect.lw(collect.$t1, 0, collect.$t0);
    genMips_LaFromSymTable(collect.$t2, target);
    collect.sw(collect.$t1, 0, collect.$t2);
}
void mipsGen::genMipsArrayValuePut()
{
    symAttr *array = codeWorkNow->getOperand1();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand2();
    genMips_LaFromSymTable(collect.$t0, array);
    genMips_LwFromSymTable(collect.$t1, idx);
    collect.mul(collect.$t1, collect.$t1, array->align);
    collect.add(collect.$t0, collect.$t0, collect.$t1);
    genMips_LaFromSymTable(collect.$t2, target);
    collect.lw(collect.$t1, 0, collect.$t2);
    collect.sw(collect.$t1, 0, collect.$t0);
}
void mipsGen::genMipsCondition()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    symAttr *num2 = codeWorkNow->getOperand2();
    genMips_LwFromSymTable(collect.$t0, num1);
    genMips_LwFromSymTable(collect.$t1, num2);
    genMips_DistinguishOp(collect.$t9, collect.$t0, collect.$t1, codeWorkNow->getOp());
}
void mipsGen::genMipsCondition4Num()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    genMips_LwFromSymTable(collect.$t9, num1);
}

void mipsGen::genMipsFourYuan()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    symAttr *num2 = codeWorkNow->getOperand2();
    symAttr *target = codeWorkNow->getResult();
    genMips_LwFromSymTable(collect.$t0, num1);
    genMips_LwFromSymTable(collect.$t1, num2);
    genMips_LaFromSymTable(collect.$t2, target);
    genMips_DistinguishOp(collect.$t3, collect.$t0, collect.$t1, codeWorkNow->getOp());
    collect.sw(collect.$t3, 0, collect.$t2);
}

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

void mipsCollect::asciiz(string name, string value) /*内含data(),text()*/
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
void mipsCollect::div(Register reg1, Register reg2)
{
    text();
    ss << "div " << Register2Str[reg1] << ", " << Register2Str[reg2] << endl;
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

void mipsGen::gen_mips_code()
{
    int size = midcode.midCode_size();
    for (int i = 0; i < size; i++)
    {
        codeWorkNow = midcode.get_midCode_by_idx(i);
        if (codeWorkNow->getType() == codeSt::Scanf)
            genMipsScanf();
        else if (codeWorkNow->getType() == codeSt::PrintStr)
            genMipsPrintStr();
        else if (codeWorkNow->getType() == codeSt::PrintStrNoNewLine)
            genMipsPrintStrNoNewLine();
        else if (codeWorkNow->getType() == codeSt::PrintExp)
            genMipsPrintExp();
        else if (codeWorkNow->getType() == codeSt::ConstVarState)
        {
            if (codeWorkNow->getOperand1()->kind == CONST)
                genMipsConstState();
            else if (codeWorkNow->getOperand1()->kind == VAR)
                genMipsVarState();
            else
                assert(false);
        }
        else if (codeWorkNow->getType() == codeSt::FunctState)
            genMipsFunctState();
        else if (codeWorkNow->getType() == codeSt::FunctRetWithValue)
            genMipsFunctRetWithValue();
        else if (codeWorkNow->getType() == codeSt::FunctRetWithoutValue)
            genMipsFunctRetWithoutValue();
        else if (codeWorkNow->getType() == codeSt::FunctArgsPush)
            genMipsFunctArgsPush();
        else if (codeWorkNow->getType() == codeSt::FunctCall)
            genMipsFunctCall();
        else if (codeWorkNow->getType() == codeSt::FunctRetUse)
            genMipsFunctRetUse();
        else if (codeWorkNow->getType() == codeSt::BNZ)
            genMipsBNZ();
        else if (codeWorkNow->getType() == codeSt::BZ)
            genMipsBZ();
        else if (codeWorkNow->getType() == codeSt::Jump)
            genMipsJUMP();
        else if (codeWorkNow->getType() == codeSt::Label)
            genMipsLabelLine();
        else if (codeWorkNow->getType() == codeSt::AssignValue)
            genMipsAssignValue();
        else if (codeWorkNow->getType() == codeSt::AssignConst)
            genMipsAssignConst();
        else if (codeWorkNow->getType() == codeSt::ArrayValuePut)
            genMipsArrayValuePut();
        else if (codeWorkNow->getType() == codeSt::ArrayValueGet)
            genMipsArrayValueGet();
        else if (codeWorkNow->getType() == codeSt::Condition)
            genMipsCondition();
        else if (codeWorkNow->getType() == codeSt::Condition4Num)
            genMipsCondition4Num();
        else if (codeWorkNow->getType() == codeSt::FourYuan)
            genMipsFourYuan();
        else
            assert(false);
    }
}