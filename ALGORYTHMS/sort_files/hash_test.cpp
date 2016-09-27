#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "debug.h"
#include "myhash.h"
#include "primes.h"

using namespace std;
using test_list_t = vector < pair<string, vector<pair<string, size_t>>>>;
using test_sublist_t = vector<pair<string, size_t>>;

int main(int argc, char* arg_vec[])
{
    bool debug_enable { true };
    print_debug debug(debug_enable);

    size_t test_list_size { 20 }, sub_list_size { 10 }, i { 0 };
    test_list_t test_list { test_list_size };
    for (auto& list_el : test_list)
    {
        test_sublist_t test_sublist { sub_list_size };
        size_t j { 0 };
        for (auto& sublist_el : test_sublist)
        {
            sublist_el = make_pair(to_string(i) + "Attr" + to_string(j), j);
            ++j;
        }

        list_el = make_pair("String" + to_string(i), test_sublist);
        ++i;
    }

    myhash test_hash(test_list);
    test_hash.dump_hash();

    //Test 1
    cout << "Test 1" << endl;
    for (const auto& test_el : test_list)
    {
        debug << test_el.first << "\n";
        if (test_el.second != test_hash.dump_file_stat(test_el.first))
        {
            cout << "Test failed" << endl;
            return 1;
        }
    }
    cout << "Test 1 : Passed" << endl;

    //Test 2
    cout << "Test 2 : " << (test_list.size() == test_hash.size() ? "Passed" : "Failed") << endl;
}
