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
        size_t element_count;
        
        class_graph_element *next_element = NULL;

    public:
        class_graph_element(string graph_element_value) : element_value(graph_element_value)
        {
            std::cout << "[DEBUG] Create CLASS" << std::endl;
            element_colour = "white";
            element_count = 0;
        }

        const string value() const
        {
            return element_value;
        }

        string& value()
        {
            return element_value;
        }

        const size_t steps_count() const
        {
            return element_count;
        }

        size_t& steps_count()
        {
            return element_count;
        }

        const string colour() const
        {
            return element_colour;
        }

        string& colour()
        {
            return element_colour;
        }

        class_graph_element& next()
        {
            return *next_element;
        }

        const class_graph_element next() const
        {
            return *next_element;
        }
};
