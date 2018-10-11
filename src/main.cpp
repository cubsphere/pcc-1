#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <vector>
#include "boyer-moore.h"

/* Flag set by ‘--verbose’. */
static int verbose_flag;

int main(int argc, char **argv)
{
    int edit_distance = 0;
    vector<string> pattern_files;
    string algorithm;
    bool count_mode = false;

    int c;
    int option_index = 0;
    static struct option long_options[] = {
        {"edit", required_argument, 0, 'e'},
        {"pattern", required_argument, 0, 'p'},
        {"algorithm", required_argument, 0, 'a'},
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
            break;

        case 'p':
            --optind;
            for (; optind < argc && *argv[optind] != '-'; ++optind)
            {
                pattern_files.push_back(argv[optind]);
            }
            break;

        case 'a':
            algorithm = optarg;
            break;

        case 'c':
            count_mode = true;
            break;

        case 'h':
            break;

        default:
            abort();
        }
    }

    if (algorithm.length() == 0)
    {
        if (edit_distance == 0)
            algorithm = "boyer-moore";
        else
            algorithm = "some-algorithm";
    }

    exit(0);
}