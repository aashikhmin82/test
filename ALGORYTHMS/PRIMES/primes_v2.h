#pragma once

#include <iostream>
#include <vector>

template <typename number_t>
class prime_numbers_v2
{
    private:
        std::vector <bool> primes_list { false, true, true };

    public:
        explicit prime_numbers_v2(const number_t& max_number)
        {
            for (number_t numb = 3; numb <= max_number; numb += 2)
            {
                primes_list.emplace_back(true);
                primes_list.emplace_back(false);
            }

            number_t max_check_num = std::sqrt((double)max_number);
            number_t cur_numb { 3 };
            while (cur_numb < max_check_num)
            {
                if (primes_list[cur_numb])
                {
                    for (number_t numb = 2 * cur_numb; numb <= max_number; numb += cur_numb)
                    {
                        primes_list[numb] = false;
                    }
                }

                ++cur_numb;
            }
    }

    decltype(primes_list)& get_primes_list()
    {
        return primes_list;
    }
};
