#pragma once

#include <vector>
#include <iostream>

template <typename data_t>
class mystack
{
    private:
    std::vector <data_t> stack_data;

    public:
    void push_back(const data_t& data_string)
    {
        stack_data.emplace_back(data_string);
    }

    decltype(auto) size()
    {
        return stack_data.size();
    }

    bool empty()
    {
        return stack_data.empty();
    }

    data_t pop_back()
    {
        auto last_element = stack_data.back();
        stack_data.pop_back();

        return last_element;
    }
};
