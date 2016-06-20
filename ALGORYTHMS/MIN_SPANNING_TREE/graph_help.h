#pragma once

#include <cstddef>
#include <string>
#include <memory>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class graph_debug
{
    private:
        bool debug_flag;

    public:
        explicit graph_debug(bool print_debug_flag) : debug_flag(print_debug_flag)
    {
        if (print_debug_flag)
            cout << "DEBUG ON" << endl;
        else
            cout << "DEBUG OFF" << endl;
    }

    template <typename T> graph_debug  operator<<(T& text)
    {
        if (debug_flag)
                    cout << text;

        return *this;
    }
};
