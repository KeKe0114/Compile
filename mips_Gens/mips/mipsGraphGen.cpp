#include "mipsGraphGen.h"
#include "debug.h"

string mipsGraphGen::genMips_AllocStrName()
{
    strIdGen++;
    return str_prefix + to_string(strIdGen);
}

void mipsGraphGen::flushToMem(int symId, mipsCollect::Register reg)
{
    symAttr *attr = symbolist.get_pointer_by_id(symId);
    if (attr->kind != VAR)
        return;
    if (attr->refer == GLOBAL)
        collect.sw(reg, attr->name);
    else if (attr->refer == FP)
        collect.sw(reg, attr->offsetRel, mipsCollect::getSeriesFP());
    else
        assert(false);
}
void mipsGraphGen::flushToMemIfGlobal(int symId, mipsCollect::Register reg)
{
    symAttr *attr = symbolist.get_pointer_by_id(symId);
    if (attr->refer == GLOBAL)
    {
        flushToMem(symId, reg);
    }
}

void mipsGraphGen::loadFromMem(int symId, mipsCollect::Register reg)
{
    symAttr *attr = symbolist.get_pointer_by_id(symId);
    if (attr->kind == CONST)
    {
        collect.li(reg, attr->getValueInt());
    }
    else if (attr->kind == VAR)
    {
        if (attr->refer == GLOBAL)
        {
            collect.lw(reg, attr->name);
        }
        else if (attr->refer == FP)
        {
            collect.lw(reg, attr->offsetRel, collect.getSeriesFP());
        }
    }
    else
    {
        assert(false);
    }
}

mipsCollect::Register mipsGraphGen::GetOrAllocRegisterToSym(symAttr *attr)
{
    if (globalReg.hasRegForSym(attr->SymId))
    {
        return collect.getSeriesS(globalReg.getRegForSym(attr->SymId));
    }
    // 走到这里:要么是全局变量但是没有寄存器,要么是局部变量
    if (tempReg.hasThisInReg(attr->SymId))
    {
        //在临时变量池中.
        int tmpId = tempReg.getRegForThis(attr->SymId);
        return collect.getSeriesT(tmpId);
    }
    else
    {
        //不在临时变量池中.
        if (tempReg.hasFreeReg())
        {
            //有free寄存器.
            int tmpId = tempReg.getAFreeRegForThis(attr->SymId);
            return collect.getSeriesT(tmpId);
        }
        else
        {
            //没有free寄存器.
            flushSt shouldFlush = tempReg.flushASymNotUseNow(codeWorkNow->getRightValue());
            flushToMem(shouldFlush.getSymId(), collect.getSeriesT(shouldFlush.getRegId()));
            int tmpId = tempReg.getAFreeRegForThis(attr->SymId);
            return collect.getSeriesT(tmpId);
        }
    }
}

mipsCollect::Register mipsGraphGen::LoadSymRealValueToRegister(symAttr *attr)
{
    if (globalReg.hasRegForSym(attr->SymId))
    {
        return collect.getSeriesS(globalReg.getRegForSym(attr->SymId));
    }
    // 走到这里:要么是全局变量但是没有寄存器,要么是局部变量
    if (tempReg.hasThisInReg(attr->SymId))
    {
        //在临时变量池中.
        int tmpId = tempReg.getRegForThis(attr->SymId);
        return collect.getSeriesT(tmpId);
    }
    else
    {
        //不在临时变量池中.
        if (tempReg.hasFreeReg())
        {
            //有free寄存器.
            int tmpId = tempReg.getAFreeRegForThis(attr->SymId);
            mipsCollect::Register ans = collect.getSeriesT(tmpId);
            loadFromMem(attr->SymId, ans);
            return ans;
        }
        else
        {
            //没有free寄存器.
            flushSt shouldFlush = tempReg.flushASymNotUseNow(codeWorkNow->getRightValue());
            flushToMem(shouldFlush.getSymId(), collect.getSeriesT(shouldFlush.getRegId()));
            int tmpId = tempReg.getAFreeRegForThis(attr->SymId);
            mipsCollect::Register ans = collect.getSeriesT(tmpId);
            loadFromMem(attr->SymId, ans);
            return ans;
        }
    }
}

void mipsGraphGen::LoadSymToRegisterTold(symAttr *attr, mipsCollect::Register told)
{
    if (globalReg.hasRegForSym(attr->SymId))
    {
        mipsCollect::Register ans = collect.getSeriesS(globalReg.getRegForSym(attr->SymId));
        collect.move(told, ans);
        return;
    }
    // 走到这里:要么是全局变量但是没有寄存器,要么是局部变量
    if (tempReg.hasThisInReg(attr->SymId))
    {
        //在临时变量池中.
        int tmpId = tempReg.getRegForThis(attr->SymId);
        mipsCollect::Register ans = collect.getSeriesT(tmpId);
        collect.move(told, ans);
        return;
    }
    else
    {
        //不在临时变量池中.
        loadFromMem(attr->SymId, told);
        return;
    }
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
    flushToMemIfGlobal(target->SymId, targetR);
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
    int num = attr->getValueInt();
    if (attr->refer == GLOBAL)
    {
        collect.word(attr->name, num);
    }
    else if (attr->refer == FP)
    {
        mipsCollect::Register tmp = GetOrAllocRegisterToSym(attr);
        collect.li(tmp, num);
        // flushToMem(attr->SymId, tmp);
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
    flushToMemIfGlobal(attr->SymId, tmp);
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
    flushToMemIfGlobal(left->SymId, leftR);
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
    flushToMemIfGlobal(attr->SymId, leftR);
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
        flushToMemIfGlobal(target->SymId, targetR);
    }
    else if (array->refer == FP)
    {
        int offset2Fp = array->offsetRel;
        collect.add(pureFree, pureFree, mipsCollect::getSeriesFP());
        collect.lw(targetR, offset2Fp, pureFree);
        flushToMemIfGlobal(target->SymId, targetR);
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
    flushToMemIfGlobal(target->SymId, targetR);
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
    vector<codeSt> globalStates = flowGraph.getGlobalState();
    for (int i = 0; i < globalStates.size(); i++)
    {
        codeWorkNow = &globalStates[i];
        cout << i << " " << codeWorkNow->getType() << "\t" << codeWorkNow->to_string();
        if (codeWorkNow->getType() == codeSt::ConstVarState)
        {
            if (codeWorkNow->getOperand1()->kind == CONST)
                genMipsConstState();
            else if (codeWorkNow->getOperand1()->kind == VAR)
                genMipsVarState();
            else
                assert(false);
        }
        else
            assert(false);
    }
    vector<funcScope> Scopes = flowGraph.getFuncScopes();
    for (int i = 0; i < Scopes.size(); i++)
    {
        vector<block> blocks = Scopes[i].getBlocks();
        for (int j = 0; j < blocks.size(); j++)
        {
            blockWorkNow = &blocks[j];
            tempReg.resetLocalPool();
            vector<codeSt> codes = blocks[j].getCodes();
            for (int h = 0; h < codes.size(); h++)
            {
                codeWorkNow = &codes[h];
                // tempReg.SHOW_USEDREG();
                tempReg.updataUsefulInfo(blockWorkNow->getCodeIn(h));
                // tempReg.SHOW_USEDREG();
                handOutToSolve(j, h);
            }
            set<int> symsUseReg = tempReg.askAllSymUseRegNow();
            for (auto sym : symsUseReg)
            {
                if (globalReg.VarPassBlocks(sym))
                {
                    flushToMem(sym, collect.getSeriesT(tempReg.getRegForThis(sym)));
                }
            }
        }
    }
}

void mipsGraphGen::handOutToSolve(int blockNum, int codeNum)
{
    cout << blockNum << " " << codeNum << " " << codeWorkNow->getType() << "\t" << codeWorkNow->to_string();
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