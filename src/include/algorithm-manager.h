#ifndef ALGORITHM_MANAGER_H
#define ALGORITHM_MANAGER_H

#include <fstream>
#include <string>
#include "boyer-moore.h"
#include "shift-or.h"

bool verify_algorithm(string algorithm_name, int edit_distance);
void process_text(ifstream &text_file, string pat, string algorithm_name, bool count_mode, int edit_distance);
void use_boyer_moore(ifstream &text_file, string pat, bool count_mode);
void use_shift_or(ifstream &text_file, string pat, bool count_mode);

#endif
