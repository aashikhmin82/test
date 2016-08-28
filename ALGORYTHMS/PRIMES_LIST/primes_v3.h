#pragma once

#include <iostream>
#include <vector>

template <typename number_t>
class prime_numbers_v3
{
    private:
    std::vector <bool> bool_primes_list { false, false, true };
    std::vector <number_t> primes_list;

    public:
    explicit prime_numbers_v3(const number_t& max_number)
    {
        for (number_t numb = 3; numb <= max_number; numb += 2)
        {
            bool_primes_list.push_back(true);
            bool_primes_list.push_back(false);
        }

        number_t max_check_num = std::sqrt((double)max_number) + 1;
        number_t cur_numb { 3 };
        while (cur_numb < max_check_num)
        {
            if (bool_primes_list[cur_numb])
            {
                for (number_t numb = 2 * cur_numb; numb <= max_number; numb += cur_numb)
                {
                    bool_primes_list[numb] = false;
                }
            }

            ++cur_numb;
        }
    }

    decltype(primes_list)& get_primes_list()
    {
        number_t i = 0;
        for (const auto& bool_prime_element : bool_primes_list)
        {
            if (bool_prime_element)
                primes_list.push_back(i);

            ++i;
        }

        return primes_list;
    }
};
