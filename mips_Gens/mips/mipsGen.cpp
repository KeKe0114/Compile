#include "mipsGen.h"
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
        collect.div(target, operand1, operand2);
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

void mipsGen::genMips_SwToSymTable(mipsCollect::Register reg, symAttr *attr)
{
    if (attr->refer == GLOBAL)
    {
        collect.sw(reg, attr->name);
    }
    else if (attr->refer == FP)
    {
        collect.sw(reg, attr->offsetRel, collect.$fp);
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
    genMips_SwToSymTable(collect.$v0, target);
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
        collect.li(collect.$t1, num);
        genMips_SwToSymTable(collect.$t1, attr);
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
    if (!codeWorkNow->isInlineRet())
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
    genMips_SwToSymTable(collect.$v0, attr);
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
    genMips_SwToSymTable(collect.$t0, left);
}
void mipsGen::genMipsAssignConst()
{
    symAttr *attr = codeWorkNow->getOperand1();
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
    genMips_SwToSymTable(collect.$t1, attr);
}

void mipsGen::genMipsArrayValueGet()
{
    symAttr *array = codeWorkNow->getOperand2();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand1();
    genMips_LwFromSymTable(collect.$t1, idx);
    genMips_LaFromSymTable(collect.$t0, array);
    collect.mul(collect.$t1, collect.$t1, array->align);
    collect.add(collect.$t0, collect.$t0, collect.$t1);
    collect.lw(collect.$t1, 0, collect.$t0);
    genMips_SwToSymTable(collect.$t1, target);
}
void mipsGen::genMipsArrayValuePut()
{
    symAttr *array = codeWorkNow->getOperand1();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand2();
    genMips_LwFromSymTable(collect.$t1, idx);
    genMips_LaFromSymTable(collect.$t0, array);
    collect.mul(collect.$t1, collect.$t1, array->align);
    collect.add(collect.$t0, collect.$t0, collect.$t1);
    genMips_LwFromSymTable(collect.$t1, target);
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
    genMips_DistinguishOp(collect.$t3, collect.$t0, collect.$t1, codeWorkNow->getOp());
    genMips_SwToSymTable(collect.$t3, target);
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

        else if (codeWorkNow->getType() == codeSt::FunctInlineRetWithValue)
            genMipsFunctRetWithValue();
        else if (codeWorkNow->getType() == codeSt::FunctInlineRetWithoutValue)
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