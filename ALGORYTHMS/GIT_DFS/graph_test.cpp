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

#include "graph_element.h"
#include "graph_help.h"
#include "graph_map.h"
#include "topological_sort.h"

using namespace std;

int main(int argc, char ** argv)
{
    string filename = "TEST/graph_list4.txt";
    vector<string> test1_result = { "5", "6", "7", "3", "2", "4", "1" };
    bool debug_flag = false;

    class_graph_debug debug{debug_flag};
    Graph graph{filename, debug};

    vector<string> topological_sorted_vec = topoligical_sort(graph, debug); 
    
    cout << "Debug Test Result:" << endl;
    for (auto& test_topolog_i : topological_sorted_vec)
    {
        cout << test_topolog_i << " ";
    }
    cout << endl;

    for (auto& test1_result_i : test1_result)
    {
        cout << test1_result_i << " ";
    }
    cout << endl;

    if (test1_result == topological_sorted_vec)
    {
        cout << "Test1 PASSED" << endl;
    }
    else
    {
        cout << "Test1 FAILED" << endl;
    }
}
