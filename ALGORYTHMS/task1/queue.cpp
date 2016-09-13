#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

#include "stack.h"
#include "myqueue.h"

using namespace std;

using number_t = size_t;

decltype(auto) create_number_list(const number_t& range_from, const number_t& range_to, const number_t& vec_size)
{
    default_random_engine generator((random_device())());
    uniform_int_distribution<number_t> random_size(range_from, range_to);

    vector<number_t> test_number_list(vec_size);

    cout << "RANDOM LIST : ";
    for (auto& test_number : test_number_list)
    {
        auto rand_num = random_size(generator);
        test_number = rand_num;
        cout << rand_num << " ";
    }
    cout << endl;

    return test_number_list;
}

size_t get_rnd_limited(const size_t num_min, const size_t num_max, const size_t operations_amount, const size_t max_amount)
{
    default_random_engine generator((random_device())());
    uniform_int_distribution<number_t> random_size(num_min, num_max);
    size_t rnd_num = random_size(generator);

    if (rnd_num + operations_amount <= max_amount)
        return rnd_num;
    else
        return (max_amount - operations_amount);
}

int main()
{
    number_t min_num { 1 };
    //    number_t max_num { 99999999 };
    number_t max_num { 999 };
    number_t vec_size { 15 };

    auto number_list = create_number_list(min_num, max_num, vec_size);

    myqueue <number_t> queue1;
    myqueue <pair<number_t, number_t>> queue2;
    myqueue <int> queue3;

    size_t total_push_count { 0 };
    size_t total_pull_count { 0 };
    number_t min_value { numeric_limits<number_t>::max() };
    int push_min_value_v3 { 0 };
    int pull_min_value_v3 { 0 };
    while (total_push_count < vec_size - 1 or total_pull_count < vec_size - 1)
    {
        auto push_rand_num = get_rnd_limited(1, 20, total_push_count, vec_size);
        for (decltype(push_rand_num) i = 0; i < push_rand_num; ++i)
        {
            // First variation
            queue1.push_back(number_list[total_push_count]);

            // Second variation
            min_value = min(min_value, number_list[total_push_count]);
//            stack_in1.push_back(make_pair(number_list[total_push_count], min_value));
            queue2.push_back(make_pair(number_list[total_push_count], min_value));

            // Third variation
            if (total_push_count == 0)
            {
                push_min_value_v3 = number_list[0];
                queue3.push_back(number_list[0]);
            }
            else
            {
                int dnum = number_list[total_push_count] - push_min_value_v3;
                queue3.push_back(dnum);

                if (dnum < 0)
                    push_min_value_v3 = number_list[total_push_count];
            }

            ++total_push_count;
        }

        auto pull_rand_num = get_rnd_limited(1, 20, total_pull_count, total_push_count);

        string cout_str1 { "" };
        string cout_str2 { "" };
        string cout_str3 { "" };
        for (decltype(pull_rand_num) i = 0; i < pull_rand_num; ++i)
        {
            // First variation
            cout_str1 += to_string(queue1.pop_front()) + " ";;

            // Second variation
            auto queue2_out = queue2.pop_front();
            cout_str2 += to_string(queue2_out.first) + " (" + to_string(queue2_out.second) + ") ";

            // Third variation
            if (total_pull_count == 0)
            {
                pull_min_value_v3 = queue3.pop_front();
                cout_str3 += to_string(pull_min_value_v3) + " [" + to_string(pull_min_value_v3) + "] ";
            }
            else
            {
                int element_stack_out3 = queue3.pop_front();
                cout_str3 += to_string(element_stack_out3 + pull_min_value_v3) + " ["
                            + to_string(pull_min_value_v3) + "] ";
                if (element_stack_out3 < 0)
                     pull_min_value_v3 = element_stack_out3 + pull_min_value_v3;
            }

            ++total_pull_count;
        }
        cout << cout_str1 << endl;
        cout << cout_str2 << endl;
        cout << cout_str3 << endl;
    }
}
