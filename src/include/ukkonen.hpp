#include <vector>

using namespace std;

struct Node;
struct Ukk_fsm;

int tree_find(Node *root, vector<int> s);
void tree_add(Node *root, vector<int> s, int idx);
vector<int> next_column(vector<int> s, char *pat, int m, char a, int r);
Ukk_fsm *build_ukk_fsm(char *pat, int m, char const *ab, int ablen, int r);
vector<int> ukk(char* txt, int n, char *pat, int m, char const *ab, int ablen, int r, Ukk_fsm *fsm);
