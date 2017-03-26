#pragma once

#include <unordered_map>
#include <vector>

#include "debug.h"

using anagram_map_t = std::unordered_map<std::string, std::vector<std::string>>;

void get_file_content(const std::string& input_filename, bool& get_file_thread_finished,
                      anagram_map_t& anagrams_map, print_debug& debug);
void print_map(const anagram_map_t& anagram_map);
