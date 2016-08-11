#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class graph_debug
{
    private:
        bool debug_flag;

    public:
    explicit graph_debug(bool print_debug_flag) : debug_flag(print_debug_flag)
    {
        debug_flag ? std::cout << "DEBUG ON" << std::endl : std::cout << "DEBUG OFF" << std::endl;
    }

    template <typename T> graph_debug&  operator<<(T& text)
    {
        if (debug_flag)
            std::cout << text;

        return *this;
    }
};
