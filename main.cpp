#include "syntacticAnalysis.h"
#include "mipsGen.h"
#include "mipsGraphGen.h"
#include "globalRegMag.h"
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
        blockFlowGraph &flowGraph = blockFlowGraph::get_instance();
        flowGraph.genfuncDivide();

        globalRegMag &globalReg = globalRegMag::get_instance();
        vector<funcScope> Scopes = flowGraph.getFuncScopes();
        for (int i = 0; i < Scopes.size(); i++)
        {
            funcScopeRegMag funcReg;
            vector<block> blocks = Scopes[i].getBlocks();
            for (int j = 0; j < blocks.size(); j++)
            {
                set<int> conflict = blocks[j].getBlockAliveIn();
                set<int> conTmp = blocks[j].getBlockAliveOut();
                conflict.insert(conTmp.begin(), conTmp.end());
                funcReg.addConflictFamily(conflict);
            }
            funcReg.genAllocResult();
            globalReg.checkInToBeMag(Scopes[i].getName(), funcReg);
        }

        mipsGraphGen &mips = mipsGraphGen::get_instance();
        mips.gen_mips_code();
        string ss = mips.get_mips_str();
        ofstream out("mips.txt");
        out << ss;
    }
    return 0;
}