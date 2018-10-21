#include <iostream>
#include <fstream>
#include "boyer-moore.h"
#include "shift-or.h"
#include "sellers.h"
#include "ukkonen.h"
#include "algorithm-manager.h"

const string ab = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

bool verify_algorithm(string algorithm_name, int edit_distance)
{
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for boyer-moore\n";
            return false;
        }
        return true;
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for shift-or\n";
            return false;
        }
        return true;
    }
    else if (algorithm_name.compare("sellers") == 0 ||
	     algorithm_name.compare("ukkonen") == 0)
    {
	return true;
    }

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return false;
}

void process_text(ifstream &text_file, string pat, string algorithm_name, bool count_mode, int edit_distance)
{
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        use_boyer_moore(text_file, pat, count_mode);
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        use_shift_or(text_file, pat, count_mode);
    }
    else if (algorithm_name.compare("sellers") == 0)
    {
	use_sellers(text_file, pat, edit_distance, count_mode);
    }
    else if (algorithm_name.compare("ukkonen") == 0)
    {
	use_ukkonen(text_file, pat, edit_distance, count_mode);
    }
}

void use_boyer_moore(ifstream &text_file, string pat, bool count_mode)
{
    auto C = bad_char(pat, ab);
    int *S = good_suffix(pat);
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, pat, C, S);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, pat, C, S);
            if(!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }
    delete[] S;
}

void use_shift_or(ifstream &text_file, string pat, bool count_mode)
{
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    auto C = char_mask(pat, ab);
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = shift_or(txt, pat, C);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = shift_or(txt, pat, C);
            if(!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }
}

void use_sellers(ifstream &text_file, string pat, int edit_distance)
{
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = sellers(txt, pat, edit_distance);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = sellers(txt, pat, edit_distance);
            if(!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }
}
