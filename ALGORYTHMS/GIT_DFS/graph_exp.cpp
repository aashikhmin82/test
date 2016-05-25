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

#include "graph_element.h"
#include "graph_help.h"
#include "graph_map.h"
#include "topological_sort.h"

using namespace std;
namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& print_debug_flag, string& filename, string& to_point)
{
    try
    {
        po::options_description desc("Options");
        string file_name;
        desc.add_options()
            ("help,h", "Show help")
            ("file,f", po::value<string>(&filename), "Filename with the graph. Format:\n  Element : el1 el2 el3")
            ("to_point,to", po::value<string>(&to_point), "Select destination point")
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
            print_debug_flag = true;
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

size_t dfs_func(const string obj_priv_value, const string obj_value, size_t discover_value, const map<string, shared_ptr<class_graph_element>>& graph_objects_list, const map<string, vector<string>>& graph_map)
{
    graph_objects_list.find(obj_value)->second->colour() = "grey";
    if (obj_priv_value == "0")
        graph_objects_list.find(obj_value)->second->priv() = graph_objects_list.find(obj_priv_value)->second;
    graph_objects_list.find(obj_value)->second->steps_count_discover() = ++discover_value;

    for (const auto& graph_obj_value_i : graph_map.find(obj_value)->second)
    {
        if (graph_objects_list.find(graph_obj_value_i)->second->colour() == "white")
            discover_value = dfs_func(obj_value, graph_obj_value_i, discover_value, graph_objects_list, graph_map);
    }
    
    graph_objects_list.find(obj_value)->second->colour() = "black";
    graph_objects_list.find(obj_value)->second->steps_count_finish() = ++discover_value;

    return discover_value;
}

int main(int argc, char ** argv)
{
    string filename = "graph_list1.txt";
    string to_point = "6";
    bool print_debug_flag = false;

    if (!process_command_line(argc, argv, print_debug_flag, filename, to_point))
        return false;

    class_graph_debug print_debug(print_debug_flag);
    Graph graph(filename, print_debug);

    auto graph_map = graph.graph_map_value();

    map<string, shared_ptr<class_graph_element>> graph_objects_list;

    for (const auto& i : graph_map)
    {
        class_graph_element graph_object_element(i.first);
        graph_objects_list.insert(pair<string, shared_ptr<class_graph_element>>(i.first, make_shared<class_graph_element> (class_graph_element(i.first))));
        cout << i.first << " : ";//"s : " << i.second << "\n";
    }

//DEBUG output
    for (const auto& i : graph_objects_list)
        cout << "[Obj] " << i.first << " : " << i.second->value() << "  (" << graph_objects_list[i.first]->value() << ")" << endl;
    print_debug.print_debug_graph(graph_map, graph_objects_list);
    print_debug << "Top Elements : ";
    auto top_elements = graph.top_elements_value();
    for (const auto& top_element : graph.top_elements_value())
        print_debug << top_element << " ";
    print_debug << "\n";
/////////

    size_t discover_value = 0;
    string fake_value_before_first = "0";
    string first_value = top_elements[0];
    vector<string> topological_sorted_vec = topoligical_sort(graph, print_debug); 
    
    print_debug << "After DFS : \n";
}
