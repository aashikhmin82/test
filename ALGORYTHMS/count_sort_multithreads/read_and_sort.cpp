#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "debug.h"

using namespace std;

void get_file_content(const string input_filename, vector<size_t>& numbers_from_file, bool& get_file_thread_finished, print_debug& debug) {
    ifstream input_fd(input_filename);

    string input_number;
    size_t i{0};
    debug << "[print_file_content] Filename : " << input_filename << "\n";

    if (input_fd.is_open()) {
        while (input_fd >> input_number) {
            int number = stoi(input_number);
            debug << "Input (" << i << ") : " << input_number << " -> " << number << "\n";
            numbers_from_file.push_back(number);
            ++i;
        }
    }

    get_file_thread_finished = true;
    debug << "[print_file_content] End\n";
}

void consider_numbers(bool& get_file_thread_finished, vector<size_t>& numbers_from_file,
                   vector<bool>& uniq_numbers_bool_vec, print_debug& debug) {
    size_t i { 0 };

    while (!get_file_thread_finished or i < numbers_from_file.size()) {
        for (size_t j = i; j < numbers_from_file.size(); ++j) {
            debug << "[print_content] : " << i << " -> " << numbers_from_file[i] << "\n";
            uniq_numbers_bool_vec[numbers_from_file[i] - 1000000] = true;
            ++i;
        }
    }

    debug << "[print_content] END\n";
}

void sort_vec(vector<size_t>& numbers_from_file, vector<bool>& uniq_numbers_bool_vec, print_debug& debug) {
    vector<size_t> numbers_from_file_tmp (numbers_from_file.size());
    size_t i { 0 };
    size_t number { 0 };

    for (const auto& number_exists : uniq_numbers_bool_vec) {
        if (number_exists) {
            numbers_from_file_tmp[i] = number + 1000000;
            ++i;
        }

        ++number;
    }

    numbers_from_file = numbers_from_file_tmp;
}
