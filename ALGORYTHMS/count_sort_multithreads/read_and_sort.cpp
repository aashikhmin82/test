#include <bitset>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "debug.h"

using namespace std;

const size_t proc_arch { 32 };
const size_t min_permitted_number { 1000000 };

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
                   vector<bool>& uniq_numbers_bool_vec, vector<size_t>& numbers_bit_vec, print_debug& debug) {
    size_t i { 0 };

    while (!get_file_thread_finished or i < numbers_from_file.size()) {
        for (size_t j = i; j < numbers_from_file.size(); ++j) {
            debug << "[print_content] : " << i << " -> " << numbers_from_file[i] << "\n";
            uniq_numbers_bool_vec[numbers_from_file[i] - min_permitted_number] = true;

            size_t bucket_num = (numbers_from_file[i] - min_permitted_number) / proc_arch;
            size_t from_num = proc_arch * bucket_num;
            size_t bit_number = 1 << (numbers_from_file[i] - min_permitted_number - from_num);
            numbers_bit_vec[bucket_num] = numbers_bit_vec[bucket_num] | bit_number;

            ++i;
        }
    }
}

void sort_vec(vector<size_t>& numbers_from_file, vector<bool>& uniq_numbers_bool_vec, print_debug& debug) {
    vector<size_t> numbers_from_file_tmp (numbers_from_file.size());
    size_t i { 0 };
    size_t number { 0 };

    for (const auto& number_exists : uniq_numbers_bool_vec) {
        if (number_exists) {
            numbers_from_file_tmp[i] = number + min_permitted_number;
            ++i;
        }

        ++number;
    }

    numbers_from_file = numbers_from_file_tmp;
}

void sort_numbers(vector<size_t>& numbers_from_file, vector<size_t>& numbers_bit_vec, print_debug& debug) {
    vector<size_t> numbers_from_file_tmp (numbers_from_file.size());
    size_t k { 0 };

    for (size_t i = 0; i < numbers_bit_vec.size(); ++i) {
        size_t bit_element = numbers_bit_vec[i];

        if (bit_element != 0) {
            auto bit_value = bitset<proc_arch>(bit_element);
            debug << i << " : " << bit_element << " (" << bit_value << ")\n";
            for(size_t j = 0; j < proc_arch; ++j) {
                size_t bit = bit_element & 1;

                if (bit) {
                    auto bit_num_value = (proc_arch * i + j) + min_permitted_number;
                    debug << "Bit Num : " << j << " (" << bit_num_value << ")\n";
                    numbers_from_file_tmp[k] = bit_num_value;
                    ++k;
                }

                bit_element = bit_element >> 1;
            }
        }
    }

    numbers_from_file = numbers_from_file_tmp;
}
