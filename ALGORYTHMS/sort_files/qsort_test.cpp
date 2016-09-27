#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "qsort.h"

using namespace std;
using data_t = vector<pair<string, size_t>>;

void check_result (const data_t& test_list, const data_t& check_list, int& exit_code, 
                    const size_t code_value = 1, const string& desc = "Test")
{
    cout << desc << " : ";
    if (test_list != check_list)
    {
        cout << "Failed" << endl;
        exit_code = code_value;
    }
    else
    {
        cout << "Passed" << endl;
    }
}

data_t create_rnd_list(const size_t list_size, const size_t max_value)
{
    vector<size_t> numbers_list;
    data_t test_list_rnd (list_size);

    default_random_engine generator((random_device())());
    uniform_int_distribution <size_t> rnd_num(0, max_value);

    size_t i { 0 };
    for (auto& list_element : test_list_rnd)
    {
        size_t list_number = rnd_num(generator);

        while (find(numbers_list.begin(), numbers_list.end(), list_number) != numbers_list.end())
        {
            list_number = rnd_num(generator);
        }

        string list_string = to_string(list_number);
        list_element = make_pair(list_string, list_number);
        numbers_list.push_back(list_number);
        ++i;
    }

    return test_list_rnd;
}

bool compare(const pair<string, size_t>& i, const pair<string, size_t>& j)
{
    return i.second < j.second;
}

int main()
{
    int exit_code { 0 };

    //Test1
    data_t test_list_null, test_list_null_check;
    my_qsort(test_list_null);
    check_result(test_list_null, test_list_null_check, exit_code, 1, "Test1");

    //Test2
    data_t test_list_one { {"a", 2}, {"b",1} };
    data_t test_list_one_check { {"b",1}, {"a", 2} };
    my_qsort(test_list_one);
    check_result(test_list_null, test_list_null_check, exit_code, 2, "Test2");

    //Test3
    data_t test_list_multi { {"a", 200}, {"b",150}, {"c", 120}, {"d", 110}, {"e", 100}, {"f", 90}, {"g", 80}, {"h", 70}, {"i", 60}, {"j", 50}, {"k", 40}, {"l", 30}, {"m", 20},  };
    data_t test_list_multi_check { {"m", 20},  {"l", 30}, {"k", 40}, {"j", 50}, {"i", 60}, {"h", 70}, {"g", 80}, {"f", 90}, {"e", 100}, {"d", 110}, {"c", 120}, {"b",150}, {"a", 200}, };

    my_qsort(test_list_multi);
    check_result(test_list_multi, test_list_multi_check, exit_code, 3, "Test3");

    //Test4
    size_t j { 1 }, error_code { 4 };
    for (size_t list_size = 0; list_size < 1000; list_size += 3)
    {
        size_t max_rnd_num { 9999999 };
        data_t test_list_rnd = create_rnd_list(list_size, max_rnd_num);
        data_t test_list_rnd_check = test_list_rnd;

        my_qsort(test_list_rnd);
        sort(test_list_rnd_check.begin(), test_list_rnd_check.end(), compare);

        check_result(test_list_rnd, test_list_rnd_check, exit_code, error_code, "Test4." + to_string(j));
        ++j;
        ++error_code;
    }

    return exit_code;
}
