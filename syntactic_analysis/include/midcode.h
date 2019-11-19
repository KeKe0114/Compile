#include <string>
#include "symbols.h"
#include "token.h"
using namespace std;
class codeSt
{
public:
    enum codeType
    {
        //读语句
        Scanf, //opeand1
        //写语句
        PrintStr, //value
        PrintExp, //operand1
        //常变量声明
        ConstVarState, //operand1
        //函数定义
        FunctState,           //operand1
        FunctRetWithValue,    //operand1 operand2
        FunctRetWithoutValue, //operand1
        //函数调用
        FunctArgsPush, //operand1
        FunctCall,     //operand1
        FunctRetUse,   //operand1
        //跳转
        BNZ,  //value
        BZ,   //value
        Jump, //value
        //Label
        Label, // value

        //伪三元
        AssignValue, //operand1,operand2

        //三元式
        AssignConst,   //operand1,value
        ArrayValueGet, //operand1,operand2,idx
        ArrayValuePut, //operand1,idx,operand2
        Condition,     //operand1,op,operand2
        Condition4Num, //operand1

        //四元式
        FourYuan, //operand1,op,operand2,result
    };

    enum op_em
    {
        op_UNKNOWNERROR = -1,
        op_PLUS = 17,
        op_MINU,
        op_MULT,
        op_DIV,
        op_LSS,
        op_LEQ,
        op_GRE,
        op_GEQ,
        op_EQL,
        op_NEQ,
    };

private:
    codeType codetype;

    string value_const_str;
    symAttr *operand1;
    symAttr *operand2;
    symAttr *result;
    op_em op;
    symAttr *idx;
    codeSt();

public:
    string getValue() { return value_const_str; }
    symAttr *getOperand1() { return operand1; }
    symAttr *getOperand2() { return operand2; }
    symAttr *getResult() { return result; }
    symAttr *getIdx() { return idx; }
    op_em getOp() { return op; }
    codeType getType() { return codetype; }

public:
    static op_em token_key2op_em(token_key key)
    {
        if (key <= 26 && key >= 17)
            return (op_em)(int)(key);
        return op_UNKNOWNERROR;
    }

public:
    codeSt(codeType codetype);

    codeSt(codeType codetype, string value);
    codeSt(codeType codetype, symAttr *operand1);

    codeSt(codeType codetype, symAttr *operand1, symAttr *operand2);

    codeSt(codeType codetype, symAttr *operand1, string value);
    codeSt(codeType codetype, symAttr *operand1, symAttr *idx, symAttr *operand2);
    codeSt(codeType codetype, symAttr *operand1, op_em op, symAttr *operand2);

    codeSt(codeType codetype, symAttr *operand1, op_em op, symAttr *operand2, symAttr *result);

    string to_string();
};

class midCodeGen
{
private:
    midCodeGen() : symbolist(symbols::get_instance()), label_prefix("Label"), tmp_prefix("$tmp") {}
    midCodeGen(const midCodeGen &) = delete;
    midCodeGen &operator&(const midCodeGen &) = delete;

public:
    static midCodeGen &get_instance()
    {
        static midCodeGen instance;
        return instance;
    }

private:
    symbols &symbolist;
    vector<codeSt> codes;
    int labelGen;
    int tmpGen;
    string label_prefix;
    string tmp_prefix;

    string genMid_AllocTmp(symType type);

public:
    void genMidFuncDef(string funcName);

    void genMidArgsPush(string paraName);
    void genMidFuncCall(string func);
    void genMidFuncRet(string funcName, string name);
    string genMidFuncRetUse(string funcName);

    void genMidVarState(string name);
    void genMidConstState(string name);

    string genMid_AllocLabel();
    void genMidLabelLine(string Label);
    string genMidConstTmp(symType type, string value);
    string genMidExpress(string operand1, token_key op, string operand2);
    string genMidValueGet(string name);
    void genMidValuePut(string name, string value);
    string genMidArrayValueGet(string array, string idx);
    void genMidArrayValuePut(string array, string idx, string value);

    void genMidCondition(string operand1, token_key op, string operand2);
    void genMidCondition4Num(string operand1);
    void genMidGoto(string Label);
    void genMidBNZ(string Label);
    void genMidBZ(string Label);

    void genMidScanf(string name);
    void genMidPrintfStr(string str);
    void genMidPrintfExp(string name);
};