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

default_random_engine re((random_device())());

string random_string(size_t max_size)
{
    uniform_int_distribution<size_t> random_size {1, max_size};
    uniform_int_distribution<char> random_letter{'a', 'z'};

    string s (random_size(re), '?' );

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

bool compare_methods (const string& needle, const string& heystack,
        const bool check_state = false, const bool expected_result = true) {

    bool fms_test { true }, compare_test { true }, regex_test { true }, boyer_moore_test { true };
    if (heystack.size() <= needle.size()) {
        if (!fsm_check_match(needle, heystack))
            fms_test = false;

    } else {
        fms_test = false;
    }

    if (!match_with_quadratic_complexity(needle, heystack))
        compare_test = false;

    if (!regex_check_match(needle, heystack))
        regex_test = false;

    if (!boyer_moore_match(needle, heystack))
        boyer_moore_test = false;

    if (fms_test != compare_test or fms_test != regex_test or fms_test != boyer_moore_test) {
        cout << "Test : Failed" << endl;
        cout << "[Error] Test failed. fms : " << fms_test << "\tcompare : " << compare_test << "\tregex : " << regex_test << endl;
        cout << "[Error] Test String : " << needle << endl;
        cout << "[Error] Search String : " << heystack << endl;
        return false;
    }

    if (check_state) {
        if (expected_result != fms_test) {
            cerr << "Expected result : " << expected_result << " \tActual result : " << fms_test << "\n";
            return false;
        }
    }

    return true;
}

bool test1() {
    for (size_t i = 0; i < 10000; ++i) {
        uniform_int_distribution<size_t> random_max_size1(1, 10000);
        size_t max_size1 { random_max_size1(re) };

        uniform_int_distribution<size_t> random_max_size2(1, 20);
        size_t max_size2 { random_max_size2(re) };

        string random_needle { random_string(max_size1) };

        string random_heystack { random_string(max_size2) };

        if (!compare_methods(random_needle, random_heystack))
            return false;
    }

    return true;
}

bool test2() {
    const vector<vector<string>> test_data { {"", ""}, {"", "aaa"}, {"aaa", ""} };
    size_t i { 1 };

    for (const auto& data : test_data) {
        cout << "Test2 : " << i << "\n";
        if (!compare_methods(data[0], data[1])) {
            cerr << "test2." << i << " failed\n";
            return false;
        }

        ++i;
    }

    return true;
}

bool test3() {
    string needle { "kdfeifsdsababcdkfeeio"}; string heystack { "ababc" };
    if (!compare_methods(needle, heystack, true, true)) {
        cerr << "test3.1 failed\n";
        return false;
    }

    needle = "kdfeifsdsfdsdfsfcdkfeeio"; heystack = "ababc";
    if (!compare_methods(needle, heystack, true, false)) {
        cerr << "test3.1 failed\n";
        return false;
    }

    return true;
}

int main() {
    vector <pair<string, decltype(test1())>> tests { {"test1()", test1()}, {"test2()", test2()}, {"test3()", test3()} };

    for (const auto& test : tests)
        cout << test.first << " : " << (test.second ? "Passed" : "Failed" ) << endl;
}
