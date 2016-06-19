#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <math.h>
#include <string>
#include <algorithm> 
#include <sys/stat.h>
#include <queue>
#include "boost/program_options.hpp"

#include "graph_help.h"
#include "graph_map.h"
#include "kruskal_alg.h"

using namespace std;
namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& debug_flag, string& filename)
{
    try
    {
        po::options_description desc("Options");
        string file_name;
        desc.add_options()
            ("help,h", "Show help")
            ("file,f", po::value<string>(&filename), "Filename with the graph. Format:\n  Element : el1 el2 el3")
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

        if (vm.count("file"))
        {
            cout << "File : " << vm["file"].as<string>() << "\n";
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown error!" << "\n";
        return false;
    }

    return true;
}

int main(int argc, char ** argv)
{
    string filename = "graph_list1.txt";
    bool debug_flag = false;

    if (!process_command_line(argc, argv, debug_flag, filename))
        return false;

    graph_debug debug(debug_flag);
    Graph graph(filename, debug);

    auto min_stp = kruskal_arg(graph, debug);

    cout << "STP : ";
    for (const auto& stp_i : min_stp)
        cout << stp_i << " ";
    cout << endl;
}
