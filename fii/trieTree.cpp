#include "trieTree.h"

trieTree::trieTree(int num, int not_found_flag)
{
    this->tot = 0;
    // 预设节点数目
    flags.resize(num, not_found_flag);
    ch.resize(num);
    for (int i = 0; i < ch.size(); i++)
    {
        // 字符集大小
        ch[i].resize(128);
    }
}

void trieTree::insert(string s, int flag)
{
    int len = s.size();
    int u = 1;
    for (int i = 0; i < len; i++)
    {
        char c = s[i];
        if (!ch[u][c])
        {
            ch[u][c] = ++tot;
        }
        u = ch[u][c];
    }
    this->flags[u] = flag;
    DEBUG(4, "the total node num of the trieTree:\t" + to_string(tot));
}

int trieTree::find(string s)
{
    int len = s.size();
    int u = 1;
    for (int i = 0; i < len; i++)
    {
        char c = s[i];
        if (!ch[u][c])
        {
            return false;
        }
        u = ch[u][c];
    }
    return flags[u];
}
