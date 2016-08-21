#include "boost/program_options.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "debug.h"
#include "primes.h"

using number_t = size_t;
using namespace std;

namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& debug_flag)
{
    po::options_description desc("Options");
    string file_name;
    desc.add_options()
        ("help,h", "Show help")
        ("debug,d", "Enable debug")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return false;
    }

    if (vm.count("debug"))
    {
        cout << "-d found: DEBUG is ON" << endl;
        debug_flag = true;
    }

    return true;
}

void create_number_list(const number_t range_from, const number_t range_to, vector <number_t>& test_number_list)
{
    default_random_engine generator((std::random_device())());
    uniform_int_distribution<number_t> random_size(range_from, range_to);

    for (auto& test_number : test_number_list)
    {
        auto rand_num = random_size(generator);
        test_number = rand_num;
    }
}

template <typename check_num_t>
bool is_prime(const check_num_t& check_num)
{
    if(check_num <= 1)
        return false;

    size_t root = sqrt((double)check_num);

    size_t x { 2 };
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

template <typename list_t, typename debug_t>
decltype(auto) prime_search1(const list_t& test_number_list, debug_t& debug)
{
    debug << "Prime Search1 func\n";
    decltype(test_number_list[0]) return_value {0};

    for (auto& test_number : test_number_list)
    {
        debug << test_number << " ";
        if (is_prime(test_number))
        {
            debug << "\nFound the Prime : " << test_number << "\n";
            return test_number;
        }
    }
    return return_value;
}

template <typename list_t, typename max_num_t, typename debug_t>
decltype(auto) prime_search2(const list_t& test_number_list, max_num_t& max_num, debug_t& debug)
{
    debug << "Prime Search2 func\n";
    prime_numbers <max_num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    debug << "PrimesSearch2 [Primes List] : ";
    decltype(max_num) found_prime { max_num };
    for (const auto& check_number : test_number_list)
    {
        if (primes_list.find(check_number) != primes_list.end())
        {
            debug << "Found : " << check_number << "\n";
            found_prime = check_number;
            break;
        }
    }

    return found_prime;
}

int main(int argc, char** argv)
{
    bool debug_flag {false};

    try
    {
        process_command_line(argc, argv, debug_flag);
    }
    catch(std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown error!" << "\n";
        return false;
    }

    print_debug debug(debug_flag);

    vector <number_t> test_number_list(150);
    number_t min_num { 1 };
//    number_t max_num { 99999999 };
    number_t max_num { 999999 };
    create_number_list(min_num, max_num, test_number_list);

    auto prime_func1 = prime_search1(test_number_list, debug);
    cout << "First Prime: " << prime_func1 << endl;

    prime_search2(test_number_list, max_num, debug);
}
