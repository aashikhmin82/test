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

#include "graph_map.h"
#include "graph_help.h"

using namespace std;

template<typename graph_map, typename graph_debug>
vector<vertex_t> prims_select_min_stp(const graph_map&& vertex_map, graph_debug& debug)
{
    vector<vertex_t> min_stp { "T", "E", "S", "T", " ", "P", "r", "i", "m", "s" };

    weight_t edge_weight { SIZE_MAX };
    debug << "MAX DEL : " << edge_weight << "\n";

    vertex_t min_edge, vertex_to_add;
    for (const auto& vertex_i : vertex_map)
    {
        debug << "Prims (del) alg : " << vertex_i.first << " : ";
        min_edge = vertex_i.first;
        for (const auto& edge_i : vertex_i.second)
        {
            if (edge_weight > edge_i.second)
            {
                edge_weight = edge_i.second;
                vertex_to_add = edge_i.second;
                debug << " + <<<<" << edge_i.second << ">>>> ";
            }
            debug << edge_i.first << " (" << edge_i.second << ")" << " ";
        }
        debug << "\n";
        min_edge = min_edge + vertex_to_add;
        debug << "Min Edge : " << min_edge << " (" << vertex_to_add << ")\n";
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
