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
    vector<bool> uniq_numbers_bool_vec(9000000, false);
    bool get_file_thread_finished { false };

    thread get_files_content(get_file_content, in_filename, ref(numbers_from_file_vec),
                             ref(get_file_thread_finished), ref(debug));
    thread print_file_content(consider_numbers, ref(get_file_thread_finished), ref(numbers_from_file_vec),
                              ref(uniq_numbers_bool_vec), ref(debug));

    get_files_content.join();
    print_file_content.join();

    sort_vec(numbers_from_file_vec, uniq_numbers_bool_vec, debug);

    cout << "Number from file : " << endl;
    for(const auto& numb_i : numbers_from_file_vec) {
        cout << numb_i << endl;
    }
}
