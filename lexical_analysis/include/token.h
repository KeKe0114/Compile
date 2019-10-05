#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <set>
#include "debug.h"
using namespace std;

//token key 以及映射:key转name,key转value
enum token_key
{
    IDENFR,

    INTCON,
    CHARCON,
    STRCON,

    CONSTTK,
    INTTK,
    CHARTK,
    VOIDTK,
    MAINTK,
    IFTK,
    ELSETK,
    DOTK,
    WHILETK,
    FORTK,
    SCANFTK,
    PRINTFTK,
    RETURNTK,

    PLUS,
    MINU,
    MULT,
    DIV,
    LSS,
    LEQ,
    GRE,
    GEQ,
    EQL,
    NEQ,
    ASSIGN,

    SEMICN,
    COMMA,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
};
const string TOKEN_NAME[] = {
    "IDENFR",
    "INTCON",
    "CHARCON",
    "STRCON",

    "CONSTTK",
    "INTTK",
    "CHARTK",
    "VOIDTK",
    "MAINTK",
    "IFTK",
    "ELSETK",
    "DOTK",
    "WHILETK",
    "FORTK",
    "SCANFTK",
    "PRINTFTK",
    "RETURNTK",

    "PLUS",
    "MINU",
    "MULT",
    "DIV",
    "LSS",
    "LEQ",
    "GRE",
    "GEQ",
    "EQL",
    "NEQ",
    "ASSIGN",

    "SEMICN",
    "COMMA",
    "LPARENT",
    "RPARENT",
    "LBRACK",
    "RBRACK",
    "LBRACE",
    "RBRACE"};

const string TOKEN_VALUE[] = {
    "",
    "",
    "",
    "",
    "const",
    "int",
    "char",
    "void",
    "main",
    "if",
    "else",
    "do",
    "while",
    "for",
    "scanf",
    "printf",
    "return",

    "+",
    "-",
    "*",
    "/",
    "<",
    "<=",
    ">",
    ">=",
    "==",
    "!=",
    "=",

    ";",
    ",",
    "(",
    ")",
    "[",
    "]",
    "{",
    "}"};

//**********************************************
// 描述 token_key 表的一些常量.区间为[BEGIN, END)
// RESERVED集合 为保留字,OP 以及
// [](){};,等扫描到就可以确定类别的token
// VALUE_REQUIRED集合的value应经过解析得到
#define RESERVED_BEGIN 4
#define RESERVED_END 36
#define VALUE_REQUIRED_BEGIN 0
#define VALUE_REQUIRED_END 4
//**********************************************
// 描述Op的第一个字符和第二个字符
// 描述什么是单字符
const set<char> OpPrefix{'<', '>', '=', '!'};
const set<char> OpSecond{'='};
const set<char> Single{',', ';', '(', ')', '[', ']', '{', '}', '+', '-', '*', '/'};
bool isOpPrefix(char c);
bool isOpSecond(char c);
bool isSingle(char c);
//**********************************************
class token
{
private:
    token_key key;
    string value;

public:
    token(token_key key);
    token(token_key key, string value);
    token_key getKey();
    string getName();
    string getValue();
};
#endif