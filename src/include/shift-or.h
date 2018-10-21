#ifndef SHIFT_OR_H
#define SHIFT_OR_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct bitmap
{
    uint64_t *bits;
    uint32_t len;
};

bitmap* all_ones(int len);
unordered_map<char, bitmap *> char_mask(string pat, string ab);
vector<int> shift_or(string txt, string pat, unordered_map<char, bitmap *> C, bitmap* ones);
vector<int> shift_or_64(string txt, string pat, unordered_map<char, bitmap *> C, bitmap* ones);

#endif