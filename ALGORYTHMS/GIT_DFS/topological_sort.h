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

using namespace std;

//void topological_sort_recursion(vector<string>& sorted_vec, const Graph& graph, const string& graph_key);
vector<string> topoligical_sort(const Graph& graph, class_graph_debug& print_debug);
