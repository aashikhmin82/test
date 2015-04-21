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
                        cout << (*row_iter)->element() << "(" << (*row_iter)->colour() << ") ";
                    else 
                        cout << "NULL ";

                    ++row_iter;
                }
                cout << endl;
                ++matrix_iter;
            }
        }
};

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

    if (tree_el->left(false))
        recreate_tree_f(tree_el->left(false), tree_func, level + 1, element_num * 2);
    else 
        element_num = element_num * 2;

    level = level_tmp;
    element_num = element_num_tmp;

    if (tree_el->right(false))
        recreate_tree_f(tree_el->right(false), tree_func, level + 1, element_num * 2 + 1);
    else 
        element_num = element_num * 2 + 1;
}

void rotate(Bin_Tree_Element *root, Bin_Tree_Element *tree_el, Tree_Output *tree_func)
{
    Bin_Tree_Element *up_tree_el = tree_el->up();
    Bin_Tree_Element* tmp_root = new Bin_Tree_Element(0,"NON");
    *tmp_root = *root;
    Bin_Tree_Element* tree_el_tmp = tree_el->up();
    Bin_Tree_Element* upup_tree_el = nullptr;

    if (tree_el->up()->up())
    {
        if (tree_el->up()->up()->element() == root->element())
            upup_tree_el = root;
        else
            upup_tree_el = tree_el->up()->up();

        delete(tmp_root);
    }
    else
    {
        tree_el->up() = tmp_root;
        tree_el_tmp = tree_el->up();
        up_tree_el = tree_el->up();
    }

    if ((tree_el->up()->right()) and (tree_el->up()->right() == tree_el))
    {
        tree_el_tmp->right() = tree_el->left();
        tree_el_tmp->up() = tree_el;

        tree_el->left() = up_tree_el;
    }
    else
    {
        tree_el_tmp->left() = tree_el->right();
        tree_el_tmp->up() = tree_el;

        tree_el->right() = up_tree_el;
    }

    tree_el->up() = upup_tree_el;
    up_tree_el = tree_el;
    tree_el = tree_el_tmp;

    if (!upup_tree_el)
    {
        *root = *(tree_el->up());
        root->colour() = "black";
        tree_el->up() = root;

        if (tmp_root->right())
        {
            //Bin_Tree_Element* tree_el_to_delete = tmp_root->right()->up();
            tmp_root->right()->up() = tmp_root;
            //delete (tree_el_to_delete); //TODO
        }
        if (tmp_root->left())
        {
            tmp_root->left()->up() = tmp_root;
        }
    }
    else if ((upup_tree_el->left()) and (upup_tree_el->left() == tree_el))
        upup_tree_el->left() = tree_el->up();
    else
        upup_tree_el->right() = tree_el->up();

    recreate_tree_f(root, tree_func);
}

void fix_rbtree(Bin_Tree_Element *root, Bin_Tree_Element *tree_element, Tree_Output *tree_func)
{
    Bin_Tree_Element *uncle_el = nullptr;
    if (tree_element->up()->up())
    {
        if ((tree_element->up()->up()->left()) and (tree_element->up() == tree_element->up()->up()->left()))
        {
            if (tree_element->up()->up()->right())
                uncle_el = tree_element->up()->up()->right();
        }
        else
        {
            if (tree_element->up()->up()->left())
                uncle_el = tree_element->up()->up()->left();
        }
    }

    if ((tree_element->colour() == "red") and (tree_element->up()->colour() == "red"))
    {
        if ((uncle_el) and (uncle_el->colour() == "red"))
        {
            tree_element->up()->colour() = "black";
            tree_element->up()->up()->colour() = "red";
            uncle_el->colour() = "black";
            root->colour() = "black";

            if ((tree_element->up()->up()->up()) and (tree_element->up()->up()->up()->up()))
                fix_rbtree(root, tree_element->up()->up(), tree_func);
        }
        else if ((!uncle_el) or (uncle_el->colour() == "black"))
        {
            if (((tree_element->element() < root->element()) and (tree_element->up()->right()) and
                    (tree_element->up()->right() == tree_element)) || 
                    ((tree_element->element() > root->element()) and (tree_element->up()->left()) and
                                        (tree_element->up()->left() == tree_element)))
            {
                Bin_Tree_Element* old_up_el = tree_element->up();

                rotate(root, tree_element, tree_func);

                old_up_el->up()->colour() = "black";
                old_up_el->up()->up()->colour() = "red";

                rotate(root,old_up_el->up(), tree_func);
            }
            /*
            else if ((tree_element->element() > root->element()) and (tree_element->up()->left()) and   
                    (tree_element->up()->left() == tree_element))
            {
                Bin_Tree_Element* old_up_el = tree_element->up();
                rotate(root, tree_element, tree_func);
                old_up_el->up()->colour() = "black";
                old_up_el->up()->up()->colour() = "red";
                rotate(root,old_up_el->up(), tree_func);
            }
            */
            else
            {
                tree_element->up()->colour() = "black";
                tree_element->up()->up()->colour() = "red";

                rotate(root, tree_element->up(), tree_func);
            }
        }
    }
}

void insert_element(Bin_Tree_Element *tree_element, Bin_Tree_Element *bin_tree_element, 
        string& label, highlight_t& highlight, Tree_Output *tree_func, size_t rbtree, 
        size_t level = 1, size_t element_num = 1, Bin_Tree_Element *root = nullptr)
{
    if (!root)
        root = tree_element;

    if (tree_element->element() > bin_tree_element->element()) 
    {
        if (!tree_element->left()) 
        {
            element_num = element_num * 2;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            tree_element->left() = bin_tree_element;
            bin_tree_element->up() = tree_element;
            if (rbtree)
                fix_rbtree(root, bin_tree_element, tree_func);
        } 
        else 
        {
            ++level;
            element_num = element_num * 2;
            insert_element(tree_element->left(), bin_tree_element, 
                            label, highlight, tree_func, rbtree, 
                            level, element_num, root);
        }
    } 
    else 
    {
        if (!tree_element->right()) 
        {
            element_num = element_num * 2 + 1;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            tree_element->right() = bin_tree_element;
            bin_tree_element->up() = tree_element;
            if (rbtree)
                fix_rbtree(root, bin_tree_element, tree_func);
        } 
        else 
        {
            ++level;
            element_num = element_num * 2 + 1;
            insert_element(tree_element->right(), bin_tree_element, 
                            label, highlight, tree_func, rbtree, 
                            level, element_num, root);
        }
    }
}

void add_value (Bin_Tree_Element *first, string& label, highlight_t& highlight, Tree_Output *tree_func, bool rbtree, size_t value_to_add = 0)
{
    cout << "Value to add : ";
    cin >> value_to_add;

    ostringstream out;
    out << "Command: 'add " << value_to_add << "'. ";
    label = out.str();

    Bin_Tree_Element *tree_element = new Bin_Tree_Element(value_to_add, "red");
    insert_element(first, tree_element, label, highlight, tree_func, rbtree);
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
        << "\trotate : Rotate [element]\n"
        << "\texit : to exit" << endl;
}

void select_max_value(const Bin_Tree_Element *root)
{
    if (root->right())
        select_max_value(root->right());
    else
        cout << "MAX Value : " << root->element() << endl;
}

void select_min_value(const Bin_Tree_Element *root)
{
    if (root->left())
        select_min_value(root->left());
    else
        cout << "MIN Value : " << root->element() << endl;
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
    else if (root->element() == search_el)
    {
        out << "Found : " << root->element() << "\t Colour : " << root->colour();
        cout << out.str() << endl;
        label += out.str();
    }
    else
    {
        if (root->element() > search_el)
            tree_search(root->left(), label, highlight, search_el);
        else
            tree_search(root->right(), label, highlight, search_el);
    }
}

void delete_replace(Bin_Tree_Element *del_el, Bin_Tree_Element *replace_el, Bin_Tree_Element *root)
{
    if (del_el->up())
    {
        if ((del_el->up()->left()) and (del_el->up()->left() == del_el))
            del_el->up()->left() = replace_el;
        else
            del_el->up()->right() = replace_el;
    }

    if (del_el->left())
    {
        replace_el->left() = del_el->left();
        del_el->left()->up() = replace_el;
    }

    if (del_el->right() != replace_el)
    {
        if (replace_el->right())
            replace_el->right()->up() = replace_el->up();

        replace_el->up()->left() = replace_el->right();
        replace_el->right() = del_el->right();

        del_el->right()->up() = replace_el;
    }

/*
    if (del_el->up())
        replace_el->up() = del_el->up();
    else
    {
        replace_el->up() = nullptr;
        *root = *replace_el;
    }
    */
    replace_el->up() = del_el->up();
    if (not del_el->up())
            *root = *replace_el;

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
    else if (tree_el->element() == delete_el)
    {
        if ((tree_el->left()) and (tree_el->right()))
        {
            if (!tree_el->right()->left())
                delete_replace(tree_el, tree_el->right(), root);
            else
            {
                Bin_Tree_Element *min_el = tree_el->right();
                while (min_el->left())
                    min_el = min_el->left();

                delete_replace(tree_el, min_el, root);
            }
        }
        else if ((!tree_el->left()) and (!tree_el->right())) 
        {
            if (!tree_el->up())
            {
                out << "Error: Unable to delete! Only one node.";
                cerr << out.str() << endl;
                label += out.str();
            }
            else
            {
                Bin_Tree_Element *up_el = tree_el->up();

                if ((up_el->left()) and (up_el->left() == tree_el))
                    tree_el->up()->left() = nullptr;
                else
                    tree_el->up()->right() = nullptr;
            }
        } 
        else  
        {
            if (!tree_el->up())
            {
                if (tree_el->left())
                {
                    tree_el->left()->up() = nullptr;
                    *root = *tree_el->left();
                }
                else
                {
                    tree_el->right()->up() = nullptr;
                    *root = *tree_el->right();
                }
            }
            else
            {
                Bin_Tree_Element *up_el = tree_el->up();

                if (tree_el->left())
                {
                    if ((up_el->left()) and (up_el->left() == tree_el))
                    {
                        up_el->left() = tree_el->left();
                        tree_el->left()->up() = up_el;
                    }
                    else
                    {
                        up_el->right() = tree_el->left();
                        tree_el->left()->up() = up_el;
                    }
                }
                else
                {
                    if ((up_el->left()) and (up_el->left() == tree_el))
                    {
                        up_el->left() = tree_el->right();
                        tree_el->right()->up() = up_el;
                    }
                    else
                    {
                        up_el->right() = tree_el->right();
                        tree_el->right()->up() = up_el;
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
        if (tree_el->element() > delete_el)
            delete_element(tree_el->left(), label, highlight, tree_func, root, delete_el);
        else
            delete_element(tree_el->right(), label, highlight, tree_func, root, delete_el);
    }

}

void search_and_rotate(Bin_Tree_Element *root, Tree_Output *tree_func, size_t search_el = 0)
{
    if (search_el == 0)
    {
        cout << "[DEBUG] Current root: " << root->element() << " (" << root << ")" << endl;
        cout << "Enter the value to search : ";
        cin >> search_el;
    }

    Bin_Tree_Element *tree_el = root;
    //    Bin_Tree_Element tmp_root = *root;
    while (tree_el)
    {
        cout << "E : " << tree_el->element() << endl;
        if (tree_el->element() == search_el)
        {
            cout << "Found : " << tree_el->element() << "\t Colour : " << tree_el->colour() << endl;
//            tree_el = tree_el;
            rotate(root, tree_el, tree_func);
            break;
        }
        else if (tree_el->element() > search_el)
        {
            //    tree_search(tree_el->left, search_el);
            tree_el = tree_el->left();
        }
        else
        {
            //  tree_search(tree_el->right, search_el);
            tree_el = tree_el->right();
        }
    }
}

//    cout << "Left Rotate : " << endl;
//    cout << "\t" << tree_el->element() << "\t" << tree_el->colour() << endl;
//    cout << "\tUp: " << tree_el->up()->element() << "\t" << tree_el->up()->colour() << endl;

void config_to_file(ostream& out, const Bin_Tree_Element *node)
{
    out << node->element() << "\n";

    if (node->left())
        config_to_file(out, node->left());

    if (node->right())
        config_to_file(out, node->right());
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
    bool rbtree = 0;
    size_t arg_i = 1;
//    size_t argproc = 0;
    string first_arg;
    vector<size_t> input_config;

    while (argc > 1)
//    if (argc > 1)
    {
//        istringstream in(arg_vec[1]);
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
//            istringst0ream in(arg_vec[2]);
//            in >> tr;
            rbtree = 1;
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
            test_element = nrand(1000);
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
        Bin_Tree_Element *tree_element = new Bin_Tree_Element(test_array[i], "red");

        if (i == 0)
        {
            string label = "Creating tree from scratch.";
            dump_tree(nullptr, highlight, "creation", label);
            first = tree_element;
            first->colour() = "black";
            tree_output_func->create_tree_to_output(0, element_number, *first);
        }
        else
        {
            string dummy;
            insert_element(first, tree_element, dummy, highlight, tree_output_func, rbtree);
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
    funcs["add_value"] = [&first, &highlight, &tree_output_func, &rbtree]()
    {
        string label;
        add_value(first, label, highlight, tree_output_func, rbtree);
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
