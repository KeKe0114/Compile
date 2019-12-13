#include "errorMags.h"
#include <sstream>

errorMags::errorMags() {}
errorMags::erritem::erritem(int line, ERRCODE code) : line(line), code(code) {}

void errorMags::errPut(int line, ERRCODE code)
{
    errorMags::erritem item(line, code);
    errlist.push_back(item);
}
bool errorMags::hasErrors()
{
    return errlist.size() > 0;
}
string errorMags::getErrorsStr()
{
    stringstream ss;
    for (int i = 0; i < errlist.size(); i++)
    {
        ss << errlist[i].getLine() << " " << errlist[i].getCodeStr() << endl;
    }
    return ss.str();
}
int errorMags::erritem::getLine()
{
    return line;
}
string errorMags::erritem::getCodeStr()
{
    string s("a");
    s[0] = 'a' + code;
    return s;
}