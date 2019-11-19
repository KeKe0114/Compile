#include "mips.h"
#include "debug.h"

void mipsGen::genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op)
{
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
}

void mipsGen::genMipsConstState() {}
void mipsGen::genMipsVarState() {}

void mipsGen::genMipsFunctState() {}
void mipsGen::genMipsFunctRetWithValue() {}
void mipsGen::genMipsFunctRetWithoutValue() {}

void mipsGen::genMipsFunctArgsPush() {}
void mipsGen::genMipsFunctCall() {}
void mipsGen::genMipsFunctRetUse() {}

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
    genMips_LaFromSymTable(collect.$t1, idx);
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
    genMips_LaFromSymTable(collect.$t1, idx);
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
