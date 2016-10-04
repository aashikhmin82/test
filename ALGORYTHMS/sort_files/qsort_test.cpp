#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "qsort.h"

using namespace std;
using data_t = vector<pair<string, size_t>>;

void check_result (const data_t& test_vec, const data_t& check_vec, int& exit_code, 
                    const size_t code_value = 1, const string& desc = "Test")
{
    cout << desc << " : ";
    if (test_vec != check_vec)
    {
        cout << "Failed" << endl;
        exit_code = code_value;
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

    size_t i { 0 };
    for (auto& vec_element : test_vec_rnd)
    {
        size_t vec_number = rnd_num(generator);

        while (find(numbers_vec.begin(), numbers_vec.end(), vec_number) != numbers_vec.end())
        {
            vec_number = rnd_num(generator);
        }

        string vec_string = to_string(vec_number);
        vec_element = make_pair(vec_string, vec_number);
        numbers_vec.push_back(vec_number);
        ++i;
    }

    return test_vec_rnd;
}

bool compare(const pair<string, size_t>& i, const pair<string, size_t>& j)
{
    return i.second < j.second;
}

int main()
{
    int exit_code { 0 };

    //Test1
    data_t test_vec_null, test_vec_null_check;
    my_qsort(test_vec_null);
    check_result(test_vec_null, test_vec_null_check, exit_code, 1, "Test1");

    //Test2
    data_t test_vec_one { {"a", 2}, {"b",1} };
    data_t test_vec_one_check { {"b",1}, {"a", 2} };
    my_qsort(test_vec_one);
    check_result(test_vec_null, test_vec_null_check, exit_code, 2, "Test2");

    //Test3
    data_t test_vec_multi { {"a", 200}, {"b",150}, {"c", 120}, {"d", 110}, {"e", 100}, {"f", 90}, {"g", 80}, {"h", 70}, {"i", 60}, {"j", 50}, {"k", 40}, {"l", 30}, {"m", 20},  };
    data_t test_vec_multi_check { {"a", 200}, {"b",150}, {"c", 120}, {"d", 110}, {"e", 100}, {"f", 90}, {"g", 80}, {"h", 70}, {"i", 60}, {"j", 50}, {"k", 40}, {"l", 30}, {"m", 20},  };
    sort(test_vec_multi_check.begin(), test_vec_multi_check.end(), compare);

    my_qsort(test_vec_multi);
    check_result(test_vec_multi, test_vec_multi_check, exit_code, 3, "Test3");

    //Test4
    size_t j { 1 }, error_code { 4 };
    for (size_t vec_size = 0; vec_size < 1000; vec_size += 3)
    {
        size_t max_rnd_num { 9999999 };
        data_t test_vec_rnd = create_rnd_vec(vec_size, max_rnd_num);
        data_t test_vec_rnd_check = test_vec_rnd;

        my_qsort(test_vec_rnd);
        sort(test_vec_rnd_check.begin(), test_vec_rnd_check.end(), compare);

        check_result(test_vec_rnd, test_vec_rnd_check, exit_code, error_code, "Test4." + to_string(j));
        ++j;
        ++error_code;
    }

    return exit_code;
}
