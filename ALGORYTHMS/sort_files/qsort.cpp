#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "qsort.h"

using namespace std;
using data_t = std::vector < std::pair<string, size_t>>;

void quick_sort(data_t& list, size_t min_i, size_t max_i, default_random_engine generator)
{
    uniform_int_distribution <size_t> pivot_rnd(min_i, max_i);
    size_t pivot_i = pivot_rnd(generator);

    swap(list[pivot_i], list[max_i]);

    size_t wall { min_i };
    for (size_t i = min_i; i < max_i; ++i)
    {
        if (list[i].second < list[max_i].second)
        {
            if (wall != i)
                swap(list[wall], list[i]);

            ++wall;
        }
    }

    swap(list[wall], list[max_i]);

    if (wall != 0 and min_i < wall - 1)
        quick_sort(list,min_i, wall - 1, generator);

    if (wall + 1 < max_i)
        quick_sort(list, wall + 1, max_i, generator);
}

void my_qsort(data_t& list)
{
    default_random_engine generator((std::random_device())());

    if (!list.empty())
        quick_sort (list, 0, list.size() - 1, generator);
}

