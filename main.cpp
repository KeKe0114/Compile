#include "syntacticAnalysis.h"
#include "mipsGen.h"
#include "mipsGraphGen.h"
#include "dataFlow.h"
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
        // blockFlowGraph &flowGraph = blockFlowGraph::get_instance();
        // flowGraph.work();
        // mipsGen &mips = mipsGen::get_instance();
        // mips.gen_mips_code();
        mipsGraphGen &mips = mipsGraphGen::get_instance();
        mips.gen_mips_code();
        string ss = mips.get_mips_str();
        ofstream out("mips.txt");
        out << ss;
    }
    return 0;
}