#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <vector>
#include "boyer-moore.h"
#include "shift-or.h"

using namespace std;
using Algorithm = function<vector<int>(string, string, string, int)>;

Algorithm findAlgorithm(string algorithm_name, int edit_distance);
void process_text(ifstream &text_file, string pat, Algorithm algorithm, bool count_mode, int edit_distance);
bool checkFile(ifstream file);

int main(int argc, char **argv)
{
    string algorithm_name;
    int edit_distance = 0;
    bool count_mode = false;

    vector<string> text_paths;
    bool text_defined = false;

    vector<string> pattern_paths;
    string pattern;
    bool use_pattern_paths = false;
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
            if(edit_distance < 0)
                edit_distance *= -1;
            break;

        case 'p':
            --optind;
            for (; optind < argc && *(argv[optind]) != '-'; ++optind)
            {
                pattern_defined = true;
                use_pattern_paths = true;
                pattern_paths.push_back(argv[optind]);
            }
            break;

        case 'a':
            algorithm_name = optarg;
            break;

        case 'c':
            count_mode = true;
            break;

        case 'h':
            cout << "this very informative message helps you run this program!";
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

    if (algorithm_name.length() == 0)
    {
        if (edit_distance == 0)
            algorithm_name = "boyer-moore";
        else
            algorithm_name = "some-algorithm";
    }
    Algorithm algorithm = findAlgorithm(algorithm_name, edit_distance);
    if (algorithm == nullptr)
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

        if (!use_pattern_paths)
        {
            process_text(text_file, pattern, algorithm, count_mode, edit_distance);
        }
        else
        {
            ifstream pattern_file;
            for (auto const &pattern_path : pattern_paths)
            {
                pattern_file.open(pattern_path);
                if (!pattern_file.is_open())
                {
                    cout << "____ could not open pattern file " + pattern_path + " ____\n";
                    continue;
                }
                getline(pattern_file, pat);
                while(!text_file.eof())
                {
                    process_text(text_file, pat, algorithm, count_mode, edit_distance);
                    getline(pattern_file, pat);
                }
                pattern_file.close();
            }
        }
        text_file.close();
    }
    return 0;
}

void process_text(ifstream &text_file, string pat, Algorithm algorithm, bool count_mode, int edit_distance)
{
    const string ab = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    vector<int> occ;
    int occnum = 0;
    string txt;
    if (!count_mode)
    {
        getline(text_file, txt);
        while(!text_file.eof())
        {
            occ = algorithm(txt, pat, ab, edit_distance);
            if(!occ.empty())
            {
                cout << txt << '\n';
            }
            getline(text_file, txt);
        }
    }
    else
    {
        getline(text_file, txt);
        while(!text_file.eof())
        {
            occ = algorithm(txt, pat, ab, edit_distance);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << " occurences\n";
    }
}

Algorithm findAlgorithm(string algorithm_name, int edit_distance)
{
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for boyer-moore\n";
            return nullptr;
        }
        return boyer_moore;
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for shift-or\n";
            return nullptr;
        }
        return shift_or;
    }

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return nullptr;
}