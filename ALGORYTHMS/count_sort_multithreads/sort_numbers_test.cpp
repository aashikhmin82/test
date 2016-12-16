#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "debug.h"
#include "read_and_sort.h"

using namespace std;

vector<size_t> create_rnd_vec(const size_t vec_size, const size_t max_value)
{
    vector<size_t> test_vec_rnd(vec_size);

    default_random_engine generator((random_device())());
    uniform_int_distribution <size_t> rnd_num(999999, max_value);

    for (size_t& vec_element_i : test_vec_rnd)
    {
        size_t random_element = rnd_num(generator);

        while (find(test_vec_rnd.begin(), test_vec_rnd.end(), random_element) != test_vec_rnd.end()) {
            random_element = rnd_num(generator);
        }

        vec_element_i = random_element;
    }

    return test_vec_rnd;
}

void create_random_test_file(const string& filename, const size_t vec_size) {
    size_t max_rnd_num { 9999999 };
    vector<size_t> test_vec_rnd = create_rnd_vec(vec_size, max_rnd_num);

    ofstream testfile;
    testfile.open(filename);
    for(const auto& vec_i : test_vec_rnd) {
        testfile << vec_i << endl;
    }
}

void create_test_file(const string& filename, const vector<size_t>& numbers_vec) {
    ofstream testfile;
    testfile.open(filename);

    for(const auto& vec_i : numbers_vec) {
        testfile << vec_i << endl;
    }
}

vector<size_t> sort_file_content(const string& filename, vector<size_t>& not_sorted_vec, print_debug& debug) {
    vector<size_t> numbers_from_file_vec;
    vector<bool> uniq_numbers_bool_vec(9000000, false);
    bool get_file_thread_finished { false };

    thread get_files_content(get_file_content, filename, ref(numbers_from_file_vec),
                             ref(get_file_thread_finished), ref(debug));
    thread print_file_content(consider_numbers, ref(get_file_thread_finished), ref(numbers_from_file_vec),
                              ref(uniq_numbers_bool_vec), ref(debug));

    get_files_content.join();
    print_file_content.join();

    not_sorted_vec = numbers_from_file_vec;
    sort_vec(numbers_from_file_vec, uniq_numbers_bool_vec, debug);

    return numbers_from_file_vec;
}

int main() {
//    const bool debug_enable { true };
    const bool debug_enable { false };
    print_debug debug(debug_enable);

    //Test1
    const string in_filename_test1 { "auto_testlist1.txt" };
    const vector<size_t> test1_vec { 9999999, 8888888, 7777777, 6666666, 5555555, 4444444, 3333333, 2222222, 1000000 };
    const vector<size_t> test1_vec_sorted { 1000000, 2222222, 3333333, 4444444, 5555555, 6666666, 7777777, 8888888, 9999999 };
    create_test_file(in_filename_test1, test1_vec);

    vector<size_t> numbers_from_file_vec1;
    auto sorted_vec = sort_file_content(in_filename_test1, numbers_from_file_vec1, debug);
    cout << "Test 1 : " << ( (sorted_vec == test1_vec_sorted) ? "Passed" : "Failed" ) << endl;

    const string in_filename { "auto_testlist.txt" };
    debug << "Filename " << in_filename << "\n";
    //Test2
    cout << "Test 2 : " << endl;
    size_t test_num { 1 };

    for (size_t random_vec_size = 0; random_vec_size < 10; ++random_vec_size) {
        create_random_test_file(in_filename, random_vec_size);

        vector<size_t> numbers_from_file_vec2;
        sorted_vec = sort_file_content(in_filename, numbers_from_file_vec2, debug);

        sort(numbers_from_file_vec2.begin(), numbers_from_file_vec2.end());
        cout << "Test 2." << test_num << " : " << ( (sorted_vec == numbers_from_file_vec2) ? "Passed" : "Failed" ) << endl;
        ++test_num;
    }

    for (size_t random_vec_size = 0; random_vec_size < 6000; random_vec_size += 100) {
        create_random_test_file(in_filename, random_vec_size);

        vector<size_t> numbers_from_file_vec2;
        sorted_vec = sort_file_content(in_filename, numbers_from_file_vec2, debug);

        sort(numbers_from_file_vec2.begin(), numbers_from_file_vec2.end());
        cout << "Test 2." << test_num << " : " << ( (sorted_vec == numbers_from_file_vec2) ? "Passed" : "Failed" ) << endl;
        ++test_num;
    }
}
