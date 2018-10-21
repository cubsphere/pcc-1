#include <iostream>
#include <fstream>
#include <string.h>
#include <string_view>
#include <stdio.h>
#include "sellers.hpp"
#include "ukkonen.hpp"
#include "boyer-moore.hpp"
#include "shift-or.hpp"
#include "algorithm-manager.hpp"

const char *ab = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

bool verify_algorithm(char const *algorithm_name, int edit_distance)
{
    if (strcmp(algorithm_name, "boyer-moore") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for boyer-moore\n";
            return false;
        }
        return true;
    }
    else if (strcmp(algorithm_name, "shift-or") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for shift-or\n";
            return false;
        }
        return true;
    }
    else if (strcmp(algorithm_name, "sellers") == 0 ||
             strcmp(algorithm_name, "ukkonen") == 0)
    {
        return true;
    }

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return false;
}

void print_occs(vector<int> occ, char *txt, int n)
{
    int pos = 0;
    int curr = 0;
    int next = 0;
    string v = txt;
    occ.push_back(n-5);
    while (pos < occ.size())
    {
        curr = 1 + v.rfind('\n', occ[pos]);
        next = v.find('\n', curr);
        if(next == -1)
            next = n;

        printf("%.*s\n", next - curr, txt + curr);
        while (pos < occ.size() && occ[pos] < next)
        { 
            ++pos;
        }
    }
}

void use_boyer_moore(ifstream &text_file, char *pat, int patlen, bool count_mode)
{
    auto C = bad_char(pat, patlen, ab, strlen(ab));
    int *S = good_suffix(pat, patlen);
    char txt[STRING_SIZE+1];
    vector<int> occ;
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            text_file.read(txt, STRING_SIZE);
            occ = boyer_moore(txt, text_file.gcount(), pat, patlen, C, S);
            occnum += occ.size();
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
        }
        printf("%d\n", occnum);
    }
    else
    {
        while (!text_file.eof())
        {
            text_file.read(txt, STRING_SIZE);
            occ = boyer_moore(txt, text_file.gcount(), pat, patlen, C, S);
            print_occs(occ, txt, text_file.gcount());
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
        }
    }
    delete[] S;
}

void use_shift_or_64(ifstream &text_file, char *pat, int patlen, bool count_mode)
{
    char txt[STRING_SIZE+1];
    vector<int> occ;
    auto C = char_mask(pat, patlen, ab, strlen(ab));
    auto ones = all_ones(patlen);

    if (count_mode)
    {
        int occnum = 0;
        text_file.read(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, text_file.gcount(), pat, patlen, C, ones);
            occnum += occ.size();
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
            text_file.read(txt, STRING_SIZE);
        }
        printf("%d\n", occnum);
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, text_file.gcount() - 1, pat, patlen, C, ones);
            if (!occ.empty())
                printf("%s\n", txt);
            text_file.getline(txt, STRING_SIZE);
        }
    }

    free(ones->bits);
    free(ones);
    for (int i = 0; i < strlen(ab); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
}

void use_shift_or(ifstream &text_file, char *pat, int patlen, bool count_mode)
{
    char txt[STRING_SIZE+1];
    vector<int> occ;
    auto C = char_mask(pat, patlen, ab, strlen(ab));
    auto ones = all_ones(patlen);

    if (count_mode)
    {
        int occnum = 0;
        text_file.read(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or(txt, text_file.gcount(), pat, patlen, C, ones);
            occnum += occ.size();
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
            text_file.read(txt, STRING_SIZE);
        }
        printf("%d\n", occnum);
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or(txt, text_file.gcount() - 1, pat, patlen, C, ones);
            if (!occ.empty())
                printf("%s\n", txt);
            text_file.getline(txt, STRING_SIZE);
        }
    }

    free(ones->bits);
    free(ones);
    for (int i = 0; i < strlen(ab); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
}

void use_sellers(ifstream &text_file, char *pat, int patlen, int edit_distance, bool count_mode)
{
    char txt[STRING_SIZE+1];
    vector<int> occ;
    if (count_mode)
    {
        int occnum = 0;
        text_file.read(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = sellers(txt, text_file.gcount(), pat, patlen, edit_distance);
            occnum += occ.size();
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
            text_file.read(txt, STRING_SIZE);
        }
        printf("%d\n", occnum);
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = sellers(txt, text_file.gcount() - 1, pat, patlen, edit_distance);
            if (!occ.empty())
                printf("%s\n", txt);
            text_file.getline(txt, STRING_SIZE);
        }
    }
}

void use_ukkonen(ifstream &text_file, char *pat, int patlen, int edit_distance, bool count_mode)
{
    char txt[STRING_SIZE+1];
    vector<int> occ;
    Ukk_fsm *fsm = build_ukk_fsm(pat, patlen, ab, strlen(ab), edit_distance);
    if (count_mode)
    {
        int occnum = 0;
        text_file.read(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = ukk(txt, text_file.gcount(), pat, patlen, ab, strlen(ab), edit_distance, fsm);
            occnum += occ.size();
            int pos = text_file.tellg();
            text_file.seekg(pos - patlen + 1);
            text_file.read(txt, STRING_SIZE);
        }
        printf("%d\n", occnum);
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = ukk(txt, text_file.gcount() - 1, pat, patlen, ab, strlen(ab), edit_distance, fsm);
            if (!occ.empty())
                printf("%s\n", txt);
            text_file.getline(txt, STRING_SIZE);
        }
    }
}

void process_text(ifstream &text_file, char *pat, int patlen, char const *algorithm_name, bool count_mode, int edit_distance)
{
    if (strcmp(algorithm_name, "boyer-moore") == 0)
    {
        use_boyer_moore(text_file, pat, patlen, count_mode);
    }
    else if (strcmp(algorithm_name, "shift-or") == 0)
    {
        if (patlen <= 64)
        {
            use_shift_or_64(text_file, pat, patlen, count_mode);
        }
        else
        {
            use_shift_or(text_file, pat, patlen, count_mode);
        }
    }
    else if (strcmp(algorithm_name, "sellers") == 0)
    {
        use_sellers(text_file, pat, patlen, edit_distance, count_mode);
    }
    else if (strcmp(algorithm_name, "ukkonen") == 0)
    {
        use_ukkonen(text_file, pat, patlen, edit_distance, count_mode);
    }
}