#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>
#include <memory>

#include "bin_tree.h"

using namespace dot;
using namespace std;

int main(int argc, char* arg_vec[])
{
    size_t size = 0;
    bool rbtree = 0;
    bool debugf = 0;
    size_t arg_i = 1;
    string first_arg;
    vector<size_t> input_config;

    while (argc > 1)
    {
        istringstream in(arg_vec[arg_i]);
        in >> first_arg;

        if (first_arg == "--help")
        {
            cout << "HELP: " << endl;
            cout << "\t--size [value]" << endl;
            cout << "\t--config [filename]" << endl;
            return 0;
        }
        else if (first_arg == "--size")
        {
            istringstream in(arg_vec[2]);
            in >> size;
            argc -= 2;
            arg_i += 2;
        }
        else if (first_arg == "--rb")
        {
            rbtree = 1;
            --argc;
            ++arg_i;
        }
        else if (first_arg == "--debug")
        {
            debugf = 1;
            --argc;
            ++arg_i;
        }
        else if (first_arg == "--config")
        {
            string file_name;
            size_t str_el;
            istringstream in(arg_vec[2]);
            in >> file_name;

            ifstream config_fl(file_name);
            while (config_fl >> str_el)
            {
                input_config.push_back(str_el);
            }
            config_fl.close();
            argc -= 2;
            arg_i += 2;
        }
    }
    
    std::cout << "Binary Tree" << std::endl;
    std::shared_ptr<Tree_Output> tree_output_func (new Tree_Output);
    std::shared_ptr<Tree_Output> tree_output_func1 (new Tree_Output);
    *tree_output_func = *tree_output_func1;

    srand( static_cast<size_t>(time(nullptr)) );

    if ((size == 0) and (input_config.size() == 0))
    {
        std::cout << "Enter the Size : ";
        std::cin >> size;
    }

    if (size == 0)
    {
        size = input_config.size();
    }

    std::vector<size_t> test_array(size);
    if (input_config.size() == 0)
    {
        for (auto& test_element : test_array) {
            test_element = nrand(1000);
        }
    }
    else
    {
        test_array = input_config;
    }

    size_t element_number = 1;
    std::shared_ptr<Bin_Tree_Element> first;

    std::cout << "Test array : ";
    for (auto& test_element : test_array) {
        std::cout << test_element << " ";
    }
    std::cout << std::endl;

    std::cout << "Test array : ";
    element_number = 1;
    for (size_t i = 0; i < test_array.size(); ++i) 
    {
        highlight_t highlight;
        std::shared_ptr<Bin_Tree_Element> tree_element (new Bin_Tree_Element(test_array[i], "red"));

        if (i == 0)
        {
            string label = "Creating tree from scratch.";
            dump_tree(nullptr, highlight, "creation", label);
            first = tree_element;
            first->colour() = "black";
            tree_output_func->create_tree_to_output(0, element_number, first);
        }
        else
        {
            string dummy;
            insert_element(first, tree_element, dummy, highlight, tree_output_func, rbtree);
        }
        check_rb(first, debugf);

        ostringstream out;
        out << "Creating tree, added value " << test_array[i] << '.';
        dump_tree(first, highlight, "creation", out.str());
    }
    std::cout << std::endl;

    tree_output_func->print_tree();

    highlight_t highlight;

    std::map<std::string, std::function<void()>> funcs;
    funcs["help"] = []() {  help(); };
    funcs["add_value"] = [&first, &highlight, &tree_output_func, &rbtree, &debugf]()
    {
        string label;
        add_value(first, label, highlight, tree_output_func, rbtree);
        check_rb(first, debugf);
        dump_tree(first, highlight, "tree", label);
    };
    funcs["print_tree"] = [&tree_output_func]() { print_tree_f(tree_output_func); };
    funcs["dump"] = [&first, &highlight]() { dump_tree(first, highlight, "tree", "Dump, difference from previous dump is highlighted."); };
    funcs["min_value"] = [&first]() { select_min_value(first); };
    funcs["max_value"] = [&first]() { select_max_value(first); };
    funcs["rotate"] = [&first, &tree_output_func]() { search_and_rotate(first, tree_output_func); };
    funcs["search_value"] = [&first, &highlight]()
    {
        string label;
        tree_search(first, label, highlight);
        dump_tree(first, highlight, "tree", label);
    };
    funcs["delete_value"] = [&first, &highlight, &tree_output_func]()
    {
        string label;
        delete_element(first, label, highlight, tree_output_func, first);
        dump_tree(first, highlight, "tree", label);
    };
    funcs["save_config"] = [&first]() { save_config(first); };
    funcs["exit"] = []() { exit(1); };

    string f_name;
    while (1) 
    {
        cout << "Please enter func_name or help : ";
        cin >> f_name;

        try 
        {
            funcs[f_name]();
        }
        catch(...)
        {
            cerr << "Incorrect value!" << endl;
        }
    }
}
