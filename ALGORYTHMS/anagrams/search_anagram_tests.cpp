#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "debug.h"
#include "read_and_search.h"

using namespace std;

int main() {
    const bool debug_enable { true };
    print_debug debug(debug_enable);

    //Test 1
    string in_filename { "tests/dic1.txt" };
    debug << "Filename " << in_filename << "\n";

    bool get_file_thread_finished { false };
    map<string, vector<string>> anagrams_map;

    thread find_anagrams_test1(get_file_content, in_filename,
                             ref(get_file_thread_finished), ref(anagrams_map), ref(debug));

    find_anagrams_test1.join();

    map<string, vector<string>> anagrams_map_test1 = { {"aabb", {"aabb", "bbaa", "abab"}} };
    print_map(anagrams_map);
    cout << "Test 1 : " << ((anagrams_map_test1 == anagrams_map) ? "Passed" : "Failed") << endl;

    //Test 2
    in_filename = "tests/dic2.txt";
    debug << "Filename " << in_filename << "\n";

    map<string, vector<string>> anagrams_map2;

    thread find_anagrams_test2(get_file_content, in_filename,
                             ref(get_file_thread_finished), ref(anagrams_map2), ref(debug));

    find_anagrams_test2.join();

    map<string, vector<string>> anagrams_map_test2 = { {"aabbc", {"caabb", "cbbaa", "cabab"}}, {"efghijk", {"kjihgfe", "efghikj", "ghikjef", "jkhiefg"}} };
    print_map(anagrams_map2);
    cout << "Test 1 : " << ((anagrams_map_test2 == anagrams_map2) ? "Passed" : "Failed") << endl;
}
