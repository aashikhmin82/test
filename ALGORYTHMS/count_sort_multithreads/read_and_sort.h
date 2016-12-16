#include <string>
#include <vector>

#include "debug.h"

void get_file_content(const std::string input_filename, std::vector<size_t>& numbers_from_file,
                        bool& get_file_thread_finished, print_debug& debug);
void consider_numbers(bool& get_file_thread_finished, std::vector<size_t>& numbers_from_file,
                   std::vector<bool>& uniq_numbers_bool_vec, print_debug& debug);
void sort_vec(std::vector<size_t>& numbers_from_file, std::vector<bool>& uniq_numbers_bool_vec, print_debug& debug);
