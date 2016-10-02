#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <random>

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

bool check_queue(myqueue <number_t> myqueue1, myqueue <pair<number_t, number_t>> myqueue2, myqueue <int> myqueue3)
{
    for (number_t max_amount = 1; max_amount < 500; max_amount += 10)
    {
        number_t min_value { numeric_limits<number_t>::max() };
        queue <number_t> c_queue;

        number_t push_count { 0 }, pull_count { 0 };
        vector <number_t> test_results_q1 (max_amount);
        vector <number_t> test_results_q2 (max_amount);
        vector <int> test_results_q3 (max_amount);

        int push_min_value_q3 { 0 }, pull_min_value_q3 { 0 };

        while (push_count < max_amount or pull_count < max_amount)
        {
            size_t push_rand_num = get_rnd_limited(1, 20, push_count, max_amount);
            size_t pull_rand_num = get_rnd_limited(1, 20, pull_count, max_amount);
            for (size_t i = 0; i < push_rand_num; ++i)
            {
                myqueue1.push_back(push_count);

                min_value = min(min_value, push_count);
                myqueue2.push_back(make_pair(push_count, min_value));

                if (push_count == 0)
                {
                    push_min_value_q3 = push_count;
                    myqueue3.push_back(push_count);
                }
                else
                {
                    int delta = push_count - push_min_value_q3;
                    myqueue3.push_back(delta);

                    if (delta < 0)
                        push_min_value_q3 = push_count;
                }

                c_queue.push(push_count);

                ++push_count;
            }

            for (size_t i = 0; i < pull_rand_num; ++i)
            {
                if (c_queue.empty())
                    break;

                test_results_q1[pull_count] = myqueue1.pop_front();

                auto front_el = myqueue2.pop_front();
                test_results_q2[pull_count] = front_el.first;
                if (front_el.second != 0)
                {
                    cerr << "Incorrect min value in the myqueue2 : " << front_el.second << endl;
                    return false;
                }

                if (pull_count == 0)
                {
                    pull_min_value_q3 = myqueue3.pop_front();
                    test_results_q3[pull_count] = pull_min_value_q3;
                }
                else
                {
                    int queue3_front = myqueue3.pop_front();
                    test_results_q3[pull_count] = queue3_front + pull_min_value_q3;

                    if (queue3_front < 0)
                        pull_min_value_q3 = queue3_front + pull_min_value_q3;

                    if (pull_min_value_q3 != 0)
                    {
                        cerr << "Incorrect min value in the myqueue3 : " << pull_min_value_q3 << endl;
                        return false;
                    }
                }

                if (test_results_q1[pull_count] != c_queue.front() or
                    test_results_q2[pull_count] != c_queue.front() or
                    test_results_q3[pull_count] != static_cast<int>(c_queue.front()))
                {
                    cerr << "Element from tested queue has to be equal to the one from STL queue" << endl;
                    return false;
                }

                c_queue.pop();

                ++pull_count;
            }
        }

        if (test_results_q1[0] != 0 or
            test_results_q2[0] != 0 or
            test_results_q3[0] != 0)
        {
            cerr << "Incorrect first element : " << test_results_q1[0] << " "
                                                 << test_results_q2[0] << " "
                                                 << test_results_q3[0] << endl;
            return false;
        }

        if (test_results_q1.size() != max_amount or
            test_results_q2.size() != max_amount or
            test_results_q3.size() != max_amount)
        {
            cerr << "Result size is not correct" << endl;
            return false;
        }

        if (test_results_q1.back() != max_amount - 1 or
            test_results_q2.back() != max_amount - 1 or
            test_results_q3.back() != static_cast<int>(max_amount - 1))
        {
            cerr << "Incorrect last element : " << test_results_q1.back() << " "
                                                << test_results_q2.back() << " "
                                                << test_results_q3.back() << endl;
            return false;
        }
    }

    return true;
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
            cout_str1 += to_string(queue1.pop_front()) + " ";

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

    cout << "Test (queue1) : " << (check_queue(queue1, queue2, queue3) ? "Passed" : "Failed") << endl;
}
