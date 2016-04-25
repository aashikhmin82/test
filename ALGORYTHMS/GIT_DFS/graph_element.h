#pragma once

#include <cstddef>
#include <string>
#include <memory>
#include <iostream>

using namespace std;

class class_graph_element
{
    private:
        string element_value;
        string element_colour;
        size_t element_count_discover;
        size_t element_count_finish;
        
//        class_graph_element *next_element = NULL;
//        class_graph_element *priv_element = NULL;
        std::shared_ptr<class_graph_element> next_element = nullptr,
                                            priv_element = nullptr;

    public:
        class_graph_element(string graph_element_value) : element_value(graph_element_value)
        {
            std::cout << "[DEBUG] Create CLASS" << std::endl;
            element_colour = "white";
            element_count_discover = 0;
            element_count_finish = 0;
        }

        const string value() const
        {
            return element_value;
        }

        string& value()
        {
            return element_value;
        }

        const size_t steps_count_discover() const
        {
            return element_count_discover;
        }

        size_t& steps_count_discover()
        {
            return element_count_discover;
        }

        const size_t steps_count_finish() const
        {
            return element_count_finish;
        }

        size_t& steps_count_finish()
        {
            return element_count_finish;
        }

        const string colour() const
        {
            return element_colour;
        }

        string& colour()
        {
            return element_colour;
        }

        shared_ptr<class_graph_element>& next()
        {
            return next_element;
        }

        const shared_ptr<class_graph_element> next() const
        {
            return next_element;
        }

        shared_ptr<class_graph_element>& priv()
        {
            return priv_element;
        }

        const shared_ptr<class_graph_element> priv() const
        {
            return priv_element;
        }
};
