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
    uniform_int_distribution<char> random_letter{'a', 'd'};

    string s (string_size, '?' );

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

void compare_methods (const string& haystack, const string& needle, times_t& times_map) {
    using time = chrono::system_clock;
    using mks = chrono::microseconds;

    auto start_time = time::now();
    fsm_check_match(haystack, needle);
    auto end_time = time::now();
    size_t fsm_time = chrono::duration_cast<mks>(end_time - start_time).count();

    start_time = time::now();
    match_with_quadratic_complexity(haystack, needle);
    end_time = time::now();
    size_t quadratic_algorithm_time = chrono::duration_cast<mks>(end_time - start_time).count();

    start_time = time::now();
    boyer_moore_match(haystack, needle);
    end_time = time::now();
    size_t boyer_moore_time = chrono::duration_cast<mks>(end_time - start_time).count();

    vector<size_t> times { fsm_time, quadratic_algorithm_time };
    times_map.push_back( {haystack.size(), { fsm_time, quadratic_algorithm_time, boyer_moore_time }} );
}

times_t check_time() {
    times_t times_map;
    for (size_t string_size = 20; string_size < 1000000; string_size += 10000) {
        string random_haystack { random_string(string_size) };

        const size_t needle_size { 10 };
        string random_needle { random_string(needle_size) };

        compare_methods(random_haystack, random_needle, times_map);
    }

    //Test 2 (the worst case for quadratic algorithm)
    string haystack { string(1000000, 'a') + 'b' };
    string needle { string(20, 'a') + 'b' };
    compare_methods(haystack, needle, times_map);

    return times_map;
}

int main() {
    auto times = check_time();

    cout << "Str size  FSM time  Compare Time  (delta)" << endl;
    for (const auto& times_el : times) {
        cout << times_el.first;

        int fsm_time { 0 }, compare_time { 0 };
        for (const auto& times_value : times_el.second)
        {
            fsm_time = compare_time;
            cout << "\t" << times_value;
            compare_time = times_value;
        }
        cout << "  " << fsm_time - compare_time << endl;
    }
}
