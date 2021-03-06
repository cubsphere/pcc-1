#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include <string.h>
#include "helpful-string.hpp"
#include "algorithm-manager.hpp"

using namespace std;

int main(int argc, char **argv)
{
    char const *algorithm_name = nullptr;
    int edit_distance = 0;
    bool count_mode = false;

    vector<char *> text_paths;
    bool text_defined = false;

    char *pattern_path;
    char *pattern;
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
            try
            {
                edit_distance = stoi(optarg);
                if (edit_distance < 0)
                    throw invalid_argument("...");
            }
            catch (...)
            {
                cout << "edit distance must be a decimal positive integer smaller than 2147483648\n";
                return 1;
            }
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
        if (!pattern_defined && !text_defined)
        {
            cout << helpful_string;
            return 1;
        }
        if (!pattern_defined)
            cout << "specify a pattern\n";
        if (!text_defined)
            cout << "specify a text file\n";
        return 1;
    }

    if (algorithm_name == nullptr)
    {
        if (edit_distance == 0)
            algorithm_name = "boyer-moore";
        else
            algorithm_name = "sellers"; // trocar pra esse ou sellers
    }
    bool alg_ok = verify_algorithm(algorithm_name, edit_distance);
    if (!alg_ok)
        return 1;

    ifstream text_file;
    for (auto const &text_path : text_paths)
    {
        text_file.open(text_path);
        if (!text_file.is_open())
        {
            cout << "____ could not open text file " << text_path << " ____\n";
            continue;
        }

        if (!use_pattern_path)
        {
            process_text(text_file, pattern, strlen(pattern), algorithm_name, count_mode, edit_distance);
        }
        else
        {
            char pat[STRING_SIZE_LESS];
            ifstream pattern_file;
            pattern_file.open(pattern_path);
            if (!pattern_file.is_open())
            {
                cout << "____ could not open pattern file " << pattern_path << " ____\n";
                return 1;
            }
            while (!pattern_file.eof())
            {
                pattern_file.getline(pat, STRING_SIZE_LESS);
                process_text(text_file, pat, pattern_file.gcount() - 1, algorithm_name, count_mode, edit_distance);
            }
            pattern_file.close();
        }
        text_file.close();
    }
    return 0;
}
