#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "qsort.h"

using namespace std;
using data_t = std::vector < std::pair<string, size_t>>;

void quick_sort(data_t& vec, size_t min_i, size_t max_i, default_random_engine generator)
{
    uniform_int_distribution <size_t> pivot_rnd(min_i, max_i);
    size_t pivot_i = pivot_rnd(generator);

    swap(vec[pivot_i], vec[max_i]);

    size_t wall { min_i };
    for (size_t i = min_i; i < max_i; ++i)
    {
        if (vec[i].second < vec[max_i].second)
        {
            if (wall != i)
                swap(vec[wall], vec[i]);

            ++wall;
        }
    }

    swap(vec[wall], vec[max_i]);

    if (wall != 0 and min_i < wall - 1)
        quick_sort(vec,min_i, wall - 1, generator);

    if (wall + 1 < max_i)
        quick_sort(vec, wall + 1, max_i, generator);
}

void my_qsort(data_t& vec)
{
    default_random_engine generator((std::random_device())());

    if (!vec.empty())
        quick_sort (vec, 0, vec.size() - 1, generator);
}

