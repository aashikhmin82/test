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
#include "boost/program_options.hpp"

#include "graph_help.h"
#include "graph_map.h"
#include "kruskal_alg.h"

using namespace std;

int main(int argc, char ** argv)
{
    string filename = "graph_list1.txt";
    const vector<string> test_result1 { "h", "g", "f", "g", "f", "c", "i", "c", "d", "c", "h", "a", "b", "a", "e", "d" };

    bool debug_flag = false;

    graph_debug debug(debug_flag);
    Graph graph(filename, debug);

    auto min_stp = kruskal_arg(graph, debug);
    
    if (test_result1 == min_stp)
        cout << "Test1 Passed" << endl;
    else
        cout << "Test1 Failed" << endl;

    cout << "STP : ";
    for (const auto& stp_i : min_stp)
        cout << stp_i << " ";
    cout << endl;
}
