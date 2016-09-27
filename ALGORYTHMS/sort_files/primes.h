#pragma once

#include <cmath>

template <typename num_t>
bool is_prime(const num_t& check_num)
{
    if(check_num <= 1)
        return false;
    else if (check_num <= 3)
        return true;

    num_t root = sqrt(check_num);

    num_t x { 2 };
    if (check_num % x == 0)
        return false;

    x = 3;
    while (x <= root)
    {
        if(check_num % x == 0)
            return false;

        x += 2;
    }

    return true;
}

template <typename num_t>
num_t get_nearest_prime(const num_t& number)
{
    num_t prime_number = number;
    while (!is_prime(prime_number))
        ++prime_number;

    return prime_number;
}
