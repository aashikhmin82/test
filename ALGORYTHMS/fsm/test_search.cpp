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

bool compare_methods (const string& test_string, const string& search_string,
        const bool check_state = false, const bool expected_result = true) {

    bool fms_test { true }, compare_test { true }, regex_test { true };
    if (search_string.size() <= test_string.size()) {
        if (!fsm_check_match(test_string, search_string))
            fms_test = false;

    } else {
        fms_test = false;
    }

    if (!compare_check_match(test_string, search_string))
        compare_test = false;

    if (!regex_check_match(test_string, search_string))
        regex_test = false;

    if (fms_test != compare_test or fms_test != regex_test) {
        cout << "Test : Failed" << endl;
        cout << "[Error] Test failed. fms : " << fms_test << "\tcompare : " << compare_test << "\tregex : " << regex_test << endl;
        cout << "[Error] Test String : " << test_string << endl;
        cout << "[Error] Search String : " << search_string << endl;
        return false;
    }

    if (check_state) {
        if (expected_result != fms_test) {
            cerr << "Expected result : " << expected_result << " \tActual result : " << fms_test << endl;
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

        string random_test_string { random_string(max_size1) };

        string random_search_string { random_string(max_size2) };

        if (!compare_methods(random_test_string, random_search_string))
            return false;
    }

    return true;
}

bool test2() {
    string test_string { "" }; string search_string { "" };
    if (!compare_methods(test_string, search_string)) {
        cerr << "test2.1 failed" << endl;
        return false;
    }

    test_string = ""; search_string = "aaa";
    if (!compare_methods(test_string, search_string)) {
        cerr << "test2.2 failed" << endl;
        return false;
    }

    test_string = "aaa"; search_string = "";
    if (!compare_methods(test_string, search_string)) {
        cerr << "test2.3 failed" << endl;
        return false;
    }

    return true;
}

bool test3() {
    string test_string { "kdfeifsdsababcdkfeeio"}; string search_string { "ababc" };
    if (!compare_methods(test_string, search_string, true, true)) {
        cerr << "test3.1 failed" << endl;
        return false;
    }

    test_string = "kdfeifsdsfdsdfsfcdkfeeio"; search_string = "ababc";
    if (!compare_methods(test_string, search_string, true, false)) {
        cerr << "test3.1 failed" << endl;
        return false;
    }

    return true;
}

int main() {

    cout << "Test1 : " << (test1() ? "Passed" : "Failed" ) << endl;

    cout << "Test2 : " << (test2() ? "Passed" : "Failed" ) << endl;

    cout << "Test3 : " << (test3() ? "Passed" : "Failed" ) << endl;
}
