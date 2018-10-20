#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <vector>
#include "boyer-moore.h"
#include "shift-or.h"
#include "helpful-string.h"

using namespace std;

const string ab = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


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

void process_text(ifstream &text_file, string pat, string algorithm_name, bool count_mode, int edit_distance)
{
    vector<int> occ;
    int occnum = 0;
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        use_boyer_moore(text_file, pat, count_mode);
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        use_shift_or(text_file, pat, count_mode);
    }
}

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

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return false;
}

int main(int argc, char **argv)
{
    string algorithm_name;
    int edit_distance = 0;
    bool count_mode = false;

    vector<string> text_paths;
    bool text_defined = false;

    string pattern_path;
    string pattern;
    bool use_pattern_path = false;
    bool pattern_defined = false;

    int c;
    int option_index = 0;
    static struct option long_options[] = {
        {"edit", required_argument, 0, 'e'},
        {"pattern", required_argument, 0, 'p'},
        {"algorithm_name", required_argument, 0, 'a'},
        {"count", no_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'}};

    while (1)
    {
        c = getopt_long(argc, argv, "e:p:a:ch", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 'e':
            edit_distance = stoi(optarg);
            if (edit_distance < 0)
                edit_distance *= -1;
            break;

        case 'p':
            pattern_defined = true;
            use_pattern_path = true;
            pattern_path = optarg;
            break;

        case 'a':
            algorithm_name = optarg;
            break;

        case 'c':
            count_mode = true;
            break;

        case 'h':
            cout << helpful_string;
            break;

        case '?':
        default:
            return 1;
        }
    }

    while (optind < argc)
    {
        if (!pattern_defined)
        {
            pattern_defined = true;
            pattern = argv[optind];
        }
        else
        {
            text_defined = true;
            text_paths.push_back(argv[optind]);
        }
        ++optind;
    }

    if (!pattern_defined || !text_defined)
    {
        cout << helpful_string;
        return 1;
    }

    if (algorithm_name.length() == 0)
    {
        if (edit_distance == 0)
            algorithm_name = "boyer-moore";
        else
            algorithm_name = "some-algorithm";
    }
    bool alg_ok = verify_algorithm(algorithm_name, edit_distance);
    if (!alg_ok)
        return 1;

    ifstream text_file;
    string pat;
    for (auto const &text_path : text_paths)
    {
        text_file.open(text_path);
        if (!text_file.is_open())
        {
            cout << "____ could not open text file " + text_path + " ____\n";
            continue;
        }

        if (!use_pattern_path)
        {
            process_text(text_file, pattern, algorithm_name, count_mode, edit_distance);
        }
        else
        {
            ifstream pattern_file;
            pattern_file.open(pattern_path);
            if (!pattern_file.is_open())
            {
                cout << "____ could not open pattern file " + pattern_path + " ____\n";
                return 1;
            }
            getline(pattern_file, pat);
            while (!pattern_file.eof())
            {
                process_text(text_file, pat, algorithm_name, count_mode, edit_distance);
                getline(pattern_file, pat);
            }
            pattern_file.close();
        }
        text_file.close();
    }
    return 0;
}