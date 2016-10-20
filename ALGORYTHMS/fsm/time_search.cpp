#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "search_func.h"
#include "debug.h"
#include "fsm.h"

using namespace std;
using times_t = vector<pair<size_t, vector <size_t>>>;

default_random_engine re((random_device())());

string random_string(const size_t string_size)
{
//    uniform_int_distribution<size_t> random_size {1, max_size};
    uniform_int_distribution<char> random_letter{'a', 'z'};

    string s (string_size, '?' );

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

void compare_methods (const string& test_string, const string& search_string, times_t& times_map, const size_t str_size) {
    typedef chrono::system_clock time;
    typedef chrono::microseconds mks;

//    decltype(time::now()) start_fsm_create_time, end_fsm_create_time;
    auto start_time = time::now();
//    if (search_string.size() <= test_string.size()) {
//        start_fsm_create_time = time::now();
//        fsm fsm_matrix { search_string };
//        auto fin_state_matrix = fsm_matrix.get();
//        end_fsm_create_time = time::now();

//        fsm_check_match(test_string, search_string, fin_state_matrix);
        fsm_check_match(test_string, search_string);
//    }
    auto end_time = time::now();
    size_t fsm_time = chrono::duration_cast<mks>(end_time - start_time).count();
//    size_t fsm_create_time = chrono::duration_cast<mks>(end_fsm_create_time - start_fsm_create_time).count();
//    cout << "FSM time : " << fsm_time << endl;

    start_time = time::now();
    compare_check_match(test_string, search_string);
    end_time = time::now();
    size_t compare_time = chrono::duration_cast<mks>(end_time - start_time).count();
//    cout << "Compare time : " << compare_time << endl;
    /*
    cout << "B";

    cout << "A";
    start_time = time::now();
    cout << "A";
    regex_check_match(test_string, search_string);
    cout << "A";
    end_time = time::now();
    cout << "A";
    size_t regex_time = chrono::duration_cast<mks>(end_time - start_time).count();
    cout << "Regex time : " << regex_time << endl;
    */
    size_t regex_time { 0 };

    vector<size_t> times { fsm_time, compare_time, regex_time };
//    times_map.push_back( {str_size, { fsm_time, compare_time, regex_time, fsm_create_time }} );
    times_map.push_back( {str_size, { fsm_time, compare_time, regex_time }} );
}

times_t check_time() {
    times_t times_map;
    for (size_t string_size = 20; string_size < 1000000; string_size += 10000) {
 //       cout << "\nString size : " << string_size << endl;
        string random_test_string { random_string(string_size) };

        const size_t search_string_size { 10 };
        string random_search_string { random_string(search_string_size) };

        compare_methods(random_test_string, random_search_string, times_map, string_size);
    }

    return times_map;
}

int main() {
    auto times = check_time();

    cout << "Str size  FSM time  Compare Time  Regex time" << endl;
    for (const auto& times_el : times) {
        cout << times_el.first;
        for (const auto& times_value : times_el.second)
            cout << "\t" << times_value;

        cout << endl;
    }
}
