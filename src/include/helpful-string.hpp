#ifndef HELPFUL_STRING_H
#define HELPFUL_STRING_H

using namespace std;

char const* helpful_string =
"pmt [options] pattern textfile [textfile...]\noptions:\n-e, --edit i: set maximum editing distance to i.\n-p, --pattern patternfile: search for all patterns in patternfile. patterns must be separated by a line break. if this option is used, then arguments are instead read as:\n    pmt [options] textfile [textfile...]\n-a, --algorithm algorithm_name: uses the algorithm algorithm_name to perform the search. algorithm_name may be any one of:\n    boyer-moore\n    shift-or\n    sellers\n    ukkonen\n-c, --count: display only the amount of occurences of each pattern in each text.\n-h, --help: display this message.\n";

#endif
