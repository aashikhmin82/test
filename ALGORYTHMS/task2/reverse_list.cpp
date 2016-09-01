#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

#include "randoms_list.h"
#include "node.h"

using namespace std;
using number_t = size_t;

template<typename first_object_t, typename rnd_list_t>
void create_list(first_object_t& first_element, const rnd_list_t& rnd_list)
{
    auto current_element = first_element;
    for (size_t i = 1; i < rnd_list.size(); ++i)
    {
        auto list_element (make_shared<node <number_t>>(rnd_list[i]));
        current_element->next() = list_element;
        current_element = list_element;
    }
}

template <typename first_object_t>
void print_list(first_object_t& first_element)
{
    cout << "[Print_list] Check List : " << flush;
    auto element = first_element;
    cout << first_element->get_value() << " " << flush;
    while (element->next())
    {
        element = element->next();
        cout << element->get_value() << " " << flush;
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    cout << endl;
}

template <typename first_object_t>
first_object_t reverse_v1(first_object_t& first_element)
{
    auto priv_element = first_element;
    auto current = first_element->next();
    auto next_element = first_element->next()->next();

    priv_element->next() = nullptr;

    while (next_element->next())
    {
        current->next() = priv_element;
        priv_element = current;
        current = next_element;
        next_element = current->next();
    }

    current->next() = priv_element;
    next_element->next() = current;

    return next_element;
}

template <typename first_object_t>
first_object_t reverse_v2(first_object_t& first_element)
{
    auto current = first_element;
    auto next = first_element->next();
    auto new_first_element = reverse_two_nodes(current, next);

    first_element->next() = nullptr;

    return new_first_element;
}

template <typename object_t>
object_t reverse_two_nodes(object_t& current_element, object_t& next_element)
{
    auto new_first_element = next_element;
    if (next_element->next())
        new_first_element = reverse_two_nodes(next_element, next_element->next());

    next_element->next() = current_element;
    return new_first_element;
}

int main()
{
    number_t min_value { 1 };
    number_t max_value { 500 };
    size_t list_size { 15 };

    randoms_list<number_t> rnd_list(min_value, max_value, list_size);
    auto numbers_list = rnd_list.get();

    auto first_element_v1(make_shared<node<number_t>>(numbers_list[0]));
    auto first_element_v2(make_shared<node<number_t>>(numbers_list[0]));

    create_list(first_element_v1, numbers_list);
    create_list(first_element_v2, numbers_list);
    print_list(first_element_v1);

    auto new_first_v1 = reverse_v1(first_element_v1);
    cout << "Reverse V1 : ";
    print_list(new_first_v1);

    auto new_first_v2 = reverse_v2(first_element_v2);
    cout << "Reverse V2 : ";
    print_list(new_first_v2);
}
