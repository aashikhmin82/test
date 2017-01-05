#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "debug.h"
#include "read_and_sort.h"

using namespace std;

int main() {
    const bool debug_enable { true };
    print_debug debug(debug_enable);

    string in_filename { "testlist.txt" };
    debug << "Filename " << in_filename << "\n";

    vector<size_t> numbers_from_file_vec;
    const size_t numbers_in_range = 9000000;
    const size_t proc_arch = 32;

    vector<bool> uniq_numbers_bool_vec(numbers_in_range, false);
    size_t bit_vec_size = numbers_in_range / proc_arch;
    vector<size_t> numbers_bit_vec(bit_vec_size, 0);

    bool get_file_thread_finished { false };

    thread get_files_content(get_file_content, in_filename, ref(numbers_from_file_vec),
                             ref(get_file_thread_finished), ref(debug));
    thread analyse_file_content(consider_numbers, ref(get_file_thread_finished), ref(numbers_from_file_vec),
                              ref(uniq_numbers_bool_vec), ref(numbers_bit_vec), ref(debug));

    get_files_content.join();
    analyse_file_content.join();

    vector<size_t> numbers_from_file_vec1 = numbers_from_file_vec;
    sort_vec(numbers_from_file_vec, uniq_numbers_bool_vec, debug);
    sort_numbers(numbers_from_file_vec1, numbers_bit_vec, debug);

    cout << "Number from file (sort_vec): " << endl;
    for(const auto& numb_i : numbers_from_file_vec) {
        cout << numb_i << endl;
    }

    cout << "Number from file (sort_numbers): " << endl;
    for(const auto& numb_i1 : numbers_from_file_vec1) {
        cout << numb_i1 << endl;
    }
}
