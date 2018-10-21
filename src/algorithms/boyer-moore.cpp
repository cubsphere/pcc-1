#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "boyer-moore.h"

using namespace std;

unordered_map<char, int> bad_char(string pat, string ab)
{
    unordered_map<char, int> C = unordered_map<char, int>();

    int l = ab.length();
    for (int i = 0; i < l; ++i)
    {
        C[ab[i]] = -1;
    }

    int m = pat.length();
    for (int i = 0; i < m; ++i)
    {
        C[pat[i]] = i;
    }
    return C;
}

int *reverse_border(string str)
{
    int m = str.length();
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

int *good_suffix(string pat)
{
    int m = pat.length();
    int *R = reverse_border(pat);
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

vector<int> boyer_moore(string txt, string pat, unordered_map<char, int> C, int *S)
{
    int n = txt.length();
    int m = pat.length();
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

vector<int> boyer_moore_standalone(string txt, string pat, string ab, int ed)
{
    int n = txt.length();
    int m = pat.length();
    unordered_map<char, int> C = bad_char(pat, ab);
    int *S = good_suffix(pat);
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
    string txt = "ababcababcababc";
    string pat = "ababca";
    string ab = "abc";
    vector<int> vec = boyer_moore(txt, pat, ab);
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        cout << *it << ", ";
    }
    return 0;
}
*/
