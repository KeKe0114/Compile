#include "mipsGraphGen.h"
#include "debug.h"

string mipsGraphGen::genMips_AllocStrName()
{
    strIdGen++;
    return str_prefix + to_string(strIdGen);
}

// 主逻辑
void mipsGraphGen::genMipsScanf()
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
        target->SHOW_ATTR();
        assert(false);
    }
    mipsCollect::Register targetR = GetOrAllocRegisterToSym(target);
    mipsCollect::Register v0R = mipsCollect::getSeriesV(0);
    collect.move(targetR, v0R);
}

void mipsGraphGen::genMipsPrintStr()
{
    string strName = genMips_AllocStrName();
    string value = codeWorkNow->getValue();
    collect.asciiz(strName, value);
    collect.la(mipsCollect::getSeriesA(0), strName);
    collect.syscall(4);
    collect.la(mipsCollect::getSeriesA(0), newLine);
    collect.syscall(4);
}

void mipsGraphGen::genMipsPrintStrNoNewLine()
{
    string strName = genMips_AllocStrName();
    string value = codeWorkNow->getValue();
    collect.asciiz(strName, value);
    collect.la(mipsCollect::getSeriesA(0), strName);
    collect.syscall(4);
}

void mipsGraphGen::genMipsPrintExp()
{
    symAttr *exp = codeWorkNow->getOperand1();
    LoadSymToRegisterTold(exp, mipsCollect::getSeriesA(0));
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
    collect.la(mipsCollect::getSeriesA(0), newLine);
    collect.syscall(4);
}

void mipsGraphGen::genMipsConstState()
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
        mipsCollect::Register tmp = GetOrAllocRegisterToSym(attr);
        collect.li(tmp, num);
        flushToMem(attr->SymId, tmp);
    }
    else
    {
        assert(false);
    }
}

void mipsGraphGen::genMipsVarState()
{
    symAttr *attr = codeWorkNow->getOperand1();
    if (attr->refer == GLOBAL)
    {
        collect.space(attr->name, attr->size);
    }
}

void mipsGraphGen::genMipsFunctState()
{
    symAttr *attr = codeWorkNow->getOperand1();
    mipsCollect::Register raR = mipsCollect::getSeriesRA();
    mipsCollect::Register fpR = mipsCollect::getSeriesFP();
    mipsCollect::Register spR = mipsCollect::getSeriesSP();
    mipsCollect::Register zeroR = mipsCollect::getSeriesZero();
    collect.labelLine(attr->name);
    collect.sw(raR, 4, spR);
    collect.sw(fpR, 8, spR);
    collect.sw(spR, 12, spR);
    collect.add(fpR, spR, zeroR);
    collect.add(spR, spR, attr->size);
}

void mipsGraphGen::genMipsFunctRetWithValue()
{
    symAttr *attr = codeWorkNow->getOperand1();
    mipsCollect::Register v0R = mipsCollect::getSeriesV(0);
    LoadSymToRegisterTold(attr, v0R);
    if (!codeWorkNow->isInlineRet())
        genMipsFunctRetWithoutValue();
}

void mipsGraphGen::genMipsFunctRetWithoutValue()
{
    mipsCollect::Register raR = mipsCollect::getSeriesRA();
    mipsCollect::Register fpR = mipsCollect::getSeriesFP();
    mipsCollect::Register spR = mipsCollect::getSeriesSP();
    collect.lw(spR, 12, fpR);
    collect.lw(fpR, 8, spR);
    collect.lw(raR, 4, spR);
    collect.jr(raR);
}

void mipsGraphGen::genMipsFunctArgsPush()
{
    symAttr *attr = codeWorkNow->getOperand1();
    mipsCollect::Register spR = mipsCollect::getSeriesSP();
    mipsCollect::Register tmp = LoadSymRealValueToRegister(attr);
    collect.sw(tmp, functionCallInventArgLen + spVerticalOffset, spR);
    spVerticalOffset += 4;
}
void mipsGraphGen::genMipsFunctCall()
{
    spVerticalOffset = 0;
    symAttr *attr = codeWorkNow->getOperand1();
    collect.jal(attr->name);
}

void mipsGraphGen::genMipsFunctRetUse()
{
    symAttr *attr = codeWorkNow->getOperand1();
    mipsCollect::Register tmp = GetOrAllocRegisterToSym(attr);
    mipsCollect::Register v0R = mipsCollect::getSeriesV(0);
    collect.move(tmp, v0R);
}

void mipsGraphGen::genMipsBNZ()
{
    collect.bnez(GetConditionReg(), codeWorkNow->getValue());
}
void mipsGraphGen::genMipsBZ()
{
    collect.beqz(GetConditionReg(), codeWorkNow->getValue());
}
void mipsGraphGen::genMipsJUMP()
{
    collect.jump(codeWorkNow->getValue());
}
void mipsGraphGen::genMipsLabelLine()
{
    collect.labelLine(codeWorkNow->getValue());
}
void mipsGraphGen::genMipsAssignValue()
{
    symAttr *left = codeWorkNow->getOperand1();
    symAttr *right = codeWorkNow->getOperand2();
    mipsCollect::Register leftR = GetOrAllocRegisterToSym(left);
    LoadSymToRegisterTold(right, leftR);
}
void mipsGraphGen::genMipsAssignConst()
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
    mipsCollect::Register leftR = GetOrAllocRegisterToSym(attr);
    collect.li(leftR, num);
    flushToMem(attr->SymId, leftR);
}

void mipsGraphGen::genMipsArrayValueGet()
{
    symAttr *array = codeWorkNow->getOperand2();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand1();

    mipsCollect::Register targetR = GetOrAllocRegisterToSym(target);
    mipsCollect::Register idxR = LoadSymRealValueToRegister(idx);
    mipsCollect::Register pureFree = targetR;
    collect.mul(pureFree, idxR, array->align);

    if (array->refer == GLOBAL)
    {
        string arrayName = array->name;
        collect.lw(targetR, arrayName, pureFree);
    }
    else if (array->refer == FP)
    {
        int offset2Fp = array->offsetRel;
        collect.add(pureFree, pureFree, mipsCollect::getSeriesFP());
        collect.lw(targetR, offset2Fp, pureFree);
    }
    else
    {
        assert(false);
    }
}
void mipsGraphGen::genMipsArrayValuePut()
{
    symAttr *array = codeWorkNow->getOperand1();
    symAttr *idx = codeWorkNow->getIdx();
    symAttr *target = codeWorkNow->getOperand2();

    mipsCollect::Register targetR = LoadSymRealValueToRegister(target);
    mipsCollect::Register idxR = LoadSymRealValueToRegister(idx);
    mipsCollect::Register pureFree = AllocAPureFreeReg();
    collect.mul(pureFree, idxR, array->align);

    if (array->refer == GLOBAL)
    {
        string arrayName = array->name;
        collect.sw(targetR, arrayName, pureFree);
    }
    else if (array->refer == FP)
    {
        int offset2Fp = array->offsetRel;
        collect.add(pureFree, pureFree, mipsCollect::getSeriesFP());
        collect.sw(targetR, offset2Fp, pureFree);
    }
}

void mipsGraphGen::genMipsCondition()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    symAttr *num2 = codeWorkNow->getOperand2();
    mipsCollect::Register num1R = LoadSymRealValueToRegister(num1);
    mipsCollect::Register num2R = LoadSymRealValueToRegister(num2);
    mipsCollect::Register condTmp = GetConditionReg();
    genMips_DistinguishOp(condTmp, num1R, num2R, codeWorkNow->getOp());
}
void mipsGraphGen::genMipsCondition4Num()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    mipsCollect::Register condTmp = GetConditionReg();
    LoadSymToRegisterTold(num1, condTmp);
}

void mipsGraphGen::genMipsFourYuan()
{
    symAttr *num1 = codeWorkNow->getOperand1();
    symAttr *num2 = codeWorkNow->getOperand2();
    symAttr *target = codeWorkNow->getResult();
    mipsCollect::Register num1R = LoadSymRealValueToRegister(num1);
    mipsCollect::Register num2R = LoadSymRealValueToRegister(num2);
    mipsCollect::Register targetR = GetOrAllocRegisterToSym(num2);

    genMips_DistinguishOp(targetR, num1R, num2R, codeWorkNow->getOp());
}

void mipsGraphGen::genMips_DistinguishOp(mipsCollect::Register target, mipsCollect::Register operand1, mipsCollect::Register operand2, codeSt::op_em op)
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

void mipsGraphGen::gen_mips_code()
{
    flowGraph.work();
    vector<block> blocks = flowGraph.getBlocks();
    for (int i = 0; i < blocks.size(); i++)
    {
        tempReg.resetLocalPool();
        vector<codeSt> codes = blocks[i].getCodes();
        for (int i = 0; i < codes.size(); i++)
        {
            codeWorkNow = &codes[i];
            cout << codeWorkNow->to_string();
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
}
