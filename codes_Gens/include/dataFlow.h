#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include "midcode.h"

using namespace std;

class block
{

private:
    int id;
    vector<codeSt> codeInBlock;

    set<int> use;
    set<int> def;
    set<int> alivein;
    set<int> aliveout;
    map<int, set<int>> idx2alivein;
    map<int, set<int>> idx2aliveout;

public:
    block(int idArg, vector<codeSt> codes) : id(idArg), codeInBlock(codes) {}
    //for work
    set<int> getBlockUse() { return use; }
    set<int> getBlockDef() { return def; }
    void genBlockUseAndDef();
    void setBlockUseDefIn(set<int> alivein);
    void setBlockUseDefOut(set<int> aliveout);
    void genUseDefInOutForSingleLine();

    //for get
    vector<codeSt> getCodes() { return codeInBlock; }
    set<int> getCodeIn(int idx) { return idx2alivein[idx]; }
    set<int> getCodeOut(int idx) { return idx2aliveout[idx]; }
    set<int> getBlockAliveIn() { return alivein; }
    set<int> getBlockAliveOut() { return aliveout; }

public:
    void SHOW_CODE_AND_USE_DEF()
    {
        cout << "code:" << endl;
        for (int i = 0; i < codeInBlock.size(); i++)
        {
            cout << codeInBlock[i].to_string();
        }
        cout << "use:" << endl;
        for (auto i : use)
        {
            cout << i << " ";
        }
        cout << endl;
        cout << "def:" << endl;
        for (auto i : def)
        {
            cout << i << " ";
        }
        cout << endl;
    }
};

class funcScope
{
private:
    /* 
     * 原中间代码中的块的顺序,按照id的顺序保持.
     * 语句j在语句i的后面意味着语句j所在的块id大于等于语句i所在的块id.
     */
    string funcName;
    vector<codeSt> originCodes;
    vector<block> id2block;
    map<int, set<int>> id2child;
    map<int, set<int>> id2father;
    map<int, set<int>> id2alivein;
    map<int, set<int>> id2aliveout;

public:
    void SHOW_SET_INNER()
    {
        cout << "***********id2child:" << endl;
        for (auto i : id2child)
        {
            cout << "block " << i.first << " :\t";
            for (auto j : i.second)
            {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "***********id2father:" << endl;
        for (auto i : id2father)
        {
            cout << "block " << i.first << " :\t";
            for (auto j : i.second)
            {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "***********id2alivein:" << endl;
        for (auto i : id2alivein)
        {
            cout << "block " << i.first << " :\t";
            for (auto j : i.second)
            {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "***********id2aliveout:" << endl;
        for (auto i : id2aliveout)
        {
            cout << "block " << i.first << " :\t";
            for (auto j : i.second)
            {
                cout << j << " ";
            }
            cout << endl;
        }
    }

public:
    funcScope(string Name, vector<codeSt> funcCode)
    {
        this->funcName = Name;
        originCodes = funcCode;
    }

private:
    void addFather(int id, int father)
    {
        if (id2father.find(id) == id2father.end())
        {
            id2father.insert(pair<int, set<int>>(id, set<int>()));
        }
        id2father[id].insert(father);
    }
    void addChild(int id, int child)
    {
        if (id2child.find(id) == id2child.end())
        {
            id2child.insert(pair<int, set<int>>(id, set<int>()));
        }
        id2child[id].insert(child);
    }
    void addRelationFatherChild(int father, int child)
    {
        addFather(child, father);
        addChild(father, child);
    }

private:
    void genBlocksFromOrigin();
    void genUseDefInOut();
    void setBlockInAndOut();

public:
    void work()
    {
        genBlocksFromOrigin();
        genUseDefInOut();
        // SHOW_SET_INNER();
        setBlockInAndOut();
    }
    vector<block> getBlocks() { return id2block; }
    string getName() { return funcName; }
};

class blockFlowGraph
{

private:
    blockFlowGraph() {}
    blockFlowGraph(const blockFlowGraph &) = delete;
    blockFlowGraph &operator&(blockFlowGraph &) = delete;

public:
    static blockFlowGraph &get_instance()
    {
        static blockFlowGraph instance;
        return instance;
    }

private:
    vector<codeSt> globalVarStates;
    vector<funcScope> func2Flows;

public:
    void genfuncDivide();
    vector<codeSt> getGlobalState() { return globalVarStates; }
    vector<funcScope> getFuncScopes() { return func2Flows; }
    void SHOW_FUNCSCOPES();
};