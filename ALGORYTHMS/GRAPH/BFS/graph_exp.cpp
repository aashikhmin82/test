#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <math.h>
#include <string>
#include <algorithm> 
#include <sys/stat.h>
#include <queue>

#include "graph_element.h"

using namespace std;

bool if_option(int argc, char ** argv, const string & a, const string & b)
{
    char ** argv_begin = argv + 1;
    char ** argv_end = argv + argc;

    return
        find(argv_begin, argv_end, a) != argv_end or
        find(argv_begin, argv_end, b) != argv_end;
}

map<string, vector<string>> create_graph_map(bool print_debug_flag)
{
    ifstream graph_list_file("graph_list1.txt");
    string graph_string;
    map<string, vector<string>> graph_map;

    while (getline(graph_list_file, graph_string))
    {
        if (print_debug_flag)
            cout << "[D] String: " << graph_string << endl;

        stringstream graph_string_desc(graph_string);
        string graph_word;
        string graph_first_word;
        vector<string> graph_string_vec;
        graph_string_desc >> graph_first_word;

        if (print_debug_flag)
            cout << "[D] " << graph_first_word << " : ";

        graph_string_desc >> graph_word; //delete : from string
        while (graph_string_desc >> graph_word)
        {
            graph_string_vec.push_back(graph_word);
            if (print_debug_flag)
                cout << graph_word << " ";
        }
        if (print_debug_flag)
            cout << endl;

        graph_map.insert(pair<string, vector<string>>(graph_first_word,graph_string_vec));
    }
    if (print_debug_flag)
        cout << endl;

    return graph_map;
}

const void print_debug_graph(bool print_debug_flag, map<string, vector<string>>& graph_map, map<string, class_graph_element*>& graph_objects_list)
{
    if (print_debug_flag)
    {
        cout << "[D] Objec map : " << endl;
        for (const auto& i : graph_map)
        {
            cout << "[D] " << graph_objects_list[i.first]->value() << " (" << graph_objects_list[i.first]->colour() << ") : ";
            for (const auto& j : i.second)
            {
                cout << graph_objects_list[j]->value() << " (" << graph_objects_list[j]->colour() << "), ";
            }
            cout << endl;
        }
    }
}

const void print_debug_result(bool print_debug_flag, map<string, vector<string>>& graph_map, map<string, class_graph_element*>& graph_objects_list)
{
    if (print_debug_flag)
    {
        cout << "[D] PRINT RESULT : " << endl;
        cout << "[D] Objec map : " << endl;
        for (const auto& i : graph_map)
        {
            cout << "[D] " << graph_objects_list[i.first]->value() << " (" << graph_objects_list[i.first]->colour() << " " << graph_objects_list[i.first]->steps_count()<< ") : ";
            for (const auto& j : i.second)
            {
                cout << graph_objects_list[j]->value() << " (" << graph_objects_list[j]->colour() << " " << graph_objects_list[j]->steps_count() << "), ";
            }
            cout << endl;
        }
    }
}

int main(int argc, char ** argv)
{
    bool print_debug_flag = if_option(argc, argv, "-d", "--debug");
    map<string, vector<string>> graph_map = create_graph_map(print_debug_flag);

    map<string, class_graph_element*> graph_objects_list;
    for (const auto& i : graph_map)
    {
        class_graph_element graph_object_element(i.first);
        graph_objects_list.insert(pair<string, class_graph_element*>(i.first, new class_graph_element(i.first)));
        cout << i.first << " : ";//"s : " << i.second << "\n";
    }

    for (const auto& i : graph_objects_list)
    {
        cout << "[Obj] " << i.first << " : " << i.second->value() << "  (" << graph_objects_list[i.first]->value() << ")" << endl;
    }

    print_debug_graph(print_debug_flag, graph_map, graph_objects_list);

    queue<string> objects_queue;
    objects_queue.push(graph_objects_list["1"]->value());
    while (!objects_queue.empty())
    {
        string queue_element = objects_queue.front();
        if (print_debug_flag)
            cout << "[D] Queue Element : " << queue_element << endl;
        graph_objects_list[queue_element]->colour() = "black";
        cout << "[D] Graph Map Val : " << graph_objects_list[queue_element]->value() << endl;
        objects_queue.pop();
        for (const auto& j : graph_map[queue_element])
        {
            size_t count = graph_objects_list[queue_element]->steps_count();
            ++count;
            if (graph_objects_list[j]->colour() == "white")
            {
                if (print_debug_flag)
                    cout << "[D] Pushed to the queue : " << j << endl;
                objects_queue.push(j);
                graph_objects_list[j]->colour() = "black";
                graph_objects_list[j]->steps_count() = count;
            }
        }
    }

    print_debug_result(print_debug_flag, graph_map, graph_objects_list);
}
