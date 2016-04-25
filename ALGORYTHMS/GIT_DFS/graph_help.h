#include <cstddef>
#include <string>
#include <memory>
#include <iostream>
#include <map>
#include <vector>

#include "graph_element.h"

using namespace std;

class class_graph_debug
{
    private:
        bool debug_flag;

    public:
        class_graph_debug(bool print_debug_flag) : debug_flag(print_debug_flag)
    {
        if (print_debug_flag)
            cout << "DEBUG ON" << endl;
        else
            cout << "DEBUG OFF" << endl;
    }

    template <typename Type> void print(string text, Type value)
    {
            if (debug_flag)
                        cout << "[Debug1] " << text << " : " << value << endl;
    }

//    template <typename Type> void operator<<(string text)
//    void operator<<(string text)
//    class_graph_debug  operator<<(string text)
    template <typename T> class_graph_debug  operator<<(T& text)
    {
        if (debug_flag)
                    cout << text;

        return *this;
    }

    const void print_debug_graph(map<string, vector<string> >& graph_map, map<string, shared_ptr<class_graph_element>>& graph_objects_list)
    {
        if (debug_flag)
        {
            cout << "[D]<print_debug_graph> Objec map : " << endl;
            for (const auto& i : graph_map)
            {
                cout << "[D]<print_debug_graph> " << graph_objects_list[i.first]->value() << " (" << graph_objects_list[i.first]->colour() << ") : ";
                for (const auto& j : i.second)
                {
                    cout << "<J: " << j << "> ";
                    cout << graph_objects_list[j]->value() << " (" << graph_objects_list[j]->colour() << "), ";
                    cout << "\tSteps : " << graph_objects_list[j]->steps_count_discover() << " (" << graph_objects_list[j]->steps_count_finish() << "), ";
                    cout << "nx ";
                }
                cout << endl;
            }
        }
    }

    const void print_debug_result(map<string, vector<string> >& graph_map, map<string, class_graph_element*>& graph_objects_list)
    {
        if (debug_flag)
        {
            cout << "[D]<print_debug_result> PRINT RESULT : " << endl;
            cout << "[D]<print_debug_result> Objec map : " << endl;
            for (const auto& i : graph_map)
            {
                cout << "[D]<print_debug_result> " << graph_objects_list[i.first]->value() << " (" << graph_objects_list[i.first]->colour() << " " << graph_objects_list[i.first]->steps_count_discover()<< ") : ";
                for (const auto& j : i.second)
                {
                    cout << graph_objects_list[j]->value() << " (" << graph_objects_list[j]->colour() << " " << graph_objects_list[j]->steps_count_discover() << "), ";
                }
                cout << endl;
            }
        }
    }
};
