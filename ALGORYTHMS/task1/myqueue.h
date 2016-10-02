#pragma once

#include <vector>
#include <iostream>

#include "stack.h"

template <typename data_t>
class myqueue
{
    private:
        mystack <data_t> stack_in, stack_out;

    public:
        void push_back(const data_t& data_string)
        {
            stack_in.push_back(data_string);
        }

        data_t pop_front()
        {
            if (stack_out.empty())
                while (!stack_in.empty())
                    stack_out.push_back(stack_in.pop_back());

            return stack_out.pop_back();
        }

        bool empty()
        {
            if (stack_in.empty() and stack_out.empty())
                return true;

            return false;
        }
};
