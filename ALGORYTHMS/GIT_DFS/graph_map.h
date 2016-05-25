#pragma once

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

#include "graph_help.h"

using namespace std;
using GraphMap = map<string, vector<string>>;
class Graph {
    public:
        explicit Graph (const string& file_name, class_graph_debug& print_debug) : filename(file_name)
    {
        try
        {
            ifstream graph_list_file(filename);
            string graph_string;

            //Read file, create graph_map
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
            //Select top elements
            //Select all keys
            for (const auto& key_element : graph_map)
            {
                print_debug << "[D] Key map value : " << key_element.first << "\n";
                top_elements.push_back(key_element.first);
            }

            //Remove all, that are not uniq
            for (const auto& key_element : graph_map)
            {
                print_debug << "[Di1] Key map value : " << key_element.first << "\n";
                for (const auto& other_element : key_element.second)
                {
                    if (!(other_element == key_element.first))
                    {
                        print_debug << "\t\t" << other_element;
                        top_elements.erase(remove(top_elements.begin(), top_elements.end(), other_element), top_elements.end());
                    }
                }
                print_debug << "\n";
            }
        }
        catch(...)
        {
            abort();
        }
    }

        const GraphMap graph_map_value() const
        {
            return graph_map;
        }

        const vector<string> top_elements_value() const
        {
            return top_elements;
        }
    private:
        string filename;
        GraphMap graph_map;
        vector <string> top_elements;
};
