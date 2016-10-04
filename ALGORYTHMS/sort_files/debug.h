#pragma once

#include <iostream>

class print_debug
{
    private:
        bool debug_flag;

    public:
    explicit print_debug(const bool print_debug_flag) : debug_flag(print_debug_flag)
    {
        std::cout << (debug_flag ? "DEBUG ON" : "DEBUG OFF") << std::endl;
    }

    template <typename T> print_debug& operator<<(T& text)
    {
        if (debug_flag)
            std::cout << text;

        return *this;
    }
};
