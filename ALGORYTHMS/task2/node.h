#pragma once

#include <iostream>
#include <memory>

template <typename data_t>
class node
{
    private:
    data_t node_data;
    std::shared_ptr <node<data_t>> next_el = nullptr;

    public:
    explicit node (const data_t& data) : node_data(data) {}

    data_t& get_value()
    {
        return node_data;
    }

    std::shared_ptr <node<data_t>>& next()
    {
        return next_el;
    }
};
