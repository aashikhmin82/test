#pragma once

#include <iostream>
#include <vector>
#include <random>

template <typename data_t>
class randoms_list
{
    private:
    std::vector<data_t> list;

    public:
    randoms_list(const data_t& min, const data_t& max, const size_t size)
    {
        std::default_random_engine generator((std::random_device())());
        std::uniform_int_distribution<data_t> random_size(min, max);

        std::cout << "[random_list] (" << min << "," << max << ") RANDOM LIST : ";
        for (size_t i = 0; i < size; ++i)
        {
            auto rand_num = random_size(generator);
            list.push_back(rand_num);
            std::cout << rand_num << " ";
        }
        std::cout << std::endl;
    }

    std::vector<data_t> get()
    {
        return list;
    }
};
