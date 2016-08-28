#pragma once

#include <iostream>
#include <set>

template <typename number_t>
class prime_numbers_v2
{
    private:
        std::set <number_t> set_primes_list { 2 };
        std::vector<number_t> primes_list;

    public:
        explicit prime_numbers_v2(const number_t& max_number)
        {
            std::set <number_t> set_primes_list_tmp;
            for (number_t numb = 3; numb <= max_number; numb += 2)
            {
                set_primes_list_tmp.insert(numb);
            }

            number_t max_check_num = std::sqrt((double)max_number) + 1;
            number_t cur_numb { 0 };
            while (cur_numb < max_check_num)
            {
                cur_numb = *set_primes_list_tmp.begin();
                set_primes_list.insert(*set_primes_list_tmp.begin());
                set_primes_list_tmp.erase(set_primes_list_tmp.begin());
                for (number_t numb = cur_numb; numb <= max_number; numb += cur_numb)
                {
                    set_primes_list_tmp.erase(numb);
                }
            }

            for (const auto& other_primes : set_primes_list_tmp)
            {
                set_primes_list.insert(other_primes);
            }
    }

    decltype(primes_list)& get_primes_list()
    {
        for (const auto& prime_i : set_primes_list)
        {
            primes_list.push_back(prime_i);
        }

        return primes_list;
    }
};
