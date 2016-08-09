#pragma once

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

using namespace std;

vector<vertex_t> kruskal_arg(const Graph& graph, graph_debug& debug);
