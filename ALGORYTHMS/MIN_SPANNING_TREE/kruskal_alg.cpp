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

using edge_weight_map = map<size_t, vector<string>>;

void check_safe_edges(const string& vertex, map<size_t, vector<string>>& safe_edges, vector<string>& min_stp)
{
    size_t count_matches { 0 };
    for (auto& safe_edges_i : safe_edges)
    {
        if (find((safe_edges_i.second).begin(), (safe_edges_i.second).end(),vertex) != (safe_edges_i.second).end())
        {
            for(auto& safe_vertex : safe_edges_i.second)
                min_stp.emplace_back(safe_vertex);

            safe_edges.erase(safe_edges_i.first);
            ++count_matches;
        }
    }
    assert(count_matches < 2);
}

void add_to_safe_edges(const string& vertex1, const string& vertex2, map<size_t, vector<string>>& safe_edges)
{
    size_t add_to_index { 0 };
    size_t matched_count { 0 };
    if (safe_edges.empty())
    {
        safe_edges.insert(pair<size_t, vector<string>>(add_to_index + 1,{vertex1, vertex2}));
    }
    else
    {
        for (auto& safe_edges_i : safe_edges)
        {
            if (find((safe_edges_i.second).begin(), (safe_edges_i.second).end(),vertex1) != (safe_edges_i.second).end() or
                find((safe_edges_i.second).begin(), (safe_edges_i.second).end(),vertex2) != (safe_edges_i.second).end())
            {
                if (matched_count == 0)
                {
                    (safe_edges_i.second).emplace_back(vertex1);
                    (safe_edges_i.second).emplace_back(vertex2);
                }
                else
                {
                    for(auto& edge_i : safe_edges_i.second)
                        (safe_edges_i.second).emplace_back(edge_i);

                    safe_edges.erase(safe_edges_i.first);
                }
                ++matched_count;
            }
            add_to_index = safe_edges_i.first;
        }
        assert(matched_count <= 2);

        if (matched_count == 0)
            safe_edges.insert(pair<size_t, vector<string>>(add_to_index + 1,{vertex1, vertex2}));
    }
}

void check_and_add_vertex (const string& vertex1, const string& vertex2, vector<string>& min_stp, vector<string>& uniq_edges, map<size_t, vector<string>>& safe_edges)
{
    if (find(uniq_edges.begin(), uniq_edges.end(), vertex2+vertex1) == uniq_edges.end())
    {
        uniq_edges.emplace_back(vertex1+vertex2);
        auto count_vertex1 = count(min_stp.cbegin(), min_stp.cend(), vertex1);
        auto count_vertex2 = count(min_stp.cbegin(), min_stp.cend(), vertex2);

        if (count_vertex1 == 0 and count_vertex2 == 0)
        {
            if (min_stp.empty())
            {
                min_stp.emplace_back(vertex1);
                min_stp.emplace_back(vertex2);
            }
            else
            {
                add_to_safe_edges(vertex1, vertex2, safe_edges);
            }

        }
        else if (count_vertex1 == 0 or count_vertex2 == 0)
        {
            min_stp.emplace_back(vertex1);
            min_stp.emplace_back(vertex2);

            if (count_vertex1 == 0)
                check_safe_edges(vertex1,safe_edges, min_stp);
            else
                check_safe_edges(vertex2,safe_edges, min_stp);
        }
    }
}

vector<string> select_min_stp(edge_weight_map&& sorted_edges, graph_debug& debug)
{
    vector<string> uniq_edges;
    vector<string> min_stp;
    map<size_t, vector<string>> safe_edges;
    for (const auto& edge_weight : sorted_edges)
    {
        debug << edge_weight.first << " : " << "\n\t";
        size_t i = 0;
        string vertex1 = "", vertex2 = "";
        for (const auto& edge_i : edge_weight.second)
        {
            ++i;
            if (i == 3)
            {
                i = 1;
                debug << " => " << vertex1 << "  " << vertex2 << "\n\t";
                check_and_add_vertex(vertex1, vertex2, min_stp, uniq_edges, safe_edges);
            }
            (i == 1) ? vertex1 = edge_i : vertex2 = edge_i;
            debug << edge_i << " ";
        }
        check_and_add_vertex(vertex1, vertex2, min_stp, uniq_edges, safe_edges);
        debug << "\n";
    }

    //Safe edges has to be empty
    assert(safe_edges.empty());

    return min_stp;
}

vector<string> kruskal_arg(const Graph& graph, graph_debug& debug)
{
    edge_weight_map sorted_edges;
    debug << "Test kruskal arg";
    debug << "\n";

    auto graph_map = graph.graph_map_value1();

    debug << "Vertex : ";
    for (const auto& vert_i : graph_map)
    {
        debug << vert_i.first << " : ";
        for (const auto& edge_i : vert_i.second)
        {
            debug << edge_i.first << " (" << edge_i.second << ")" << "  ";
            sorted_edges[edge_i.second].emplace_back(vert_i.first);
            sorted_edges[edge_i.second].emplace_back(edge_i.first);
            debug << "Insert new Edge to the map\n";
        }
        debug << "\n";
    }
    debug << "\n";
    vector<string> min_stp = select_min_stp(move(sorted_edges), debug);

    return min_stp;
}
