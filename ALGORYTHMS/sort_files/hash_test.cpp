#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "debug.h"
#include "myhash.h"
#include "primes.h"

using namespace std;
using test_vec_t = vector < pair<string, vector<pair<string, size_t>>>>;
using test_subvec_t = vector<pair<string, size_t>>;

int main(int argc, char* arg_vec[])
{
    bool debug_enable { true };
    print_debug debug(debug_enable);

    size_t test_vec_size { 20 }, sub_vec_size { 10 }, i { 0 };
    test_vec_t test_vec { test_vec_size };
    for (auto& vec_el : test_vec)
    {
        test_subvec_t test_subvec { sub_vec_size };
        size_t j { 0 };
        for (auto& subvec_el : test_subvec)
        {
            subvec_el = make_pair(to_string(i) + "Attr" + to_string(j), j);
            ++j;
        }

        vec_el = make_pair("String" + to_string(i), test_subvec);
        ++i;
    }

    myhash test_hash(test_vec);
    test_hash.dump_hash();

    //Test 1
    cout << "Test 1" << endl;
    for (const auto& test_el : test_vec)
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
    cout << "Test 2 : " << (test_vec.size() == test_hash.size() ? "Passed" : "Failed") << endl;
}
