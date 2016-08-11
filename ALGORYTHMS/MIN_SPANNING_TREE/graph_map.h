#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph_help.h"

using weight_t = size_t;
using vertex_t = std::string;
using GraphMap = std::unordered_map<vertex_t, std::unordered_map<vertex_t, weight_t>>;
using edge_weight_map = std::map<weight_t, std::vector<vertex_t>>;

template <typename T>
void process_weighted_node_string(const T& node_with_weight, std::unordered_map<vertex_t, weight_t>& edges_of_vertex, graph_debug& debug)
{
    debug << " '" << node_with_weight << "' ";

    //find (
    auto pos1 = node_with_weight.find("(");
    //find )
    auto pos2 = node_with_weight.find(")");
    vertex_t vertex_value = node_with_weight.substr(0,pos1);
    std::string edge_weight = node_with_weight.substr(pos1+1, pos2-2);
    debug << "==== " << vertex_value << " " << edge_weight << " ====";
    edges_of_vertex.insert(make_pair(vertex_value, std::stoi(edge_weight)));
}

class Graph {
    public:
        Graph (const std::vector<std::vector<std::string>>& input_graph, graph_debug& debug)
        {
            for (const auto& input_string : input_graph)
            {
                debug << "STRING : ";
                std::unordered_map<vertex_t, weight_t> edges_of_vertex;
                bool is_first_it = true;
                for (const auto& node_i : input_string)
                {
                    if (is_first_it)
                    {
                        debug << input_string[0] << " : ";
                        is_first_it = false;
                        continue;
                    }

                    debug << node_i << " ";
                    process_weighted_node_string(node_i, edges_of_vertex, debug);
                }

                graph_map.insert(make_pair(input_string[0],edges_of_vertex));
            }
        }

        Graph (const std::string& filename, graph_debug& debug)
        {
            std::ifstream graph_list_file(filename);
            std::string graph_string;

            //Read file, create graph_map
            while (std::getline(graph_list_file, graph_string))
            {
                debug << "[DG] String : " << graph_string << "\n";

                std::stringstream graph_string_desc(graph_string);
                std::string graph_word;
                vertex_t graph_first_word;
                graph_string_desc >> graph_first_word;

                debug << "[DG] " << graph_first_word << " : ";

                graph_string_desc >> graph_word; //delete : from string

                std::unordered_map<vertex_t, weight_t> edges_of_vertex;
                while (graph_string_desc >> graph_word)
                    process_weighted_node_string(graph_word, edges_of_vertex, debug);

                debug << "\n";

                graph_map.insert(make_pair(graph_first_word,edges_of_vertex));
            }
            debug << "\n";
            //Select top elements
            //Select all keys
            std::set<vertex_t> all_vertex;
            for (const auto& key_element : graph_map)
            {
                debug << "[D] Key map value : " << key_element.first << "\n";
                all_vertex.insert(key_element.first);
                top_elements.emplace_back(key_element.first);
            }
        }

        GraphMap graph_map_value() const
        {
            return graph_map;
        }

        std::vector<vertex_t> top_elements_value() const
        {
            return top_elements;
        }
    private:
        GraphMap graph_map;
        std::vector <vertex_t> top_elements;
};
