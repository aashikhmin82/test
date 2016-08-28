#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <cmath>

template <typename number_t>
class prime_numbers_v1
{
    private:
    std::vector <number_t> primes_list { 2 };

    template <typename check_num_t>
        bool is_prime(const check_num_t& check_num)
        {
            if(check_num <= 1)
                return false;
            else if (check_num <= 3)
                return true;

            check_num_t root = sqrt(static_cast<double>(check_num));

            check_num_t x { 2 };
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

    public:
    explicit prime_numbers_v1(const number_t& max_number)
    {
        for (number_t numb = 3; numb <= max_number; numb += 2)
        {
            if (is_prime(numb))
                primes_list.push_back(numb);
        }
    }

    decltype(primes_list)& get_primes_list()
    {
        return primes_list;
    }
};
