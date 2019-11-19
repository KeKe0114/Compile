#ifndef TRIE_TREE_H
#define TRIE_TREE_H
#include <string>
#include <vector>
#include "debug.h"
using namespace std;
class trieTree
{
private:
    vector<vector<int>> ch;
    vector<int> flags;
    int tot;

public:
    trieTree(int num = 100, int not_found_flag = 0);
    void insert(string s, int flag);
    int find(string s);
};
#endif