#pragma once

#include <cstddef>
#include <string>

class Bin_Tree_Element
{
    private:
        size_t e;
        std::string c;
        Bin_Tree_Element *u = nullptr, 
                         *l = nullptr, 
                         *r = nullptr;

    public:
        Bin_Tree_Element(const size_t& ar_el, const std::string ar_col) : e(ar_el), c(ar_col) {}
        size_t element() const { return e; }
        std::string colour() const { return c; }
        Bin_Tree_Element* up() const { return u; }
        Bin_Tree_Element* left() const { return l; }
        Bin_Tree_Element* right() const { return r; }
        size_t& element() { return e; }
        std::string& colour()  { return c; }
        Bin_Tree_Element*& up()  { return u; }
        Bin_Tree_Element*& left()  { return l; }
        Bin_Tree_Element*& right()  { return r; }
};
