#include <iostream>
#include <fstream>
#include <string.h>
#include "boyer-moore.h"
#include "shift-or.h"
#include "algorithm-manager.h"

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

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return false;
}

void use_boyer_moore(ifstream &text_file, char *pat, int patlen, bool count_mode)
{
    auto C = bad_char(pat, patlen, ab, strlen(ab));
    int *S = good_suffix(pat, patlen);
    char txt[STRING_SIZE];
    vector<int> occ;
    if (count_mode)
    {
        int occnum = 0;
        text_file.readsome(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, text_file.gcount(), pat, patlen, C, S);
            occnum += occ.size();
            text_file.readsome(txt, STRING_SIZE);
        }
        cout << occnum << '\n';
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, text_file.gcount(), pat, patlen, C, S);
            if (!occ.empty())
                cout << txt << '\n';
            text_file.getline(txt, STRING_SIZE);
        }
    }
    delete[] S;
}

void use_shift_or_64(ifstream &text_file, char *pat, int patlen, bool count_mode)
{
    char txt[STRING_SIZE];
    vector<int> occ;
    auto C = char_mask(pat, patlen, ab, strlen(ab));
    auto ones = all_ones(patlen);

    if (count_mode)
    {
        int occnum = 0;
        text_file.readsome(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, text_file.gcount(), pat, patlen, C, ones);
            occnum += occ.size();
            text_file.readsome(txt, STRING_SIZE);
        }
        cout << occnum << '\n';
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, text_file.gcount(), pat, patlen, C, ones);
            if (!occ.empty())
                cout << txt << '\n';
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
    char txt[STRING_SIZE];
    vector<int> occ;
    auto C = char_mask(pat, patlen, ab, strlen(ab));
    auto ones = all_ones(patlen);

    if (count_mode)
    {
        int occnum = 0;
        text_file.readsome(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or(txt, text_file.gcount(), pat, patlen, C, ones);
            occnum += occ.size();
            text_file.readsome(txt, STRING_SIZE);
        }
        cout << occnum << '\n';
    }
    else
    {
        text_file.getline(txt, STRING_SIZE);
        while (!text_file.eof())
        {
            occ = shift_or(txt, text_file.gcount(), pat, patlen, C, ones);
            if (!occ.empty())
                cout << txt << '\n';
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
}
