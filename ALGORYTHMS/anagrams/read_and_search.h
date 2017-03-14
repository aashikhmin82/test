#pragma once

#include <map>
#include <string>
#include <vector>

#include "debug.h"

void get_file_content(const std::string input_filename, bool& get_file_thread_finished,
                      std::map<std::string, std::vector<std::string>>& anagrams_map, print_debug& debug);
void print_map(const std::map<std::string, std::vector<std::string>> anagram_map);
