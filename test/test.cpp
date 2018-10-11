#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

struct bitmap
{
    uint64_t *bits;
    uint32_t len;
};

bitmap *all_ones(int len)
{
    len = len / 64 + (len % 64 == 0 ? 0 : 1);
    uint64_t *b = (uint64_t*)malloc(len*sizeof(uint64_t));
    for (int i = 0; i < len; ++i)
    {
        b[i] = 0xFFFFFFFFFFFFFFFF;
    }
    bitmap *map = (bitmap*)malloc(sizeof(bitmap));
    map->bits = b;
    map->len = len;
    return map;
}

bitmap *all_zeroes(int len)
{
    len = len / 64 + (len % 64 == 0 ? 0 : 1);
    uint64_t *b = (uint64_t*)malloc(len*sizeof(uint64_t));
    for (int i = 0; i < len; ++i)
    {
        b[i] = 0ull;
    }
    bitmap *map = (bitmap*)malloc(sizeof(bitmap));
    map->bits = b;
    map->len = len;
    return map;
}

void shift_left_1(bitmap *map)
{
    int i;
    uint64_t msb_prev = 0;
    uint64_t msb_cur;
    for (i = 0; i+3 < map->len; i += 4)
    {
        msb_cur = ((map->bits[i] & 0x8000000000000000) >> 63) & 1ull;
        map->bits[i] = (map->bits[i] << 1) | msb_prev;
        msb_prev = msb_cur;
        msb_cur = ((map->bits[i + 1] & 0x8000000000000000) >> 63) & 1ull;
        map->bits[i + 1] = (map->bits[i + 1] << 1) | msb_prev;
        msb_prev = msb_cur;
        msb_cur = ((map->bits[i + 2] & 0x8000000000000000) >> 63) & 1ull;
        map->bits[i + 2] = (map->bits[i + 2] << 1) | msb_prev;
        msb_prev = msb_cur;
        msb_cur = ((map->bits[i + 3] & 0x8000000000000000) >> 63) & 1ull;
        map->bits[i + 3] = (map->bits[i + 3] << 1) | msb_prev;
        msb_prev = msb_cur;
    }
    for (int j = i; j < map->len; ++j)
    {
        msb_cur = ((map->bits[i] & 0x8000000000000000) >> 63) & 1;
        map->bits[i] = (map->bits[i] << 1) | msb_prev;
        msb_prev = msb_cur;
    }
}

void bitOr(bitmap *map1, bitmap *map2)
{
    int i;
    for (i = 0; i < map1->len; i += 4)
    {
        map1->bits[i] |= map2->bits[i];
        map1->bits[i + 1] |= map2->bits[i + 1];
        map1->bits[i + 2] |= map2->bits[i + 2];
        map1->bits[i + 3] |= map2->bits[i + 3];
    }
    for (int j = i; j < map1->len; ++j)
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

vector<int> shift_or(string txt, string pat, string ab)
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

    for(int i = 0; i<ab.length(); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
    free(S->bits);
    free(S);
    return occ;
}

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
