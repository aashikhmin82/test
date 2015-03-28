#pragma once

#include <cstddef>
#include <string>

class Bin_Tree_Element
{
    public:
        size_t element;
        std::string colour;
        Bin_Tree_Element *up = nullptr, 
                         *left = nullptr, 
                         *right = nullptr;

        Bin_Tree_Element(const size_t& ar_el, const std::string ar_col) : element(ar_el), colour(ar_col) {}
};
