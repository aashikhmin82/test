#include "boost/program_options.hpp"
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

#include "graph_help.h"
#include "graph_map.h"
#include "kruskal_alg.h"
#include "prims_alg.h"

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
    const vector<vertex_t> test_kruskal_result1 { "h", "g", "f", "g", "f", "c", "i", "c", "d", "c", "h", "a", "b", "a", "e", "d" };
    const vector<vertex_t> test_prims_result1 { "i", "c", "c", "f", "f", "g", "g", "h", "c", "d", "c", "b", "b", "a", "d", "e" };

    int exit_code = 0;

    bool debug_flag = false;
    graph_debug debug(debug_flag);

    Graph graph{input_graph1, debug};

    auto min_stp_test1 = kruskal_arg(graph, debug);
    if (test_kruskal_result1 == min_stp_test1)
        cout << "Test1 Passed" << endl;
    else
    {
        cout << "Test1 Failed" << endl;
        exit_code = 1;
    }

    auto min_stp_test2 = prims_alg(graph, debug);
    if (test_prims_result1 == min_stp_test2)
        cout << "Test2 Passed" << endl;
    else
    {
        cout << "Test2 Failed" << endl;
        exit_code = 1;
    }

    return exit_code;
}
