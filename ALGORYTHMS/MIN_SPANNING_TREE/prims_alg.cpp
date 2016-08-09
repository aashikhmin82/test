#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#include "graph_map.h"
#include "graph_help.h"

using namespace std;

template<typename node_map, typename edge_map>
void add_weighted_edge(edge_map& weighted_edge, const vertex_t& from_node, const node_map& to_node_n_weight, const vector<vertex_t>& min_stp_node_list)
{
    for (auto& node_i : to_node_n_weight)
    {
        auto to_node = node_i.first;
        auto weight = node_i.second;

        if (find(min_stp_node_list.begin(), min_stp_node_list.end(),to_node) == min_stp_node_list.end())
        {
            vector <vertex_t> edge {from_node, to_node};
            weighted_edge.emplace(weight, edge);
        }
    }
};

template<typename graph_map, typename graph_debug>
vector<vertex_t> prims_select_min_stp(graph_map&& vertex_map, graph_debug& debug)
{
    vector<vertex_t> min_stp;
    vertex_t min_edge, vertex_to_add;

    multimap <weight_t, vector<vertex_t>> weighted_edge_map;
    vector<vertex_t> min_stp_node_list;

    auto init_node = vertex_map.cbegin()->first;
    auto node_n_weight = vertex_map.cbegin()->second;
    min_stp_node_list.push_back(init_node);
    add_weighted_edge(weighted_edge_map, init_node, node_n_weight, min_stp_node_list);

    while (!weighted_edge_map.empty())
    {
        for (auto& edge_element : weighted_edge_map)
        {
            auto from_node = edge_element.second[0];
            auto to_node = edge_element.second[1];
            if (find(min_stp_node_list.begin(), min_stp_node_list.end(),to_node) == min_stp_node_list.end())
            {
                min_stp_node_list.push_back(to_node);
                min_stp.push_back(from_node);
                min_stp.push_back(to_node);

                node_n_weight = vertex_map.find(to_node)->second;
                add_weighted_edge(weighted_edge_map, to_node, node_n_weight, min_stp_node_list);

                break;
            }
            else
            {
                auto it = weighted_edge_map.find(edge_element.first);
                weighted_edge_map.erase(it);
            }
        }
    }

    return min_stp;
}

vector<vertex_t> prims_alg(const Graph& graph, graph_debug& debug)
{
    debug << "Test Prims Alg";
    debug << "\n";

    auto graph_map = graph.graph_map_value();

    debug << "Vertex : ";

    return (prims_select_min_stp(move(graph_map), debug));
}
