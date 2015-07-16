#pragma once

#include <cstddef>
#include <string>
#include <memory>

enum node_colour {red, black, none};

class Bin_Tree_Element
{
    private:
        size_t e;
        node_colour c;
        std::weak_ptr<Bin_Tree_Element> u;
        std::shared_ptr<Bin_Tree_Element> l = nullptr,
//                         l = nullptr,
                         r = nullptr;

        mutable size_t ec = 0;
        mutable size_t lc = 0;
        mutable size_t rc = 0;

    public:
        Bin_Tree_Element(const size_t& ar_el, node_colour ar_col) : e(ar_el), c(ar_col) {}

        size_t element(bool update_counter = true) const
        {
            if (update_counter)
                ++ec;
            return e;
        }
        node_colour colour(bool update_counter = true) const { return c; }
//        const std::weak_ptr<Bin_Tree_Element> up(bool update_counter = true) const
        const std::shared_ptr<Bin_Tree_Element> up(bool update_counter = true) const
        {
//            return u;
            return u.lock();
//            return u.get();
        }
        const std::shared_ptr<Bin_Tree_Element> left(bool update_counter = true) const
        {
            if (update_counter)
                ++lc;
            return l;
        }
        const std::shared_ptr<Bin_Tree_Element> right(bool update_counter = true) const
        {
            if (update_counter)
                ++rc;
            return r;
        }

        size_t& element(bool update_counter = true)
        {
            if (update_counter)
                ++ec;
            return e;
        }
//        std::string& colour(bool update_counter = true)  { return c; }
        node_colour& colour(bool update_counter = true)  { return c; }
        std::shared_ptr<Bin_Tree_Element> up(bool update_counter = true)
//        std::weak_ptr<Bin_Tree_Element>& up(bool update_counter = true)
        {
//            return u;
//            std::shared_ptr<Bin_Tree_Element> u1; 
//            u1 = u.lock();
//            return u.lock();
            return u.lock();
        }
        std::weak_ptr<Bin_Tree_Element>& weak_up(bool update_counter = true)
        {
            return u;
//            std::shared_ptr<Bin_Tree_Element> u1; 
//            u1 = u.lock();
//            return u.lock();
//            return u.lock();
        }
        std::shared_ptr<Bin_Tree_Element>& left(bool update_counter = true)
        {
            if (update_counter)
                ++lc;
            return l;
        }
        std::shared_ptr<Bin_Tree_Element>& right(bool update_counter = true)
        {
            if (update_counter)
                ++rc;
            return r;
        }

        size_t element_counter() const { return ec; }
        size_t left_counter() const { return lc; }
        size_t right_counter() const { return rc; }
};
