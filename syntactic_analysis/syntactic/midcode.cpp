#include "midcode.h"
#include "debug.h"
//codeSt
codeSt::codeSt(codeType codetype)
{
    this->codetype = codetype;
}

codeSt::codeSt(codeType codetype, string value)
{
    this->codetype = codetype;
    this->value_const_str = value;
}
codeSt::codeSt(codeType codetype, symAttr *operand1)
{
    this->codetype = codetype;
    this->operand1 = operand1;
}

codeSt::codeSt(codeType codetype, symAttr *operand1, symAttr *operand2)
{
    this->codetype = codetype;
    this->operand1 = operand1;
    this->operand2 = operand2;
}

codeSt::codeSt(codeType codetype, symAttr *operand1, string value)
{
    this->codetype = codetype;
    this->operand1 = operand1;
    this->value_const_str = value;
}
codeSt::codeSt(codeType codetype, symAttr *operand1, symAttr *idx, symAttr *operand2)
{
    this->codetype = codetype;
    this->operand1 = operand1;
    this->operand2 = operand2;
    this->idx = idx;
}
codeSt::codeSt(codeType codetype, symAttr *operand1, op_em op, symAttr *operand2)
{
    this->codetype = codetype;
    this->operand1 = operand1;
    this->operand2 = operand2;
    this->op = op;
}

codeSt::codeSt(codeType codetype, symAttr *operand1, op_em op, symAttr *operand2, symAttr *result)
{
    this->codetype = codetype;
    this->operand1 = operand1;
    this->operand2 = operand2;
    this->result = result;
    this->op = op;
}

//midCodeGen
string midCodeGen::genMid_AllocTmp(symType type)
{
    tmpGen++;
    string name = tmp_prefix + to_string(tmpGen);
    symAttr attr = {name, type, VAR};
    symbolist.insert(attr);
    return name;
}

string midCodeGen::genMid_AllocLabel()
{
    labelGen++;
    return label_prefix + to_string(labelGen);
}

void midCodeGen::genMidFuncDef(string funcName)
{
    codeSt funcDef(codeSt::FunctState, symbolist.get_pointer(funcName));
    codes.push_back(funcDef);
}

void midCodeGen::genMidArgsPush(string paraName)
{
    codeSt para(codeSt::FunctArgsPush, symbolist.get_pointer(paraName));
    codes.push_back(para);
}
void midCodeGen::genMidFuncCall(string func)
{
    codeSt call(codeSt::FunctCall, symbolist.get_pointer(func));
    codes.push_back(call);
}
void midCodeGen::genMidFuncRet(string funcName, string name)
{
    if (name == "")
    {
        codeSt retWithoutValue(codeSt::FunctRetWithoutValue, symbolist.get_pointer(funcName));
        codes.push_back(retWithoutValue);
    }
    else
    {
        assert(symbolist.has(name));
        codeSt retWithValue(codeSt::FunctRetWithValue, symbolist.get_pointer(funcName), symbolist.get_pointer(name));
        codes.push_back(retWithValue);
    }
}

string midCodeGen::genMidFuncRetUse(string funcName)
{
    string tmp = genMid_AllocTmp(symbolist.get_pointer(funcName)->type);
    codeSt useRet(codeSt::FunctRetUse, symbolist.get_pointer(tmp));
    codes.push_back(useRet);
    return tmp;
}

void midCodeGen::genMidVarState(string name)
{
    codeSt varState(codeSt::ConstVarState, symbolist.get_pointer(name));
    codes.push_back(varState);
}
void midCodeGen::genMidConstState(string name)
{
    codeSt varState(codeSt::ConstVarState, symbolist.get_pointer(name));
    codes.push_back(varState);
}

void midCodeGen::genMidLabelLine(string Label)
{
    codeSt labelLine(codeSt::Label, Label);
    codes.push_back(labelLine);
}

string midCodeGen::genMidConstTmp(symType type, string value)
{
    string tmp = genMid_AllocTmp(type);
    codeSt constTmp(codeSt::AssignConst, symbolist.get_pointer(tmp), value);
    codes.push_back(constTmp);
    return tmp;
}

string midCodeGen::genMidExpress(string operand1, token_key op, string operand2)
{
    string tmp = genMid_AllocTmp(INT);
    codeSt exp(codeSt::FourYuan, symbolist.get_pointer(operand1), codeSt::token_key2op_em(op), symbolist.get_pointer(operand2), symbolist.get_pointer(tmp));
    codes.push_back(exp);
    return tmp;
}

string midCodeGen::genMidValueGet(string name)
{
    symAttr *attr = symbolist.get_pointer(name);
    string tmp = genMid_AllocTmp(attr->type);
    codeSt valueGet(codeSt::AssignValue, symbolist.get_pointer(tmp), attr);
    return tmp;
}

void midCodeGen::genMidValuePut(string name, string value)
{
    codeSt valuePut(codeSt::AssignValue, symbolist.get_pointer(name), symbolist.get_pointer(value));
    codes.push_back(valuePut);
}

string midCodeGen::genMidArrayValueGet(string array, string idx)
{
    symAttr *attr = symbolist.get_pointer(array);
    string tmp = genMid_AllocTmp(attr->type);
    codeSt arrayValueGet(codeSt::ArrayValueGet, symbolist.get_pointer(tmp), attr, symbolist.get_pointer(idx));
    codes.push_back(arrayValueGet);
    return tmp;
}

void midCodeGen::genMidArrayValuePut(string array, string idx, string value)
{
    codeSt valuePut(codeSt::ArrayValuePut, symbolist.get_pointer(array), symbolist.get_pointer(idx), symbolist.get_pointer(value));
    codes.push_back(valuePut);
}

void midCodeGen::genMidCondition(string operand1, token_key op, string operand2)
{
    codeSt::op_em opem = codeSt::token_key2op_em(op);
    codeSt condition(codeSt::Condition, symbolist.get_pointer(operand1), opem, symbolist.get_pointer(operand2));
    codes.push_back(condition);
}

void midCodeGen::genMidCondition4Num(string operand1)
{
    codeSt condition(codeSt::Condition4Num, symbolist.get_pointer(operand1));
    codes.push_back(condition);
}

void midCodeGen::genMidGoto(string Label)
{
    codeSt Goto(codeSt::Jump, Label);
    codes.push_back(Goto);
}
void midCodeGen::genMidBNZ(string Label)
{
    codeSt BNZ(codeSt::BNZ, Label);
    codes.push_back(BNZ);
}
void midCodeGen::genMidBZ(string Label)
{
    codeSt BZ(codeSt::BZ, Label);
    codes.push_back(BZ);
}
void midCodeGen::genMidScanf(string name)
{
    codeSt scanf(codeSt::Scanf, symbolist.get_pointer(name));
    codes.push_back(scanf);
}
void midCodeGen::genMidPrintfStr(string str)
{
    codeSt printStr(codeSt::PrintStr, str);
    codes.push_back(printStr);
}
void midCodeGen::genMidPrintfExp(string name)
{
    codeSt printExp(codeSt::PrintExp, symbolist.get_pointer(name));
    codes.push_back(printExp);
}

//to_string()
string codeSt::to_string()
{
    int ret = (int)codetype;
    return std::to_string(ret);
}

// void midCodeGen::genMidFuncDef(string funcName)
// {
//     symAttr funcAttr = symbolist.get(funcName);
//     string type_str;
//     if (funcAttr.type == VOID)
//     {
//         type_str = "void";
//     }
//     else if (funcAttr.type == INT)
//     {
//         type_str = "int";
//     }
//     else if (funcAttr.type == CHAR)
//     {
//         type_str = "char";
//     }
//     else
//     {
//         type_str = "UNKNOWN_ERROR";
//     }
//     midFile << type_str << " " << funcAttr.name << "()" << endl;
// }
// void midCodeGen::genMidArgsPush(string paraName) /*标识符或常量*/
// {
//     midFile << "push " << paraName << endl;
// }
// void midCodeGen::genMidFuncCall(string func)
// {
//     midFile << "call " << func << endl;
// }
// void midCodeGen::genMidFuncRet(string name)
// {
//     midFile << "ret " << name << endl;
// }
// string midCodeGen::genMidFuncRetUse()
// {
//     string temp_val = genMid_AllocTmp();
//     midFile << temp_val << "=RET" << endl;
//     return temp_val;
// }

// void midCodeGen::genMidVarState(string name)
// {
//     symAttr varAttr = symbolist.get(name);
//     string type_str;
//     if (varAttr.type == VOID)
//     {
//         type_str = "void";
//     }
//     else if (varAttr.type == INT)
//     {
//         type_str = "int";
//     }
//     else if (varAttr.type == CHAR)
//     {
//         type_str = "char";
//     }
//     else
//     {
//         type_str = "UNKNOWN_ERROR";
//     }
//     if (varAttr.len == 0)
//     {
//         midFile << "var " << type_str << " " << name << "@" << varAttr.SymId << endl;
//     }
//     else
//     {
//         midFile << "var " << type_str << " " << name << "[" << varAttr.len << "]"
//                 << "@" << varAttr.SymId << endl;
//     }
// }
// void midCodeGen::genMidConstState(string name)
// {
//     symAttr varAttr = symbolist.get(name);
//     string type_str;
//     if (varAttr.type == VOID)
//     {
//         type_str = "void";
//     }
//     else if (varAttr.type == INT)
//     {
//         type_str = "int";
//     }
//     else if (varAttr.type == CHAR)
//     {
//         type_str = "char";
//     }
//     else
//     {
//         type_str = "UNKNOWN_ERROR";
//     }
//     midFile << "const " << type_str << " " << name << "@" << varAttr.SymId << endl;
// }

// string midCodeGen::genMid_AllocLabel()
// {
//     string ret = "Label" + to_string(label_idx);
//     label_idx++;
//     return ret;
// }
// void midCodeGen::genMidLabelLine(string Label)
// {
//     midFile << Label << ":" << endl;
// }
// void midCodeGen::genMid_ResetTmp()
// {
//     temp_idx = 0;
// }
// string midCodeGen::genMid_AllocTmp()
// {
//     string ret = "_tmp" + to_string(temp_idx);
//     temp_idx++;
//     return ret;
// }
// string midCodeGen::genMidExpress(string operand1, string op, string operand2)
// {
//     string ret = genMid_AllocTmp();
//     string p1;
//     string p2;
//     if (symbolist.has(operand1))
//     {
//         symAttr pAttr = symbolist.get(operand1);
//         p1 = operand1 + "@" + to_string(pAttr.SymId);
//     }
//     else
//     {
//         p1 = operand1;
//     }
//     if (symbolist.has(operand2))
//     {
//         symAttr pAttr = symbolist.get(operand2);
//         p2 = operand2 + "@" + to_string(pAttr.SymId);
//     }
//     else
//     {
//         p2 = operand2;
//     }
//     midFile << ret << "=" << p1 << op << p2 << endl;
//     return ret;
// }
// string midCodeGen::genMidValueGet(string name)
// {
//     string ret = genMid_AllocTmp();
//     symAttr nameAttr = symbolist.get(name);
//     midFile << ret << "=" << name << "@" << nameAttr.SymId << endl;
//     return ret;
// }
// void midCodeGen::genMidValuePut(string name, string value)
// {
//     symAttr nameAttr = symbolist.get(name);
//     midFile << name << "@" << nameAttr.SymId << "=" << value << endl;
// }
// string midCodeGen::genMidArrayValueGet(string array, string idx)
// {
//     string ret = genMid_AllocTmp();
//     symAttr arrayAttr = symbolist.get(array);
//     midFile << ret << "=" << array << "@" << arrayAttr.SymId << "[" << idx
//             << "]" << endl;
//     return ret;
// }
// void midCodeGen::genMidArrayValuePut(string array, string idx, string value)
// {
//     symAttr arrayAttr = symbolist.get(array);
//     midFile << array << "@" << arrayAttr.SymId << "[" << idx << "]"
//             << "=" << value << endl;
// }

// void midCodeGen::genMidCondition(string operand1, string op, string operand2)
// {
//     midFile << operand1 << op << operand2 << endl;
// }
// void midCodeGen::genMidCondition4Num(string operand1)
// {
//     midFile << operand1 << "!=0" << endl;
// }
// void midCodeGen::genMidGoto(string Label)
// {
//     midFile << "goto " << Label << endl;
// }
// void midCodeGen::genMidBNZ(string Label)
// {
//     midFile << "BNZ " << Label << endl;
// }
// void midCodeGen::genMidBZ(string Label)
// {
//     midFile << "BZ" << Label << endl;
// }

// void midCodeGen::genMidScanf(string name)
// {
//     symAttr scanfAttr = symbolist.get(name);
//     midFile << "scanf " << scanfAttr.name << "@" << scanfAttr.SymId << endl;
// }
// void midCodeGen::genMidPrintfStr(string str)
// {
//     midFile << "printStr:" << str << endl;
// }
// void midCodeGen::genMidPrintfExp(string name)
// {
//     string type_str;
//     if (type == INT)
//     {
//         type_str = "INT";
//     }
//     else if (type == CHAR)
//     {
//         type_str = "CHAR";
//     }
//     else
//     {
//         type_str = "UNKNOWN ERROR";
//     }
//     midFile << "printExp:" << name << "%" << type_str << endl;
// }