#include "syntacticAnalysis.h"
#include "mips.h"

int main(int argc, char const *argv[])
{
    syntacticAnalysis syntactic("testfile.txt", "output.txt");
    syntactic.procedureCheck();
    mipsGen &mips = mipsGen::get_instance();
    mips.gen_mips_code();
    string ss = mips.get_mips_str();
    cout << ss;
    return 0;
}