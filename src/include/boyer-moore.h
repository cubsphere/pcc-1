#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H

#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<int> boyer_moore(string txt, string pat, unordered_map<char, int> C, int *S);
int *good_suffix(string pat);
unordered_map<char, int> bad_char(string pat, string ab);

#endif
