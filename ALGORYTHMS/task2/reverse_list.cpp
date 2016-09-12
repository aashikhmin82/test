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
void print_list(const first_object_t& first_element)
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
    if (!first_element)
        return first_element;

    auto prev_el = first_element;
    auto current_el = first_element;

    while (current_el)
    {
        auto next_el = current_el->next();
        current_el->next() = prev_el;
        prev_el = current_el;
        current_el = next_el;
    }

    first_element->next() = nullptr;

    return prev_el;
}

template <typename first_object_t>
first_object_t reverse_v2(first_object_t& first_element)
{
    if (!first_element or !first_element->next())
        return first_element;

    auto current = first_element;
    auto new_first_element = reverse_two_nodes(current, current->next());

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

template <typename first_object_t>
vector<number_t> dump_to_vector(const first_object_t& first_element)
{
    auto element = first_element;
    vector<number_t> dump_list;
    while (element)
    {
        dump_list.push_back(element->get_value());
        element = element->next();
    }

    return dump_list;
}

template <typename obj_t>
bool check_reversed(const obj_t& init_first, const obj_t& reversed_first)
{
    vector<number_t> init_list = dump_to_vector(init_first);
    vector<number_t> reversed_init_list = dump_to_vector(reversed_first);

    if (init_list.size() != reversed_init_list.size())
        return false;

    size_t i { init_list.size() };
    for (const auto& init_el : init_list)
    {
        --i;
        if (init_el != reversed_init_list[i])
        {
            return false;
        }
    }

    return true;
}

int main()
{
    number_t min_value { 1 };
    number_t max_value { 500 };
//    size_t list_size { 15 };
//    vector<size_t> list_sizes { 1, 2, 4, 6 };

    for (size_t list_size = 1; list_size < 100; list_size *= 2)
    {
        auto numbers_list = randoms_list(min_value, max_value, list_size);

        auto first_element_v1(make_shared<node<number_t>>(numbers_list[0]));
        create_list(first_element_v1, numbers_list);
        auto new_first_v1 = reverse_v1(first_element_v1);

        //CHECK reverse_v1 begin
        auto first_el(make_shared<node<number_t>>(numbers_list[0]));
        create_list(first_el, numbers_list);
        cout << "Test reverse_v1 (size : " << list_size << ") : " <<
            (check_reversed(first_el, new_first_v1) ? "Passed" : "Failed") << endl;
        //end;

        auto first_element_v2(make_shared<node<number_t>>(numbers_list[0]));
        create_list(first_element_v2, numbers_list);
        auto new_first_v2 = reverse_v2(first_element_v2);

        //CHECK reverse_v2 begin
        cout << "Test reverse_v2 (size : " << list_size << ") : " <<
            (check_reversed(first_el, new_first_v2) ? "Passed" : "Failed") << endl;
        //end;
    }

    //CHECK Nullptr begin
    cout << "Func 1 -> Test nullptr : ";
    shared_ptr<node<number_t>> first_element_v1 = nullptr;
    auto new_first_v1 = reverse_v1(first_element_v1);
    cout << ( !new_first_v1 ? "Passed" : "Failed" )<< endl;

    cout << "Func 2 -> Test nullptr : ";
    shared_ptr<node<number_t>> first_element_v2 = nullptr;
    auto new_first_v2 = reverse_v2(first_element_v2);
    cout << ( !new_first_v2 ? "Passed" : "Failed" ) << endl;
    //End
}
