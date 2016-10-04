#include <algorithm>
#include <iostream>
#include <random>
#include <regex>
#include <vector>

#include "boost/program_options.hpp"

#include "debug.h"
#include "files.h"
#include "myhash.h"
#include "primes.h"
#include "qsort.h"

using namespace std;
using fsize_vec_t = vector < pair<string, size_t>>;
using fstat_vec_t = vector < pair<string, vector<pair<string, size_t>>>>;

namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& debug_enable, string& dir, string& search_regexp)
{
    try
    {
        po::options_description desc("Options");
        string file_name;
        desc.add_options()
            ("help,h", "Show help")
            ("dir,D", po::value<string>(&dir), "Folder with files")
            ("regexp,re", po::value<string>(&search_regexp), "Files regexp")
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

        if (vm.count("dir")) {
            cout << "Search Dir : " << vm["dir"].as<string>() << "\n";
        }
    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
    catch(...) {
        std::cerr << "Unknown error!" << "\n";
        return false;
    }

    return true;
}

int main(int argc, char ** argv)
{
    bool debug_enable { false };
    string dir_path { "." };
    string search_regexp_string { ".*" };
//    const regex exe_regex(".*\\.exe");

    if (!process_command_line(argc, argv, debug_enable, dir_path, search_regexp_string))
                return false;

    print_debug debug(debug_enable);

    regex search_regexp (search_regexp_string);
/*
    string dir_path { "/home/aav/TEST/" };
    if (argc == 2)
    {
        dir_path = arg_vec[1];
        if (dir_path == "--help")
        {
            cerr << "./programm <path>" << endl;
            return 1;
        }
    }
    */

    const auto files = get_files(dir_path);
    cout << dir_path << " : " << endl;
    fsize_vec_t files_vec;
    fstat_vec_t fstat_vec;
    for (const auto& file : files)
    {
        auto file_size = get_file_size(dir_path + "/" + file);
        auto regex_matched_flag = regex_match(file, search_regexp);
        debug << file << " (" << file_size << ")" << " Reg : " << regex_matched_flag << "\n";

        if (file_size >= 0 and regex_matched_flag)
            files_vec.push_back(make_pair(file, file_size));

        fstat_vec.push_back(make_pair(file, get_file_stat(file)));
    }

    my_qsort(files_vec);
    cout << "\nSorted File vec : " << endl;
    for (const auto& file_attr : files_vec)
        cout << file_attr.first << " (" << file_attr.second << ")" << endl;
    cout << endl;

    myhash file_hash(fstat_vec);
    file_hash.dump_hash();

    while (true)
    {
        cout << "Enter the filename to get stat information or enter exit [exit] : ";
        string filename;
        getline(cin, filename);

        if (filename.empty() or filename == "exit")
            break;

        cout << filename << " : " << endl;
        file_hash.dump_file_stat(filename);
    }

}
