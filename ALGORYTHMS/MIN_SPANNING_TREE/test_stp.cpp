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
    const vector<vector<string>> input_graph1 { 
        { "a", "b(4)", "h(8)" }, 
        { "b", "a(4)", "c(8)", "h(11)" },
        { "c", "b(8)", "d(7)", "f(4)", "i(2)" },
        { "d", "c(7)", "e(9)", "f(14)" },
        { "e", "d(9)", "f(10)" },
        { "f", "c(4)", "d(14)", "e(10)", "g(2)" },
        { "g", "f(2)", "h(1)", "i(6)" },
        { "h", "a(8)", "g(1)", "i(7)" },
        { "i", "c(2)", "g(6)", "h(7)" } 
    };
    const vector<vertex_t> test_result1 { "h", "g", "f", "g", "f", "c", "i", "c", "d", "c", "h", "a", "b", "a", "e", "d" };

    int exit_code = 0;

    bool debug_flag = false;
    graph_debug debug(debug_flag);

    Graph graph{input_graph1, debug};

    auto min_stp = kruskal_arg(graph, debug);
    
    if (test_result1 == min_stp)
        cout << "Test1 Passed" << endl;
    else
    {
        cout << "Test1 Failed" << endl;
        exit_code = 1;
    }

    cout << "STP : ";
    for (const auto& stp_i : min_stp)
        cout << stp_i << " ";
    cout << endl;

    return exit_code;
}
