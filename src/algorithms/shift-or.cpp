#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "shift-or.h"

using namespace std;

struct bitmap
{
    uint64_t *bits;
    uint32_t len;
};

bitmap *all_ones(int len)
{
    len = len / 64 + (len % 64 == 0 ? 0 : 1);
    uint64_t *b = (uint64_t *)malloc(len * sizeof(uint64_t));
    for (int i = 0; i < len; ++i)
    {
        b[i] = 0xFFFFFFFFFFFFFFFF;
    }
    bitmap *map = (bitmap *)malloc(sizeof(bitmap));
    map->bits = b;
    map->len = len;
    return map;
}

bitmap *all_zeroes(int len)
{
    len = len / 64 + (len % 64 == 0 ? 0 : 1);
    uint64_t *b = (uint64_t *)malloc(len * sizeof(uint64_t));
    for (int i = 0; i < len; ++i)
    {
        b[i] = 0ull;
    }
    bitmap *map = (bitmap *)malloc(sizeof(bitmap));
    map->bits = b;
    map->len = len;
    return map;
}

void shift_left_1(bitmap *map)
{
    uint64_t msb_prev = 0;
    uint64_t msb_cur;
    for (int j = 0; j < map->len; ++j)
    {
        msb_cur = ((map->bits[j] & 0x8000000000000000) >> 63) & 1;
        map->bits[j] = (map->bits[j] << 1) | msb_prev;
        msb_prev = msb_cur;
    }
}

void bitOr(bitmap *map1, bitmap *map2)
{
    int i;
    for (int j = 0; j < map1->len; ++j)
    {
        map1->bits[j] |= map2->bits[j];
    }
}

void set(bitmap *map, int pos)
{
    map->bits[pos / 64] |= 1ull << (pos % 64);
}

void reset(bitmap *map, int pos)
{
    map->bits[pos / 64] &= (1ull << (pos % 64)) ^ 0xFFFFFFFFFFFFFFFF;
}

unordered_map<char, bitmap *> char_mask(string pat, string ab)
{
    int m = pat.length();
    unordered_map<char, bitmap *> C;
    for (int i = 0; i < ab.length(); ++i)
    {
        C.emplace(ab[i], all_ones(m));
    }
    for (int i = 0; i < m; ++i)
    {
        reset(C[pat[i]], i);
    }
    return C;
}

vector<int> shift_or(string txt, string pat, unordered_map<char, bitmap *> C)
{
    int n = txt.length();
    int m = pat.length();
    bitmap *S = all_ones(m);
    vector<int> occ;
    for (int i = 0; i < n; ++i)
    {
        shift_left_1(S);
        bitOr(S, C[txt[i]]);
        if (!(S->bits[S->len - 1] & (1ull << (m - 1 % 64))))
        {
            occ.push_back(i - m + 1);
        }
    }
    free(S->bits);
    free(S);
    return occ;
}

vector<int> shift_or_standalone(string txt, string pat, string ab)
{
    int n = txt.length();
    int m = pat.length();
    unordered_map<char, bitmap *> C = char_mask(pat, ab);
    bitmap *S = all_ones(m);
    vector<int> occ;
    for (int i = 0; i < n; ++i)
    {
        shift_left_1(S);
        bitOr(S, C[txt[i]]);
        if (!(S->bits[S->len - 1] & (1ull << (m - 1 % 64))))
        {
            occ.push_back(i - m + 1);
        }
    }

    for (int i = 0; i < ab.length(); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
    free(S->bits);
    free(S);
    return occ;
}

/*
int main()
{
    string txt = "ababcababcababc";
    string pat = "ababca";
    string ab = "abc";
    vector<int> occ = shift_or(txt, pat, ab);
    for (vector<int>::iterator it = occ.begin(); it != occ.end(); ++it)
    {
        cout << *it << ", ";
    }
    return 0;
}
*/