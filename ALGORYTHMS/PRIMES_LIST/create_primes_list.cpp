#include "boost/program_options.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <vector>

#include "debug.h"
#include "primes_v1.h"
#include "primes_v2.h"
#include "primes_v3.h"

using number_t = size_t;
using debug_t = print_debug;

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
/*
template <typename num_t>
decltype(auto) create_primes_list1(const num_t& max_num, debug_t& debug)
{
    debug << "Create Primes List1 func\n";

    prime_numbers_v1 <num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    return primes_list;
}

template <typename num_t>
decltype(auto) create_primes_list2(const num_t& max_num, debug_t& debug)
{
    debug << "Create Primes List2 func\n";

    prime_numbers_v2 <num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    return primes_list;
}

template <typename num_t>
decltype(auto) create_primes_list3(const num_t& max_num, debug_t& debug)
{
    debug << "Create Prime List3 func\n";
    prime_numbers_v3 <num_t> primes(max_num);
    auto primes_list = primes.get_primes_list();

    return primes_list;
}
*/

template <typename list_t>
void print_primes(const list_t& primes_list, const bool debug_flag, const string& desc = "Unknown Function")
{
    if (debug_flag)
    {
        cout << "Function (" << desc << ") -> Primes : ";
        for (const auto prime : primes_list)
            cout << prime << " ";

        cout << endl;
    }
}

decltype(auto) check_func_duration(const number_t& max_num, const size_t func_amount, const bool debug_flag, debug_t& debug)
{
    typedef chrono::system_clock time;
    typedef chrono::microseconds mks;

    auto start_time = time::now();
//    auto primes_list1 = create_primes_list1(max_num, debug);
    prime_numbers_v1 <number_t> primes1(max_num);
    auto end_time = time::now();

    auto primes_list1 = primes1.get_primes_list();
    print_primes(primes_list1, debug_flag, "Function 1 (Check all numbers)");

    using duration_t = decltype(chrono::duration_cast<mks>(end_time - start_time).count());
    vector<duration_t> func_durations(func_amount);
    func_durations[0] = chrono::duration_cast<mks>(end_time - start_time).count();

    start_time = time::now();
//    auto primes_list2 = create_primes_list2(max_num, debug);
    prime_numbers_v2 <number_t> primes2(max_num);
    end_time = time::now();

    auto primes_list2 = primes2.get_primes_list();
    print_primes(primes_list2, debug_flag, "Function 2 (Using std::set)");

    func_durations[1] = chrono::duration_cast<mks>(end_time - start_time).count();

    start_time = time::now();
//    auto primes_list3 = create_primes_list3(max_num, debug);
    prime_numbers_v3 <number_t> primes3(max_num);
    end_time = time::now();
    auto primes_list3 = primes3.get_primes_list();
    print_primes(primes_list3, debug_flag, "Function 3 (std::vector<bool>)");

    func_durations[2] = chrono::duration_cast<mks>(end_time - start_time).count();

    assert(primes_list1 == primes_list3);
    assert(primes_list2 == primes_list3);

    return func_durations;
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

    vector<number_t> max_num_list { 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000 };
    //vector<number_t> max_num_list { 100, 500, 1000, 5000, 10000, 50000, 100000 };
    size_t func_amount { 3 };

    string filename = "result.md";
    ofstream result_file(filename, ios::out);

    cout << "Max Number\t" 
        << "Function 1 - Check all numbers (mks)\t"
        << "Function 2 - Using std::set (mks)\t"
        << "Function 3 - std::vector<bool> (mks)\t"
        << "(Min Value)" << endl;

    result_file << "Max Number | "
        << "Function 1 - Check all numbers (mks) | "
        << "Function 2 - Using std::set (mks) | "
        << "Function 3 - std::vector<bool> (mks) | "
        << "Fastest Function" << endl;

    result_file << "-------------:|"
        << "------------------------------------------:|"
        << "------------------------------------------:|"
        << "------------------------------------------:|"
        << "-----------------------" << endl;

    for (const auto max_num : max_num_list)
    {
        auto func_durations = check_func_duration(max_num, func_amount, debug_flag, debug);

        auto minmax = minmax_element(func_durations.begin(), func_durations.end());
        auto min_func_i = minmax.first - func_durations.begin();
        vector <string> bold_font(3);
        bold_font[min_func_i] = "**";

        cout << max_num << "\t"
            << func_durations[0] << "\t"
            << func_durations[1] << "\t"
            << func_durations[2] << "\t"
            << "(Func " << (min_func_i + 1) << ")" << endl;

        result_file << max_num << " | ";
        result_file << bold_font[0] << func_durations[0]<< bold_font[0] << " | ";
        result_file << bold_font[1] << func_durations[1]<< bold_font[1] << " | ";
        result_file << bold_font[2] << func_durations[2]<< bold_font[2] << " | ";
        result_file << "Function " << min_func_i + 1 << endl;
    }

    result_file.close();
/*
    auto duration = func_durations[0];
    map<decltype(duration), string> function_time;
    function_time.insert({func_durations[0], "Function 1 (Check All Numbers)"});
    function_time.insert({func_durations[1], "Function 2 (Using std::set)"});
    function_time.insert({func_durations[2], "Function 3 (std::vector<bool>)"});

    cout << "Function\t\tDuration (ms)" << endl;
    for (const auto& f_time : function_time)
    {
        cout << f_time.second << "\t\t" << f_time.first << endl;
    }
    */
}
