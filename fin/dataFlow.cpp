#include "dataFlow.h"
#include <algorithm>

void block::genBlockUseAndDef()
{
    vector<codeSt>::iterator iter;
    for (iter = codeInBlock.begin(); iter != codeInBlock.end(); iter++)
    {
        set<int>::iterator right;
        set<int> rightSet = iter->getRightValue();
        for (right = rightSet.begin(); right != rightSet.end(); right++)
        {
            if (def.find(*right) == def.end())
            {
                use.insert(*right);
            }
        }
        set<int>::iterator left;
        set<int> leftSet = iter->getLeftValue();
        for (left = leftSet.begin(); left != leftSet.end(); left++)
        {
            if (use.find(*left) == use.end())
            {
                def.insert(*left);
            }
        }
    }
    // SHOW_CODE_AND_USE_DEF();
}

void block::setBlockUseDefIn(set<int> alivein)
{
    this->alivein = alivein;
}

void block::setBlockUseDefOut(set<int> aliveout)
{
    this->aliveout = aliveout;
}

void block::genUseDefInOutForSingleLine()
{
    for (int i = 0; i < codeInBlock.size(); i++)
    {
        idx2alivein.insert(pair<int, set<int>>(i, set<int>()));
        idx2aliveout.insert(pair<int, set<int>>(i, set<int>()));
    }
    idx2aliveout[codeInBlock.size() - 1].insert(aliveout.begin(), aliveout.end());
    //work
    for (int i = codeInBlock.size() - 1; i >= 0; i--)
    {
        //calculate idx2aliveout[i] = union children's id2alivein
        if (i != codeInBlock.size() - 1)
        {
            idx2aliveout[i] = idx2alivein[i + 1];
        }
        //work id2alivein[i] =  use union ( out[i] - def )
        set<int> temp;
        set<int> def = codeInBlock[i].getLeftValue();
        set<int> use = codeInBlock[i].getRightValue();
        set_difference(idx2aliveout[i].begin(), idx2aliveout[i].end(), def.begin(), def.end(), inserter(idx2alivein[i], idx2alivein[i].begin()));
        idx2alivein[i].insert(use.begin(), use.end());
    }
}

void funcScope::genBlocksFromOrigin()
{
    map<int, string> blockId2dstLabel;
    map<string, int> entryLabel2block;

    int begin, end, id;
    begin = 0;
    for (int i = 0; i < originCodes.size(); i++)
    {
        if (i > 0 && originCodes[i].isBlockStart() && originCodes[i - 1].isBlockEnd())
        {
            // build graph : label
            int idNow = id2block.size();
            string entryLabel = originCodes[i].getBlockEntry();
            entryLabel2block.insert(pair<string, int>(entryLabel, idNow));
        }
        else if (originCodes[i].isBlockStart())
        {
            //build block
            int idPrev = id2block.size();
            block blockPrev(idPrev, vector<codeSt>(originCodes.begin() + begin, originCodes.begin() + i));
            blockPrev.genBlockUseAndDef();
            id2block.push_back(blockPrev);
            begin = i;

            // build graph : label
            int idNow = id2block.size();
            string entryLabel = originCodes[i].getBlockEntry();
            entryLabel2block.insert(pair<string, int>(entryLabel, idNow));

            // build graph : direct child
            if (originCodes[i - 1].canGodown())
            {
                addRelationFatherChild(idPrev, idNow);
            }
        }
        else if (originCodes[i].isBlockEnd())
        {
            // build block
            int idNow = id2block.size();
            block blockTemp(idNow, vector<codeSt>(originCodes.begin() + begin, originCodes.begin() + i + 1));
            blockTemp.genBlockUseAndDef();
            id2block.push_back(blockTemp);
            begin = i + 1;

            //build graph : label
            string dstLabel = originCodes[i].getBlockDst();
            blockId2dstLabel.insert(pair<int, string>(idNow, dstLabel));

            //build graph : direct child
            if (originCodes[i].canGodown())
            {
                addRelationFatherChild(idNow, idNow + 1);
            }
        }
    }
    end = originCodes.size();
    id = id2block.size();
    block blockTemp(id, vector<codeSt>(originCodes.begin() + begin, originCodes.begin() + end));
    id2block.push_back(blockTemp);

    // build father child
    map<int, string>::iterator it;
    for (it = blockId2dstLabel.begin(); it != blockId2dstLabel.end(); it++)
    {
        int father = it->first;
        map<string, int>::iterator child_it = entryLabel2block.find(it->second);
        if (child_it == entryLabel2block.end())
        {
            cout << it->second << endl;
        }
        assert(child_it != entryLabel2block.end());
        int child = child_it->second;
        addRelationFatherChild(father, child);
    }
}

void funcScope::genUseDefInOut()
{
    //init
    for (int i = 0; i < id2block.size(); i++)
    {
        id2alivein.insert(pair<int, set<int>>(i, set<int>()));
        id2aliveout.insert(pair<int, set<int>>(i, set<int>()));
    }
    //work
    bool noChange = false;
    while (!noChange)
    {
        noChange = true;
        for (int i = id2block.size() - 1; i >= 0; i--)
        {
            //calculate id2aliveout[i] = union children's id2alivein
            set<int>::iterator iter;
            for (iter = id2child[i].begin(); iter != id2child[i].end(); iter++)
            {
                set<int> temp = id2alivein[*iter];
                id2aliveout[i].insert(temp.begin(), temp.end());
            }
            //work id2alivein[i] =  use union ( out[i] - def )
            int oldSize = id2alivein[i].size();
            set<int> use = id2block[i].getBlockUse();
            set<int> def = id2block[i].getBlockDef();
            set_difference(id2aliveout[i].begin(), id2aliveout[i].end(), def.begin(), def.end(), inserter(id2alivein[i], id2alivein[i].begin()));
            id2alivein[i].insert(use.begin(), use.end());
            if (noChange && oldSize < id2alivein[i].size())
                noChange = false;
        }
    }
}

void funcScope::setBlockInAndOut()
{
    for (int i = 0; i < id2block.size(); i++)
    {
        id2block[i].setBlockUseDefOut(id2aliveout[i]);
        id2block[i].setBlockUseDefIn(id2alivein[i]);
    }
    for (int i = 0; i < id2block.size(); i++)
    {
        id2block[i].genUseDefInOutForSingleLine();
    }
}

void blockFlowGraph::genfuncDivide()
{
    vector<codeSt> originCodes = midCodeGen::get_instance().getCodesVector();
    vector<int> funcStart;
    for (int i = 0; i < originCodes.size(); i++)
    {
        if (originCodes[i].isFuncStart())
        {
            funcStart.push_back(i);
        }
    }
    cout << "****************";
    for (auto i : funcStart)
    {
        cout << i << " ";
    }
    cout << endl;
    assert(funcStart.size() >= 1);
    globalVarStates.insert(globalVarStates.end(), originCodes.begin(), originCodes.begin() + funcStart[0]);
    for (int i = 0; i < funcStart.size() - 1; i++)
    {
        /* [start, end) */
        int start = funcStart[i];
        int end = funcStart[i + 1];
        assert(originCodes[start].getType() == codeSt::FunctState);
        string funcName = originCodes[start].getOperand1()->name;
        funcScope scope(funcName, vector<codeSt>(originCodes.begin() + start, originCodes.begin() + end));
        scope.work();
        func2Flows.push_back(scope);
    }
    int start = funcStart[funcStart.size() - 1];
    assert(originCodes[start].getType() == codeSt::FunctState);
    string funcName = originCodes[start].getOperand1()->name;
    funcScope scope(funcName, vector<codeSt>(originCodes.begin() + start, originCodes.end()));
    scope.work();
    func2Flows.push_back(scope);
    // SHOW_FUNCSCOPES();
}

void blockFlowGraph::SHOW_FUNCSCOPES()
{
    // cout << "into" << endl;
    for (int i = 0; i < func2Flows.size(); i++)
    {
        funcScope Flows = func2Flows[i];
        vector<block> blocks = Flows.getBlocks();
        for (int j = 0; j < blocks.size(); j++)
        {
            vector<codeSt> codes = blocks[j].getCodes();
            for (int k = 0; k < codes.size(); k++)
            {
                cout << codes[k].to_string();
            }
        }
    }
}
