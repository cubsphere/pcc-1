#include <vector>

using namespace std;

struct Node;
struct Ukk_fsm;

int tree_find(Node* root, vector<int> s);
void tree_add(Node* root, vector<int> s, int idx);
vector<int> next_column(vector<int> s, string pat, char a, int r);
Ukk_fsm* build_ukk_fsm(string pat, string ab, int r);
vector<int> ukk(string txt, string pat, string ab, int r, Ukk_fsm* fsm);
