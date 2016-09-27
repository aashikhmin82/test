#pragma once

#include <vector>

std::vector <std::string> get_files (const std::string& dir);
int get_file_size (const std::string& filename);
std::vector<std::pair<std::string, size_t>> get_file_stat (const std::string& filename);
