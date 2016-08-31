#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

#include "stack.h"

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
    stack <number_t> stack_in;
    stack <number_t> stack_out;

    stack <pair<number_t, number_t>> stack_in1;
    stack <pair<number_t, number_t>> stack_out1;

    stack <int> stack_in3;
    stack <int> stack_out3;

    size_t total_push_count { 0 };
    size_t total_pull_count { 0 };
    number_t min_value { numeric_limits<number_t>::max() };
    number_t push_min_value_v3 { 0 };
    number_t pull_min_value_v3 { 0 };
    while (total_push_count < vec_size - 1 and total_pull_count < vec_size - 1)
    {
        auto push_rand_num = get_rnd_limited(1, 20, total_push_count, vec_size);
        for (decltype(push_rand_num) i = 0; i < push_rand_num; ++i)
        {
            // First variation
            stack_in.push_back(number_list[total_push_count]);

            // Second variation
            min_value = min(min_value, number_list[total_push_count]);
            stack_in1.push_back(make_pair(number_list[total_push_count], min_value));

            // Third variation
            if (total_push_count == 0)
            {
                push_min_value_v3 = number_list[0];
                stack_in3.push_back(number_list[0]);
            }
            else
            {
                int dnum = number_list[total_push_count] - push_min_value_v3;
                stack_in3.push_back(dnum);

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
            if (stack_out.empty())
                while (!stack_in.empty())
                    stack_out.push_back(stack_in.pop_back());

            cout_str1 += to_string(stack_out.pop_back()) + " ";

            // Second variation
            if (stack_out1.empty())
                while (!stack_in1.empty())
                    stack_out1.push_back(stack_in1.pop_back());

            auto element_stack_out1 = stack_out1.pop_back();
            cout_str2 += to_string(element_stack_out1.first) + " (" + to_string(element_stack_out1.second) + ") ";

            // Third variation
            if (stack_out3.empty())
                while (!stack_in3.empty())
                    stack_out3.push_back(stack_in3.pop_back());

            if (total_pull_count == 0)
            {
                pull_min_value_v3 = stack_out3.pop_back();
                cout_str3 += to_string(pull_min_value_v3) + " (" + to_string(pull_min_value_v3) + ") ";
            }
            else
            {
                auto element_stack_out3 = stack_out3.pop_back();
                cout_str3 += to_string(element_stack_out3 + push_min_value_v3) + " ["
                            + to_string(push_min_value_v3) + "] ";
                if (element_stack_out3 < 0)
                     pull_min_value_v3 = element_stack_out3 + push_min_value_v3;
            }

            ++total_pull_count;
        }
        cout << cout_str1 << endl;
        cout << cout_str2 << endl;
        cout << cout_str3 << endl;
    }
}
