#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>

using namespace std;

struct Node
{
    int idx;
    Node **chd;

    Node()
    {
        idx = -1;
        chd = new Node *[3];
        chd[0] = NULL;
        chd[1] = NULL;
        chd[2] = NULL;
    }
};

struct Ukk_fsm
{
    map<pair<int, char>, int> delta;
    set<int> F;
    int idx;

    Ukk_fsm(map<pair<int, char>, int> d, set<int> f, int id)
    {
        idx = id;
        F = f;
        delta = d;
    }
};

int tree_find(Node *root, vector<int> s)
{
    Node *cur = root;
    int i = 1;
    while (i < s.size())
    {
        int d = s[i] - s[i - 1];
        if (cur->chd[d + 1])
        {
            cur = cur->chd[d + 1];
            i++;
        }
        else
            break;
    }
    if (i == s.size())
        return cur->idx;
    else
        return -2;
}

void tree_add(Node *root, vector<int> s, int idx)
{
    Node *cur = root;
    int i = 1;
    int d;
    while (i < s.size())
    {
        d = s[i] - s[i - 1];
        if (cur->chd[d + 1])
        {
            cur = cur->chd[d + 1];
            i++;
        }
        else
            break;
    }
    while (i < s.size())
    {
        Node *nn = new Node();
        nn->idx = idx;
        d = s[i] - s[i - 1];
        cur->chd[d + 1] = nn;
        cur = nn;
        i++;
    }
}

vector<int> next_column(vector<int> s, char* pat, int m, char a, int r)
{
    vector<int> t;
    t.push_back(0);
    for (int i = 1; i <= m; i++)
    {
        t.push_back(min(min(min(s[i] + 1, t[i - 1] + 1), s[i - 1] + (int)((pat[i - 1] != a))), r + 1));
    }
    return t;
}

Ukk_fsm *build_ukk_fsm(char* pat, int m, char const* ab, int ablen, int r)
{
    vector<int> s;
    for (int i = 0; i <= m; i++)
    {
        s.push_back(i);
    }
    queue<pair<vector<int>, int>> que;
    que.push(pair<vector<int>, int>(s, 0));
    Node *Q = new Node();
    tree_add(Q, s, 0);
    set<int> F;
    int idx = 0;
    map<pair<int, char>, int> delta;
    if (s[s.size() - 1] <= r)
    {
        F.insert(idx);
    }
    while (!que.empty())
    {
        pair<vector<int>, int> par = que.front();
        que.pop();
        for (int i = 0; i < ablen; i++)
        {
            vector<int> t = next_column(get<0>(par), pat, m, ab[i], r);
            int idx_t = tree_find(Q, t);
            //	    cout << "idx_t: " << idx_t << "\n";
            if (idx_t == -2)
            {
                idx++;
                idx_t = idx;
                tree_add(Q, t, idx);
                que.push(pair<vector<int>, int>(t, idx_t));
                if (t[t.size() - 1] <= r)
                {
                    F.insert(idx);
                }
            }
            delta.insert(pair<pair<int, char>, int>(pair<int, char>(get<1>(par), ab[i]), idx_t));
        }
    }
    return new Ukk_fsm(delta, F, idx);
}

vector<int> ukk(char* txt, int n, char* pat, int m, char const* ab, int ablen, int r, Ukk_fsm *fsm)
{
    map<pair<int, char>, int> delta;
    set<int> F;
    int l;
    if (!fsm)
    {
        fsm = build_ukk_fsm(pat, m, ab, ablen, r);
    }
    delta = fsm->delta;
    F = fsm->F;
    l = fsm->idx;
    int s = 0;
    vector<int> occ;
    /*    for (std::map<pair<int,char>,int>::iterator it=delta.begin(); it!=delta.end(); ++it)
	std::cout << "(" << get<0>(it->first) << ", " << get<1>(it->first) << ")" << " => " << it->second << '\n';
    cout << "F: set([";
    for (std::set<int>::iterator it=F.begin(); it!=F.end(); ++it)
	std::cout << *it << ", ";
	cout << "])\nl: " << l << endl;*/
    auto search = F.find(s);
    if (search != F.end())
    {
        occ.push_back(l);
    }
    for (int i = 0; i < n; i++)
    {
        //	cout << delta[pair<int,char>(s, txt[i])] << endl;
        s = delta.at(pair<int, char>(s, txt[i]));
        auto search = F.find(s);
        if (search != F.end())
        {
            occ.push_back(i);
        }
    }
    return occ;
}
/*
int main() {
    char* txt = "abadac";
    char* pat = "cada";
    char const* ab = "abcd";
    int r = 2;
    Ukk_fsm* fsm = build_ukk_fsm(pat, ab, r);
    cout << "# states = " << fsm->idx << endl;
    vector<int> occ = ukk(txt, pat, ab, r, NULL);
    for (int i = 0; i < occ.size(); i++) {
	cout << occ[i] << " ";
    }
}
*/
