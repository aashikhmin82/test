#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#include <random>

template <typename data_t>
std::vector<data_t> randoms_list(const data_t& min, const data_t& max, const size_t size)
{
    std::default_random_engine generator((std::random_device())());
    std::uniform_int_distribution<data_t> random_size(min, max);

//    std::cout << "[random_list] (" << min << "," << max << ") RANDOM LIST : ";

    assert (size >= 1);
    std::vector<data_t> list(size);
    for (auto& list_element : list)
    {
        list_element = random_size(generator);
 //       std::cout << list_element << " ";
    }
//    std::cout << std::endl;

    return list;
}
