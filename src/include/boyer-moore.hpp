#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H

#include <vector>
#include <unordered_map>
using namespace std;

vector<int> boyer_moore(char *txt, int n, char *pat, int m, unordered_map<char, int> C, int *S);
int *good_suffix(char *pat, int m);
unordered_map<char, int> bad_char(char *pat, int m, char const *ab, int ablen);

#endif
