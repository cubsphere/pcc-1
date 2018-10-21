#include <iostream>
#include <unordered_map>
#include <vector>
#include <string.h>
#include "boyer-moore.h"

using namespace std;

unordered_map<char, int> bad_char(char* pat, int m, char const* ab, int l)
{
    unordered_map<char, int> C = unordered_map<char, int>();

    for (int i = 0; i < l; ++i)
    {
        C[ab[i]] = -1;
    }

    for (int i = 0; i < m; ++i)
    {
        C[pat[i]] = i;
    }
    return C;
}

int *reverse_border(char* str, int m)
{
    int *nxt = new int[m + 1];
    for (int i = 0; i < m + 1; ++i)
    {
        nxt[i] = 0;
    }

    int i = 1, j = 0;
    while (i + j < m)
    {
        while (i + j < m && str[m - 1 - i + j] == str[m - 1 - j])
        {
            ++j;
            nxt[i + j] = j;
        }
        i += max(1, (j - nxt[j]));
        j = nxt[j];
    }
    return nxt;
}

int *good_suffix(char* pat, int m)
{
    int *R = reverse_border(pat, m);
    int *S = new int[m + 1];
    for (int i = 0; i < m + 1; ++i)
    {
        S[i] = m - R[m];
    }
    for (int l = 1, j; l < m + 1; ++l)
    {
        j = m - 1 - R[l];
        S[j] = min(S[j], l - R[l]);
    }
    delete[] R;
    return S;
}

vector<int> boyer_moore(char* txt, int n, char* pat, int m, unordered_map<char, int> C, int *S)
{
    vector<int> occ;

    for (int i = 0; i <= n - m;)
    {
        int j = 0;
        while (j >= 0 && txt[i + j] == pat[j])
        {
            --j;
        }
        if (j == -1)
        {
            occ.push_back(i);
            i += S[m];
        }
        else
        {
            i += max(S[j], j - C[txt[i + j]]);
        }
    }
    return occ;
}

vector<int> boyer_moore_standalone(char* txt, int n, char* pat, int m, char* ab, int ed)
{
    unordered_map<char, int> C = bad_char(pat, m, ab, strlen(ab));
    int *S = good_suffix(pat, m);
    vector<int> occ;

    for (int i = 0; i <= n - m;)
    {
        int j = m-1;
        while (j >= 0 && txt[i + j] == pat[j])
        {
            --j;
        }
        if (j == -1)
        {
            occ.push_back(i);
            i += S[m];
        }
        else
        {
            i += max(S[j], j - C[txt[i + j]]);
        }
    }
    delete[] S;
    return occ;
}

/*
int main()
{
    char* txt = "ababcababcababc";
    char* pat = "ababca";
    char* ab = "abc";
    vector<int> vec = boyer_moore(txt, pat, ab);
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        cout << *it << ", ";
    }
    return 0;
}
*/
