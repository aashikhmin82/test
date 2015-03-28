#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>

#include "bin_tree.h"
#include "dot.h"

using namespace dot;
using namespace std;

class Tree_Output 
{
    private:
        std::vector< std::vector<Bin_Tree_Element *> > bin_tree_struct;
    
    public:
        void create_tree_to_output(const size_t level, size_t element_num, Bin_Tree_Element& obj_element)
        {
            if (bin_tree_struct.size() < level + 1) 
            {
                vector<Bin_Tree_Element *> row(pow(2,level),nullptr);
                bin_tree_struct.push_back(row);
            }

            size_t element_array_num = 0;
            if (element_num != 0)
                element_array_num = element_num - pow(2, level);

            bin_tree_struct[level][element_array_num] = &obj_element;
        }

        void delete_old_tree()
        {
            bin_tree_struct.clear();
        }

        void print_tree()
        {
            size_t i = 0;
            size_t spaces_amount = (pow(2,bin_tree_struct.size() - 1) * 6 + pow(2,bin_tree_struct.size() - 1)) / 2;

            std::vector< std::vector<Bin_Tree_Element *> >::iterator matrix_iter = bin_tree_struct.begin();
            while (matrix_iter != bin_tree_struct.end()) 
            {
                string spaces(spaces_amount, ' ');
                cout << i << " : " << spaces;
                spaces_amount = spaces_amount - (5 * pow(2,i) / 2);
                ++i;
                std::vector<Bin_Tree_Element *>::iterator row_iter = (*matrix_iter).begin();   
                while (row_iter != (*matrix_iter).end()) 
                {
                    if (*row_iter) 
                        cout << (*row_iter)->element << "(" << (*row_iter)->colour << ") ";
                    else 
                        cout << "NULL ";

                    ++row_iter;
                }
                cout << endl;
                ++matrix_iter;
            }
        }
};

void insert_element(Bin_Tree_Element *root, Bin_Tree_Element *bin_tree_element, string& label, highlight_t& highlight, Tree_Output *tree_func, size_t level = 1, size_t element_num = 1)
{
    highlight.node(root, visited);

    if (root->element > bin_tree_element->element) 
    {
        if (!root->left) 
        {
            element_num = element_num * 2;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            root->left = bin_tree_element;
            bin_tree_element->up = root;
            highlight.edge(root, left_edge, added);
            highlight.leaf(bin_tree_element, added);
        } 
        else 
        {
            ++level;
            element_num = element_num * 2;
            insert_element(root->left, bin_tree_element, label, highlight, tree_func, level, element_num);
            highlight.edge(root, left_edge, visited);
        }
    } 
    else 
    {
        if (!root->right) 
        {
            element_num = element_num * 2 + 1;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            root->right = bin_tree_element;
            bin_tree_element->up = root;
            highlight.edge(root, right_edge, added);
            highlight.leaf(bin_tree_element, added);
        } 
        else 
        {
            ++level;
            element_num = element_num * 2 + 1;
            insert_element(root->right, bin_tree_element, label, highlight, tree_func, level, element_num);
            highlight.edge(root, right_edge, visited);
        }
    }
}

void recreate_tree_f(Bin_Tree_Element *tree_el, Tree_Output *tree_func, size_t level = 0, size_t element_num = 0)
{
    size_t level_tmp = level;
    size_t element_num_tmp = element_num;
    if (level == 0)
    {
        tree_func->delete_old_tree();
        element_num = 1;
        element_num_tmp = element_num;
    }

    tree_func->create_tree_to_output(level, element_num, *tree_el);

    if (tree_el->left) 
    {
        ++level;
        element_num = element_num * 2;
        recreate_tree_f(tree_el->left, tree_func, level, element_num);
    }
    else 
        element_num = element_num * 2;

    level = level_tmp;
    element_num = element_num_tmp;

    if (tree_el->right) 
    {
        ++level;
        element_num = element_num * 2 + 1;
        recreate_tree_f(tree_el->right, tree_func, level, element_num);
    }
    else 
        element_num = element_num * 2 + 1;
}

void add_value (Bin_Tree_Element *first, string& label, highlight_t& highlight, Tree_Output *tree_func, size_t value_to_add = 0)
{
    cout << "Value to add : ";
    cin >> value_to_add;

    ostringstream out;
    out << "Command: 'add " << value_to_add << "'. ";
    label = out.str();

    Bin_Tree_Element *tree_element = new Bin_Tree_Element(value_to_add, "NON");
    insert_element(first, tree_element, label, highlight, tree_func);
}

void print_tree_f(Tree_Output *tree_func)
{
    tree_func->print_tree();
}

void help()
{
    cout << "You can enter :\n" 
        << "\thelp : Output this message.\n"
        << "\tadd_value [value]: Enter the size_t value to the tree.\n"
        << "\tsearch_value [value] : Search value.\n"
        << "\tdelete_value [value]: Delete value.\n"
        << "\tprint_tree : Output the Tree.\n" 
        << "\tdump : Dump the Tree.\n" 
        << "\tmin_value : Print Min value.\n"
        << "\tmax_value : Print Max value.\n"
        << "\tsave_config [filename] : Save config.\n"
        << "\texit : to exit" << endl;
}

void select_max_value(const Bin_Tree_Element *root)
{
    if (root->right)
        select_max_value(root->right);
    else
        cout << "MAX Value : " << root->element << endl;
}

void select_min_value(const Bin_Tree_Element *root)
{
    if (root->left)
        select_min_value(root->left);
    else
        cout << "MIN Value : " << root->element << endl;
}

void tree_search(const Bin_Tree_Element *root, string& label, highlight_t& highlight, size_t search_el = 0)
{
    if (search_el == 0)
    {
        cout << "Enter the value to search : ";
        cin >> search_el;

        ostringstream out;
        out << "Command: 'search " << search_el << "'. ";
        label = out.str();
    }

    ostringstream out;

    if (!root)
    {
        out << "Not found!";
        cout << out.str() << endl;
        label += out.str();
    }
    else if (root->element == search_el)
    {
        highlight.node(root, found);
        out << "Found : " << root->element << "\t Colour : " << root->colour;
        cout << out.str() << endl;
        label += out.str();
    }
    else
    {
        highlight.node(root, visited);
        if (root->element > search_el)
        {
            highlight.edge(root, left_edge, root->left ? visited : not_found);
            highlight.node(nodeid_t(nullptr, root, left_edge), not_found);
            tree_search(root->left, label, highlight, search_el);
        }
        else
        {
            highlight.edge(root, right_edge, root->right ? visited : not_found);
            highlight.node(nodeid_t(nullptr, root, right_edge), not_found);
            tree_search(root->right, label, highlight, search_el);
        }
    }
}

void delete_replace(Bin_Tree_Element *del_el, Bin_Tree_Element *replace_el, Bin_Tree_Element *root)
{
    if (del_el->up)
    {
        if ((del_el->up->left) and (del_el->up->left->element == del_el->element))
            del_el->up->left = replace_el;
        else
            del_el->up->right = replace_el;
    }

    if (del_el->left)
    {
        replace_el->left = del_el->left;
        del_el->left->up = replace_el;
    }

    if (del_el->right != replace_el)
    {
        if (replace_el->right)
            replace_el->right->up = replace_el->up;

        replace_el->up->left = replace_el->right;
        replace_el->right = del_el->right;

        del_el->right->up = replace_el;
    }

    if (del_el->up)
        replace_el->up = del_el->up;
    else
    {
        replace_el->up = nullptr;
        *root = *replace_el;
    }

}

void delete_element(Bin_Tree_Element *tree_el, string& label, highlight_t& highlight, Tree_Output *tree_func, Bin_Tree_Element *root, size_t delete_el = 0)
{
    if (delete_el == 0)
    {
        cout << "Enter the value to delete : ";
        cin >> delete_el;

        ostringstream out;
        out << "Command: 'delete " << delete_el << "'. ";
        label = out.str();
    }

    ostringstream out;
    if (!tree_el)
    {
        out << "Not found!";
        cout << out.str() << endl;
        label += out.str();
    }
    else if (tree_el->element == delete_el)
    {
        if ((tree_el->left) and (tree_el->right))
        {
            if (!tree_el->right->left)
                delete_replace(tree_el, tree_el->right, root);
            else
            {
                Bin_Tree_Element *min_el = tree_el->right;
                while (min_el->left)
                    min_el = min_el->left;

                delete_replace(tree_el, min_el, root);
            }
        }
        else if ((!tree_el->left) and (!tree_el->right)) 
        {
            if (!tree_el->up)
            {
                out << "Error: Unable to delete! Only one node.";
                cerr << out.str() << endl;
                label += out.str();
            }
            else
            {
                Bin_Tree_Element *up_el = tree_el->up;

                if ((up_el->left) and (up_el->left->element == tree_el->element))
                    tree_el->up->left = nullptr;
                else
                    tree_el->up->right = nullptr;
            }
        } 
        else  
        {
            if (!tree_el->up)
            {
                if (tree_el->left)
                {
                    tree_el->left->up = nullptr;
                    *root = *tree_el->left;
                }
                else
                {
                    tree_el->right->up = nullptr;
                    *root = *tree_el->right;
                }
            }
            else
            {
                Bin_Tree_Element *up_el = tree_el->up;

                if (tree_el->left)
                {
                    if ((up_el->left) and (up_el->left->element == tree_el->element))
                    {
                        up_el->left = tree_el->left;
                        tree_el->left->up = up_el;
                    }
                    else
                    {
                        up_el->right = tree_el->left;
                        tree_el->left->up = up_el;
                    }
                }
                else
                {
                    if ((up_el->left) and (up_el->left->element == tree_el->element))
                    {
                        up_el->left = tree_el->right;
                        tree_el->right->up = up_el;
                    }
                    else
                    {
                        up_el->right = tree_el->right;
                        tree_el->right->up = up_el;
                    }
                }
            }
        }

        if (tree_el != root)
            delete(tree_el);
        recreate_tree_f(root, tree_func);
    }
    else
    {
        highlight.node(tree_el, visited);
        if (tree_el->element > delete_el)
        {
            highlight.edge(tree_el, left_edge, tree_el->left ? visited : not_found);
            highlight.node(nodeid_t(nullptr, tree_el, left_edge), not_found);
            delete_element(tree_el->left, label, highlight, tree_func, root, delete_el);
        }
        else
        {
            highlight.edge(tree_el, right_edge, tree_el->right ? visited : not_found);
            highlight.node(nodeid_t(nullptr, tree_el, right_edge), not_found);
            delete_element(tree_el->right, label, highlight, tree_func, root, delete_el);
        }
    }

}

void config_to_file(ostream& out, const Bin_Tree_Element *node)
{
    out << node->element << "\n";

    if (node->left)
        config_to_file(out, node->left);

    if (node->right)
        config_to_file(out, node->right);
}

void save_config(const Bin_Tree_Element *root, string save_file = "test.txt")
{
    cout << "Enter save file : ";
    cin >> save_file;

    fstream out(save_file, fstream::out);

    config_to_file(out, root);

    out.close();
}

size_t nrand(size_t n)
{
    const size_t bucket_size = RAND_MAX / n;
    size_t r;

    do r = rand() / bucket_size;
    while(r >= n);

    return r;
} 

int main(int argc, char* arg_vec[])
{
    size_t size = 0;
    string first_arg;
    vector<size_t> input_config;

    if (argc > 1)
    {
        istringstream in(arg_vec[1]);
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
        }
    }
    
    std::cout << "Binary Tree" << std::endl;
    Tree_Output *tree_output_func = new Tree_Output;
    Tree_Output *tree_output_func1 = new Tree_Output;
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
            test_element = nrand(100);
        }
    }
    else
    {
        test_array = input_config;
    }

//    std::vector<size_t> test_array;
//    test_array = {50, 20, 10, 60, 70, 30, 25, 40, 80};
//    test_array = {70, 30, 25, 40};

    size_t element_number = 1;
    Bin_Tree_Element *first;

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
        Bin_Tree_Element *tree_element = new Bin_Tree_Element(test_array[i], "NON");
        highlight.node(tree_element, added);

        if (i == 0)
        {
            string label = "Creating tree from scratch.";
            highlight.node(nullptr, added);
            dump_tree(nullptr, highlight, "creation", label);
            first = tree_element;
            tree_output_func->create_tree_to_output(0, element_number, *first);
            highlight.leaf(first, added);
        }
        else
        {
            string dummy;
            insert_element(first, tree_element, dummy, highlight, tree_output_func);
        }

        ostringstream out;
        out << "Creating tree, added value " << test_array[i] << '.';
        dump_tree(first, highlight, "creation", out.str());
    }
    std::cout << std::endl;

    tree_output_func->print_tree();

    highlight_t highlight;

    std::map<std::string, std::function<void()>> funcs;
    funcs["help"] = []() {  help(); };
    funcs["add_value"] = [&first, &highlight, &tree_output_func]()
    {
        string label;
        add_value(first, label, highlight, tree_output_func);
        dump_tree(first, highlight, "tree", label);
    };
    funcs["print_tree"] = [&tree_output_func]() { print_tree_f(tree_output_func); };
    funcs["dump"] = [&first, &highlight]() { dump_tree(first, highlight, "tree", "Dump, difference from previous dump is highlighted."); };
    funcs["min_value"] = [&first]() { select_min_value(first); };
    funcs["max_value"] = [&first]() { select_max_value(first); };
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
