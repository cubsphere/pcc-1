#ifndef SHIFT_OR_H
#define SHIFT_OR_H

#include <vector>
#include <unordered_map>

using namespace std;

struct bitmap
{
    uint64_t *bits;
    uint32_t len;
};

bitmap *all_ones(int len);
unordered_map<char, bitmap *> char_mask(char *pat, int patlen, char const *ab, int ablen);
vector<int> shift_or(char *txt, int n, char *pat, int m, unordered_map<char, bitmap *> C, bitmap *ones);
vector<int> shift_or_64(char *txt, int n, char *pat, int m, unordered_map<char, bitmap *> C, bitmap *ones);

#endif
