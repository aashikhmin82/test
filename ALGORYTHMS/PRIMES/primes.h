#pragma once

#include <iostream>
#include <set>

template <typename number_t>
class prime_numbers
{
    private:
        std::set <size_t> primes_list;

    public:
        explicit prime_numbers(const number_t& max_number)
        {
            std::set <number_t> primes_list_tmp;
            for (number_t numb = 3; numb <= max_number; numb += 2)
            {
                primes_list_tmp.insert(numb);
            }

            number_t max_check_num = std::sqrt((double)max_number); 
            std::cout << "[Prime Num Class ] Max_Check_Num : " << max_check_num << "\n";
            number_t cur_numb { 0 };
            std::cout << "[Prime Num Class ] Curr Number : ";
            while (cur_numb < max_check_num)
            {
                std::cout << cur_numb << " ";
                cur_numb = *primes_list_tmp.begin();
                primes_list.insert(*primes_list_tmp.begin());
                primes_list_tmp.erase(primes_list_tmp.begin()); 
                for (number_t numb = cur_numb; numb <= max_number; numb += cur_numb)
                {
                    primes_list_tmp.erase(numb);
                }
            }

            for (const auto& other_primes : primes_list_tmp)
            {
                primes_list.insert(other_primes);
            }
    }

    std::set <size_t>& get_primes_list()
    {
        return primes_list;
    }
};
