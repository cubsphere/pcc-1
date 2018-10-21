#ifndef ALGORITHM_MANAGER_H
#define ALGORITHM_MANAGER_H

#include <fstream>
#include "boyer-moore.hpp"
#include "shift-or.hpp"

const int STRING_SIZE = 1048576; //1MB
const int STRING_SIZE_LESS = 4096; //4KB

bool verify_algorithm(char const* algorithm_name, int edit_distance);
void process_text(ifstream &text_file, char* pat, int patlen, char const* algorithm_name, bool count_mode, int edit_distance);
void use_boyer_moore(ifstream &text_file, char* pat, int patlen, bool count_mode);
void use_shift_or(ifstream &text_file, char* pat, int patlen, bool count_mode);

#endif
