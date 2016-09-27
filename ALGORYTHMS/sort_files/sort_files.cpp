#include <algorithm>
#include <iostream>
#include <random>
#include <regex>
#include <vector>

#include "debug.h"
#include "files.h"
#include "myhash.h"
#include "primes.h"
#include "qsort.h"

using namespace std;
using fsize_list_t = vector < pair<string, size_t>>;
using fstat_list_t = vector < pair<string, vector<pair<string, size_t>>>>;

int main(int argc, char* arg_vec[])
{
    bool debug_enable { true };
    print_debug debug(debug_enable);

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

    const auto files = get_files(dir_path);
    const regex exe_regex(".*\\.exe");
    cout << dir_path << " : " << endl;
    fsize_list_t files_list;
    fstat_list_t fstat_list;
    for (const auto& file : files)
    {
        auto file_size = get_file_size(dir_path + file);
        auto regex_matched_flag = regex_match(file, exe_regex);
        debug << file << " (" << file_size << ")" << " Reg : " << regex_matched_flag << "\n";

        if (file_size >= 0 and regex_matched_flag)
            files_list.push_back(make_pair(file, file_size));

        fstat_list.push_back(make_pair(file, get_file_stat(file)));
    }

    my_qsort(files_list);
    cout << "\nSorted File list : " << endl;
    for (const auto& file_attr : files_list)
        cout << file_attr.first << " (" << file_attr.second << ")" << endl;
    cout << endl;

    myhash file_hash(fstat_list);
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
