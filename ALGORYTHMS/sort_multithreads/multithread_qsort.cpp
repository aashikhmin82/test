#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "debug.h"
#include "multithread_qsort.h"

using namespace std;

size_t get_random_it_num(const size_t min, const size_t max) {

    default_random_engine generator((std::random_device())());
    uniform_int_distribution <size_t> pivot_rnd(min, max);

    return pivot_rnd(generator);
}

size_t partition_by_pivot(const size_t min_i, const size_t max_i, vector<size_t>& test_vec, print_debug& debug) {
    size_t pivot_i = get_random_it_num(min_i, max_i);
    debug << "[D] Pivot : " << pivot_i << "  (" << test_vec[pivot_i] << ")\n";
    debug << "[D partition_by_pivot] min : " << min_i << "  max : " << max_i << "\n";
    swap(test_vec[pivot_i], test_vec[max_i]);

    size_t wall { min_i };
    for (size_t i = min_i; i < max_i; ++i)
    {
        if (test_vec[i] < test_vec[max_i])
        {
            if (wall != i)
                swap(test_vec[wall], test_vec[i]);

            ++wall;
        }
    }

    swap(test_vec[wall], test_vec[max_i]);

    return wall;
}

void sort_thread(const size_t min_i, const size_t max_i, vector<size_t>& test_vec, print_debug& debug) {
    debug << "[sort_thread] min_i : " << min_i << "  max_i : " << max_i << "\n";
    if (min_i <= max_i) {
        if (min_i != max_i) {
            size_t wall = partition_by_pivot(min_i, max_i, test_vec, debug);
            debug << "[sort_thread] Wall : " << wall << "\n";

            if (min_i != wall) {
                debug << "[sort_thread L ] Run low sort_thread -> min_i: " << min_i << "  wall : " << wall << "\n";
                sort_thread(min_i, wall, test_vec, debug);
            }

            if (wall + 1 < max_i) {
                debug << "[sort_thread H ] Run high sort_thread -> wall : " << wall << "  max : " << max_i  << "\n";
                sort_thread(wall + 1, max_i, test_vec, debug);
            }
        }
    } else {
        debug << "[sort_thread] Min > Max\n";
    }
}

void create_threads(auto& test_vec, const auto& walls_vector, auto& debug) {
    vector<thread> thread_vec;

    for(size_t i = 0; i < walls_vector.size() - 1; ++i) {
        size_t min = walls_vector[i] + 1;
        if (i == 0)
            min = 0;

        thread_vec.push_back(thread(sort_thread, min, walls_vector[i + 1], ref(test_vec), ref(debug)));
    }

    for(auto& thread_sort : thread_vec) {
        thread_sort.join();
    }
}

size_t div_vec_into_two_intervals(auto& test_vec, auto& walls_vector, auto& debug) {
    const size_t wall = partition_by_pivot(0, test_vec.size() - 1, test_vec, debug);
    walls_vector.push_back(wall);

    if (wall != test_vec.size() - 1) {
        walls_vector.push_back(test_vec.size() - 1);
    }

    return wall;
}

void save_state_and_reset_counters(auto& walls_vector_tmp, auto& walls_vector, auto& test_vec,
                                   auto& walls_vector_size_prev, auto& wall_i, auto& min) {
    if (walls_vector_tmp.back() != test_vec.size() - 1) {
        walls_vector_tmp.push_back(test_vec.size() - 1);
    }

    walls_vector_size_prev = walls_vector_tmp.size() - 1;
    wall_i = 1;
    walls_vector = walls_vector_tmp;

    walls_vector_tmp.resize(1);
    min = 0;
}

void divide_vec_to_parts(auto& test_vec, auto& walls_vector, size_t threads_amount, auto& debug) {
    size_t thread_i { 1 }, wall_i { 0 }, wall { 0 };
    size_t walls_vector_size_prev { 2 };
    vector<size_t> walls_vector_tmp { 0 };

    while (thread_i < threads_amount and wall < test_vec.size() - 1 ) {
        ++thread_i;

        if (thread_i == 2) {

            wall = div_vec_into_two_intervals(test_vec, walls_vector, debug);

        } else {
            size_t min = walls_vector[wall_i];

            if (wall_i == walls_vector_size_prev) {

                save_state_and_reset_counters(walls_vector_tmp, walls_vector, test_vec, walls_vector_size_prev,
                                              wall_i, min);
            } else {
                ++wall_i;
            }

            size_t max = walls_vector[wall_i];
            wall = partition_by_pivot(min, max, test_vec, debug);

            if (wall != min and wall != max)
                walls_vector_tmp.push_back(wall);

            walls_vector_tmp.push_back(max);
        }

        if (thread_i == threads_amount and thread_i != 2) {
           ++wall_i;
           for (; wall_i < walls_vector_size_prev; ++wall_i) {
               walls_vector_tmp.push_back(walls_vector[wall_i]);
           }
           walls_vector = walls_vector_tmp;
        }
    }
}

void my_multithread_sort (vector<size_t>& test_vec, size_t threads_amount) {
    const bool debug_enable { false };
    print_debug debug(debug_enable);

    vector<size_t> walls_vector { 0 };

    if (test_vec.size() < 2)
        return;

    if (threads_amount == 0) {
        threads_amount = 1;
        walls_vector.push_back(test_vec.size() - 1);
    } else if (test_vec.size() < threads_amount) {
        threads_amount = test_vec.size();
    }

    if (threads_amount > 1)
        divide_vec_to_parts(test_vec, walls_vector, threads_amount, debug);

    create_threads(test_vec, walls_vector, debug);
}
