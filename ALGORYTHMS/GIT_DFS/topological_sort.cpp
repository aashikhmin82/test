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

void topological_sort_recursion(vector<string>& topologically_sorted, const Graph& graph, const string& graph_vertex)
{
    if (find(topologically_sorted.begin(), topologically_sorted.end(), graph_vertex) == topologically_sorted.end())
    {
        auto graph_map = graph.graph_map_value();
        for (const auto& graph_element : graph_map.find(graph_vertex)->second)
        {
            if (not(graph_element == graph_vertex))
            {
                topologically_sorted.erase(remove(topologically_sorted.begin(), topologically_sorted.end(), graph_vertex), topologically_sorted.end());
                topological_sort_recursion(topologically_sorted, graph, graph_element);
            }
        }
        topologically_sorted.emplace_back(graph_vertex);
    }
}

vector<string> topoligical_sort(const Graph& graph, class_graph_debug& debug)
{
    vector<string> topologically_sorted;
    for (const auto& top_element : graph.top_elements_value())
        topological_sort_recursion(topologically_sorted, graph, top_element);

    debug << "Topologically sorted vector : ";
    for (const auto& debug_element : topologically_sorted)
    {
        debug << debug_element << " ";
    }
    debug << "\n";

    return topologically_sorted;
}

