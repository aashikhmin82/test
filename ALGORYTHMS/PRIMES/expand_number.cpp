#include "boost/program_options.hpp"
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <memory>

#include "debug.h"
#include "primes.h"
#include "primes_v2.h"

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
        cout << desc << "\n";
        return false;
    }

    if (vm.count("debug"))
    {
        cout << "-d found: DEBUG is ON" << endl;
        debug_flag = true;
    }

    return true;
}

decltype(auto) create_number_list(const number_t& range_from, const number_t& range_to, const number_t& vec_size)
{
    default_random_engine generator((random_device())());
    uniform_int_distribution<number_t> random_size(range_from, range_to);

    auto test_number_list = make_unique< vector<number_t> >(vec_size);

    for (auto& test_number : *test_number_list)
    {
        auto rand_num = random_size(generator);
        test_number = rand_num;
    }

    return test_number_list;
}

template <typename check_num_t>
bool is_prime(const check_num_t& check_num)
{
    if(check_num < 1)
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

template <typename list_t, typename num_t, typename debug_t>
decltype(auto) prime_search2(const list_t& test_number_list, const num_t& max_num, debug_t& debug)
{
    debug << "Prime Search2 func\n";
    prime_numbers <num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    debug << "PrimesSearch2 [Primes List] : ";
    num_t found_prime { 0 };
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

template <typename list_t, typename num_t, typename debug_t>
decltype(auto) prime_search3(const list_t& test_number_list, const num_t& max_num, debug_t& debug)
{
    debug << "Prime Search3 func\n";
    prime_numbers_v2 <num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    debug << "PrimesSearch3 [Primes List] : ";
    num_t found_prime { 0 };
    for (const auto& check_number : test_number_list)
    {
        if (primes_list[check_number])
        {
            debug << "Found : " << check_number << "\n";
            found_prime = check_number;
            break;
        }
    }

    return found_prime;
}

template <typename function_t>
decltype(auto) check_duration(function_t&& function, string description = "No informaion")
{
    cout << "Function - " << description << endl;

    auto start_time = chrono::system_clock::now();
    auto function_result = function;
    auto end_time = chrono::system_clock::now();

    auto duration = end_time - start_time;
    cout << duration.count() << " ms" << endl;

    return function_result;
}

int main(int argc, char** argv)
{
    bool debug_flag {false};

    try
    {
        process_command_line(argc, argv, debug_flag);
    }
    catch(exception& e)
    {
        cerr << "Error : " << e.what() << "\n";
        return false;
    }
    catch(...)
    {
        cerr << "Unknown error!" << "\n";
        return false;
    }

    print_debug debug(debug_flag);

    number_t min_num { 1 };
//    number_t max_num { 99999999 };
    number_t max_num { 999999 };
    number_t vec_size { 150 };

    auto test_number_list = create_number_list(min_num, max_num, vec_size);

    auto prime_func1 = check_duration(prime_search1(*test_number_list, debug), "Function 1");
    cout << "Prime (func1): " << prime_func1 << endl;

    auto prime_func2 = check_duration(prime_search2(*test_number_list, max_num, debug), "Function 2");
    cout << "Prime (func2): " << prime_func2 << endl;

    auto prime_func3 = check_duration(prime_search3(*test_number_list, max_num, debug), "Function 3");
    cout << "Prime (func3): " << prime_func3 << endl;

    assert(prime_func2 == prime_func1);
    assert(prime_func3 == prime_func1);
}
