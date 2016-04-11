#include <iostream>
#include <fstream>
#include <map>
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

map<string, vector<string>> create_graph_map(class_graph_debug print_debug, string filename)
{
    ifstream graph_list_file(filename);
    string graph_string;
    map<string, vector<string>> graph_map;

    while (getline(graph_list_file, graph_string))
    {
        print_debug << "[DG] String : " << graph_string << "\n";

        stringstream graph_string_desc(graph_string);
        string graph_word;
        string graph_first_word;
        vector<string> graph_string_vec;
        graph_string_desc >> graph_first_word;

        print_debug << "[DG] " << graph_first_word << " : ";

        graph_string_desc >> graph_word; //delete : from string
        while (graph_string_desc >> graph_word)
        {
            graph_string_vec.push_back(graph_word);
            print_debug << graph_word << " ";
        }
        print_debug << "\n";

        graph_map.insert(pair<string, vector<string>>(graph_first_word,graph_string_vec));
    }
    print_debug << "\n";

    return graph_map;
}

int main(int argc, char ** argv)
{
    string filename = "graph_list1.txt";
    string to_point = "6";
    bool print_debug_flag = false;

    if (!process_command_line(argc, argv, print_debug_flag, filename, to_point))
        return false;

    class_graph_debug print_debug(print_debug_flag);

    map<string, vector<string>> graph_map = create_graph_map(print_debug, filename);
    map<string, class_graph_element*> graph_objects_list;

    for (const auto& i : graph_map)
    {
        class_graph_element graph_object_element(i.first);
        graph_objects_list.insert(pair<string, class_graph_element*>(i.first, new class_graph_element(i.first)));
        cout << i.first << " : ";//"s : " << i.second << "\n";
    }

    for (const auto& i : graph_objects_list)
    {
        cout << "[Obj] " << i.first << " : " << i.second->value() << "  (" << graph_objects_list[i.first]->value() << ")" << endl;
    }

    print_debug.print_debug_graph(graph_map, graph_objects_list);

    queue<string> objects_queue;
    objects_queue.push(graph_objects_list["1"]->value());
    while (!objects_queue.empty())
    {
        string queue_element = objects_queue.front();

        print_debug << "[D] Queue Element : " << queue_element << "\n";
        print_debug << "[D] Graph Map Val : " << graph_objects_list[queue_element]->value() << "\n";

        objects_queue.pop();
        for (const auto& j : graph_map[queue_element])
        {
            size_t count = graph_objects_list[queue_element]->steps_count();
            ++count;
            if (graph_objects_list[j]->colour() == "white")
            {
                print_debug << "Pushed to the queue : " << j << "\n";

                objects_queue.push(j);
                graph_objects_list[j]->colour() = "black";
                graph_objects_list[j]->priv() = graph_objects_list[queue_element];
                graph_objects_list[j]->steps_count() = count;
                cout << "[DEL] " << graph_objects_list[queue_element] << endl;
                graph_objects_list[j]->priv() = graph_objects_list[queue_element];
            }
        }
    }

    print_debug.print_debug_result(graph_map, graph_objects_list);
    cout << "\n\nWay from " << to_point << " to 1 : " << endl;
    cout << graph_objects_list[to_point]->value() << endl;
    class_graph_element* priv_element = graph_objects_list[to_point]->priv();
    while (priv_element)
    {
        cout << priv_element->value() << endl;
        priv_element = priv_element->priv();
    }
}
