#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
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
using GraphMap = unordered_map<string, vector<string>>;
class Graph {
    public:
        explicit Graph (const string& filename, class_graph_debug& debug)
    {
        try
        {
            ifstream graph_list_file(filename);
            string graph_string;

            //Read file, create graph_map
            while (getline(graph_list_file, graph_string))
            {
                debug << "[DG] String : " << graph_string << "\n";

                stringstream graph_string_desc(graph_string);
                string graph_word;
                string graph_first_word;
                vector<string> graph_string_vec;
                graph_string_desc >> graph_first_word;

                debug << "[DG] " << graph_first_word << " : ";

                graph_string_desc >> graph_word; //delete : from string
                while (graph_string_desc >> graph_word)
                {
                    graph_string_vec.push_back(graph_word);
                    debug << graph_word << " ";
                }
                debug << "\n";

                graph_map.insert(make_pair(graph_first_word,graph_string_vec));
            }
            debug << "\n";
            //Select top elements
            //Select all keys
            set<string> all_vertex;
            for (const auto& key_element : graph_map)
            {
                debug << "[D] Key map value : " << key_element.first << "\n";
                all_vertex.insert(key_element.first);
            }

            //Remove all, that are not first
            set<string> non_first_vertex;
            for (const auto& key_element : graph_map)
            {
                debug << "[Di1] Key map value : " << key_element.first << "\n";
                for (const auto& other_element : key_element.second)
                {
                    if (other_element != key_element.first)
                    {
                        debug << "\t\t" << other_element;
                        non_first_vertex.insert(other_element);
                    }
                }
                debug << "\n";
            }

            set_difference(all_vertex.begin(), all_vertex.end(), non_first_vertex.begin(), non_first_vertex.end(), inserter(top_elements, top_elements.end()));
        }
        catch(...)
        {
            abort();
        }
    }

        GraphMap graph_map_value() const
        {
            return graph_map;
        }

        vector<string> top_elements_value() const
        {
            return top_elements;
        }
    private:
        GraphMap graph_map;
        vector <string> top_elements;
};
