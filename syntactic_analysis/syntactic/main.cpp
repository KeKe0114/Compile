#include "syntacticAnalysis.h"
#include "mips.h"
#include "errorMags.h"

int main(int argc, char const *argv[])
{
    syntacticAnalysis syntactic("testfile.txt", "output.txt");
    syntactic.procedureCheck();
    errorMags &error = errorMags::get_instance();
    if (error.hasErrors())
    {
        cout << error.getErrorsStr();
    }
    else
    {
        mipsGen &mips = mipsGen::get_instance();
        mips.gen_mips_code();
        string ss = mips.get_mips_str();
        ofstream out("mips.txt");
        out << ss;
    }
    return 0;
}