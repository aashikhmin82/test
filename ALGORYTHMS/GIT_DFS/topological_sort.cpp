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

void topological_sort_recursion(vector<string>& sorted_vec, const Graph& graph, const string& graph_key)
{
    if (find(sorted_vec.begin(), sorted_vec.end(), graph_key) == sorted_vec.end())
    {
        auto graph_map = graph.graph_map_value();
        for (const auto& graph_element : graph_map.find(graph_key)->second)
        {
            if (not(graph_element == graph_key))
            {
                sorted_vec.erase(remove(sorted_vec.begin(), sorted_vec.end(), graph_key), sorted_vec.end());
                topological_sort_recursion(sorted_vec, graph, graph_element);
            }
        }
        sorted_vec.emplace_back(graph_key);
    }
}

vector<string> topoligical_sort(const Graph& graph, class_graph_debug& print_debug)
{
    vector<string> sorted_vec;
    for (const string top_element : graph.top_elements_value())
        topological_sort_recursion(sorted_vec, graph, top_element);

    print_debug << "Topologically sorted vector : ";
    for (const auto& debug_element : sorted_vec)
    {
        print_debug << debug_element << " ";
    }
    print_debug << "\n";

    return sorted_vec;
}

