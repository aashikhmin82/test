#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <cstdlib>
#include <string>

#include "primes.h"

class myhash
{
//    using data_t = std::pair <std::string, size_t>;
    using data_t = std::pair<std::string, std::vector<std::pair<std::string, size_t>>>;
    using hash_t = std::vector <std::vector<data_t>>;

    private:
        hash_t hashed_list;
        size_t m { 0 }, p { 4294967291 }, a { 0 }, b { 0 };

        size_t hash_function(const size_t& k)
        {
            return ((a*k + b)%p)%m;
        }

        size_t str_to_size (const std::string& string)
        {
            size_t result_val { 0 };

            size_t factor { 1 };

            for (const auto& ch : string)
            {
                factor *= p;
                result_val += factor * ch;
            }

            return result_val;
        }

        hash_t create_hash (const std::vector<data_t>& input_list)
        {
            std::cout << "Inupt list size: " << input_list.size() << "  M : " << m << std::endl;
            std::cout << "P : " << p << "  A: " << a << "  B: " << b << std::endl;

            hash_t hashlist(m);

            for (const auto input_el : input_list)
            {
                size_t hash_i = hash_function(str_to_size(input_el.first));
                hashlist[hash_i].push_back(input_el);
            }

            return hashlist;
        }

    public:
        myhash(const std::vector<data_t>& input_list)
        {
            m = get_nearest_prime(input_list.size());

            std::default_random_engine generator((std::random_device())());
            std::uniform_int_distribution <size_t> Za(1, p - 1);
            a = Za(generator);

            std::uniform_int_distribution<size_t> Zb(0, p - 1);
            b = Zb(generator);

            hashed_list = create_hash(input_list);
        }

        void dump_hash()
        {
            size_t i { 0 };
            for (const auto& hash_el : hashed_list)
            {
                std::cout << i << " : ";

                if (hash_el.empty())
                {
                    std::cout << "Empty" << std::endl;
                }
                else
                {
                    for (const auto& hashed_pair : hash_el)
                    {
                        std::cout << hashed_pair.first << " (" << std::endl;
                        for (const auto& fstat : hashed_pair.second)
                            std::cout << fstat.first << " -> " << fstat.second << std::endl;
                        std::cout << ")\n";
                    }

                    std::cout << std::endl;
                }

                ++i;
            }
        }

        std::vector<std::pair<std::string, size_t>> dump_file_stat (const std::string& search_string)
        {
            size_t hash_i = hash_function(str_to_size(search_string));
            if (hashed_list[hash_i].empty())
            {
                std::cerr << "Not found" << std::endl;
            }
            else
            {
                bool found { false };
                for (const auto& hashed_pair : hashed_list[hash_i])
                {
                    if (hashed_pair.first == search_string)
                    {
                        found = true;
                        for (const auto& fstat : hashed_pair.second)
                            std::cout << fstat.first << " -> " << fstat.second << std::endl;

                        return hashed_pair.second;
                    }
                }

                if (!found)
                    std::cerr << "Not found" << std::endl;
            }

            std::vector<std::pair<std::string, size_t>> return_value;
            return return_value;
        }

        size_t size()
        {
            size_t size { 0 };

            for (const auto& hash_el : hashed_list)
                if (!hash_el.empty())
                    size += hash_el.size();

            return size;
        }
};

