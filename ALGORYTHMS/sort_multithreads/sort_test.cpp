#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#include "multithread_qsort.h"

using namespace std;
using data_t = vector<size_t>;

void check_result (const data_t& test_vec, const data_t& check_vec, int& exit_code,
                    const size_t code_value = 1, const string& desc = "Test")
{
    cout << desc << " : ";
    if (test_vec != check_vec)
    {
        cout << "Failed" << endl;
        exit_code = code_value;
        assert(test_vec == check_vec);
    }
    else
    {
        cout << "Passed" << endl;
    }
}

data_t create_rnd_vec(const size_t vec_size, const size_t max_value)
{
    vector<size_t> numbers_vec;
    data_t test_vec_rnd (vec_size);

    default_random_engine generator((random_device())());
    uniform_int_distribution <size_t> rnd_num(0, max_value);

    for (auto& vec_element : test_vec_rnd)
    {
        vec_element = rnd_num(generator);
    }

    return test_vec_rnd;
}

int main()
{
    int exit_code { 0 };
    size_t thread_amount { 2 };

    //Test1
    data_t test_vec_multi { 200, 150, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20 };
//    data_t test_vec_multi { 10,4,8,2,5,1,6,7,3,21,15,17,44,40 };
    auto test_vec_multi_check = test_vec_multi;
    sort(test_vec_multi_check.begin(), test_vec_multi_check.end());

    my_multithread_sort(test_vec_multi, thread_amount);
    check_result(test_vec_multi, test_vec_multi_check, exit_code, 3, "Test1.1");

    cout << "Sorted Vector : ";
    for (const auto& test_vec_multi_i : test_vec_multi) {
        cout << test_vec_multi_i << " ";
    }
    cout << endl;

    data_t test_vec1 { 9169755, 9042061, 340429, 1660266, 6873819, 5713239, 7814901, 7363629, 9258859, 1360609, 8109504, 1468742, 9155658, 2909853, 5496889, 4278055, 722935, 8059806, 9142093, 5737896, 766007, 9460823 };
    auto test_vec1_check = test_vec1;
    sort(test_vec1_check.begin(), test_vec1_check.end());

    my_multithread_sort(test_vec1, thread_amount);
    check_result(test_vec1, test_vec1_check, exit_code, 3, "Test1.2");

    data_t test_vec_multi2 { 200, 150, 120 };
    auto test_vec_multi2_check = test_vec_multi2;
    sort(test_vec_multi2_check.begin(), test_vec_multi2_check.end());

    my_multithread_sort(test_vec_multi2, thread_amount);
    check_result(test_vec_multi2, test_vec_multi2_check, exit_code, 0, "Test1.3");

    //Test2
    size_t j { 1 }, error_code { 4 };
    for (size_t vec_size = 0; vec_size < 1000; ++vec_size)
    {
        cout << "vec_size : " << vec_size << endl;
        size_t max_rnd_num { 9999999 };
        data_t test_vec_rnd = create_rnd_vec(vec_size, max_rnd_num);
        auto test_vec_rnd_check = test_vec_rnd;

        sort(test_vec_rnd_check.begin(), test_vec_rnd_check.end());
        for (size_t threads_amount = 0; threads_amount < 17; ++threads_amount) {
            my_multithread_sort(test_vec_rnd, threads_amount);

            string description = "Test2." + to_string(j) + ":" + to_string(threads_amount);
            check_result(test_vec_rnd, test_vec_rnd_check, exit_code, error_code, description);
        }
        ++j;
        ++error_code;
    }

    return exit_code;
}
