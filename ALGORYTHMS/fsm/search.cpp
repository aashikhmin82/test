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

bool process_command_line(int argc, char ** argv, bool& debug_enable, string& search_string, string& test_string)
{
    try
    {
        po::options_description desc("Options");
        string file_name;
        desc.add_options()
            ("help,h", "Show help")
            ("search,s", po::value<string>(&search_string), "Search")
            ("test_string,t", po::value<string>(&test_string), "String where to search in")
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
    string search_string { "" };
    string test_string { "" };

    if (!process_command_line(argc, argv, debug_enable, search_string, test_string))
                return false;

    print_debug debug(debug_enable);
    debug << "Search string : " << search_string << "\n";

    cout << (fsm_check_match(test_string, search_string) ? "[fsm_check_match] Found!" :
                                                           "[fsm_check_match] Not found!") << endl;

    cout << (match_with_quadratic_complexity(test_string, search_string) ? "[match_with_quadratic_complexity] Found!" :
                                                               "[match_with_quadratic_complexity] Not found!") << endl;

    cout << (regex_check_match(test_string, search_string) ? "[regex_check_match] Found!" :
                                                             "[regex_check_match] Not found!") << endl;
}
