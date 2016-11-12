#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "boost/program_options.hpp"

#include "search_func.h"
#include "debug.h"
#include "fsm.h"

using namespace std;
namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& debug_enable, string& needle, string& heystack)
{
    try
    {
        po::options_description desc("Options");
        string file_name;
        desc.add_options()
            ("help,h", "Show help")
            ("search,s", po::value<string>(&needle), "Search")
            ("heystack,t", po::value<string>(&heystack), "String where to search in")
            ("debug,d", "Enable debug")
            ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return false;
        }

        if (vm.count("debug")) {
            debug_enable = true;
        }
    }
    catch(std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
    catch(...) {
        cerr << "Unknown error!" << "\n";
        return false;
    }

    return true;
}

int main(int argc, char ** argv)
{
    bool debug_enable { false };
    string needle { "" };
    string heystack { "" };

    if (!process_command_line(argc, argv, debug_enable, needle, heystack))
                return false;

    print_debug debug(debug_enable);
    debug << "Search string : " << needle << "\n";

    cout << (fsm_check_match(heystack, needle) ? "[fsm_check_match] Found!" :
                                                           "[fsm_check_match] Not found!") << endl;

    cout << (match_with_quadratic_complexity(heystack, needle) ? "[match_with_quadratic_complexity] Found!" :
                                                               "[match_with_quadratic_complexity] Not found!") << endl;

    cout << (regex_check_match(heystack, needle) ? "[regex_check_match] Found!" :
                                                             "[regex_check_match] Not found!") << endl;

    cout << (boyer_moore_match(heystack, needle) ? "[boyer_moore_match] Found!" :
                                                             "[boyer_moore_match] Not found!") << endl;

}
