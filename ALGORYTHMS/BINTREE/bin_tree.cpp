#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>
#include <memory>
#include <cassert>

#include "dot.h"
#include "bin_tree.h"

using namespace dot;
using namespace std;

void recreate_tree_f(std::shared_ptr<Bin_Tree_Element> tree_el, std::shared_ptr<Tree_Output> tree_func, size_t level, size_t element_num)
{
    size_t level_tmp = level;
    if (level > 22)
        throw new string("Maybe endless loop.");

    size_t element_num_tmp = element_num;
    if (level == 0)
    {
        tree_func->delete_old_tree();
        element_num = 1;
        element_num_tmp = element_num;
    }

    tree_func->create_tree_to_output(level, element_num, tree_el);

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

void rotate(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Bin_Tree_Element> tree_el, std::shared_ptr<Tree_Output> tree_func)
{
    std::shared_ptr<Bin_Tree_Element> up_tree_el = tree_el->up();
    std::shared_ptr<Bin_Tree_Element> tmp_root (new Bin_Tree_Element(0,"NON"));
    *tmp_root = *root;
    std::shared_ptr<Bin_Tree_Element> tree_el_tmp (new Bin_Tree_Element(0,"NON"));
    *tree_el_tmp = *(tree_el->up());
    std::shared_ptr<Bin_Tree_Element>  upup_tree_el = nullptr;

    assert(tree_el->up());
    if (tree_el->up()->up())
            upup_tree_el = tree_el->up()->up();
    else
    {
        tree_el->up() = root;
        tree_el_tmp = tmp_root;
        up_tree_el = tmp_root;
    }

/*
      x         y
     / \       / \
    y   c  <- a   x
   / \           / \
  a   b         b   c
*/
    if ((tree_el->up()->right()) and (tree_el->up()->right() == tree_el))
    {
        tree_el_tmp->right() = tree_el->left();
        tree_el_tmp->up() = tree_el->up();
        
        if (tree_el->up()->left())
            tree_el->up()->left()->up() = tree_el;

        if (tree_el->right())
            tree_el->right()->up() = tree_el->up();

        tree_el->left() = tree_el;
    }
    else
/*
      x         y
     / \       / \
    y   c  -> a   x
   / \           / \
  a   b         b   c
*/
    {
        tree_el_tmp->left() = tree_el->right();
        tree_el_tmp->up() = tree_el->up();

        if (tree_el->left())
            tree_el->left()->up() = tree_el->up();

        if (tree_el->up()->right())
            tree_el->up()->right()->up() = tree_el;

        tree_el->right() = tree_el;
    }

    *(tree_el->up()) = *tree_el;
    tree_el->up()->up() = upup_tree_el;
    *tree_el = *tree_el_tmp;

    if (!upup_tree_el)
        root->colour() = "black";

    recreate_tree_f(root, tree_func);
}

void fix_rbtree(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Bin_Tree_Element> tree_element, std::shared_ptr<Tree_Output> tree_func)
{
    assert(tree_element->up());

    std::shared_ptr<Bin_Tree_Element> uncle_el = nullptr;
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
            std::shared_ptr<Bin_Tree_Element> sub_root = tree_element->up()->up();

            bool condition_a = 
                (tree_element->element() < sub_root->element()) and 
                (tree_element->up()->right()) and
                (tree_element->up()->right() == tree_element);

            bool condition_b =
                (tree_element->element() > sub_root->element()) and 
                (tree_element->up()->left()) and
                (tree_element->up()->left() == tree_element);

            if (condition_a or condition_b)
            {
                std::shared_ptr<Bin_Tree_Element>  old_up_el = tree_element->up();

                rotate(root, tree_element, tree_func);

                tree_element->up()->colour() = "black";
                tree_element->up()->up()->colour() = "red";

                rotate(root,tree_element->up(), tree_func);
            }
            else
            {
                tree_element->up()->colour() = "black";
                tree_element->up()->up()->colour() = "red";

                rotate(root, tree_element->up(), tree_func);
            }
        }
    }
}

void insert_element(std::shared_ptr<Bin_Tree_Element> tree_element, std::shared_ptr<Bin_Tree_Element> bin_tree_element, 
        string& label, highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, size_t rbtree, 
        size_t level, size_t element_num, std::shared_ptr<Bin_Tree_Element> root)
{
    if (!root)
        root = tree_element;

    if (tree_element->element() > bin_tree_element->element()) 
    {
        if (!tree_element->left()) 
        {
            tree_func->create_tree_to_output(level, element_num * 2, bin_tree_element);
            tree_element->left() = bin_tree_element;
            bin_tree_element->up() = tree_element;
            if (rbtree)
                fix_rbtree(root, bin_tree_element, tree_func);
        } 
        else 
            insert_element(tree_element->left(), bin_tree_element, 
                            label, highlight, tree_func, rbtree, 
                            ++level, element_num * 2, root);
    } 
    else 
    {
        if (!tree_element->right()) 
        {
            tree_func->create_tree_to_output(level, element_num * 2 + 1, bin_tree_element);
            tree_element->right() = bin_tree_element;
            bin_tree_element->up() = tree_element;
            if (rbtree)
                fix_rbtree(root, bin_tree_element, tree_func);
        } 
        else 
            insert_element(tree_element->right(), bin_tree_element, 
                            label, highlight, tree_func, rbtree, 
                            ++level, element_num * 2 + 1, root);
    }
}

void add_value (std::shared_ptr<Bin_Tree_Element> first, string& label, highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, bool rbtree, size_t value_to_add)
{
    cout << "Value to add : ";
    cin >> value_to_add;

    ostringstream out;
    out << "Command: 'add " << value_to_add << "'. ";
    label = out.str();

    std::shared_ptr<Bin_Tree_Element> tree_element (new Bin_Tree_Element(value_to_add, "red"));
    insert_element(first, tree_element, label, highlight, tree_func, rbtree);
}

void print_tree_f(std::shared_ptr<Tree_Output> tree_func)
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

void select_max_value(const std::shared_ptr<Bin_Tree_Element> root)
{
    if (root->right())
        select_max_value(root->right());
    else
        cout << "MAX Value : " << root->element() << endl;
}

void select_min_value(const std::shared_ptr<Bin_Tree_Element> root)
{
    if (root->left())
        select_min_value(root->left());
    else
        cout << "MIN Value : " << root->element() << endl;
}

void tree_search(const std::shared_ptr<Bin_Tree_Element> root, string& label, highlight_t& highlight, size_t search_el)
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

void delete_replace(std::shared_ptr<Bin_Tree_Element> del_el, std::shared_ptr<Bin_Tree_Element> replace_el, std::shared_ptr<Bin_Tree_Element> root)
{
    if (del_el->left())
    {
        replace_el->left() = del_el->left();
    }

    replace_el->up() = del_el->up();

    *del_el = *replace_el;
}

void delete_tree_el(std::shared_ptr<Bin_Tree_Element> tree_el, string& label, highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, std::shared_ptr<Bin_Tree_Element> root)
{
        ostringstream out;

        if ((tree_el->left()) and (tree_el->right()))
        {
                std::shared_ptr<Bin_Tree_Element> replace_el = tree_el->right();
                if (replace_el->left())
                {
                    while (replace_el->left())
                        replace_el = replace_el->left();
                    replace_el->up()->left() = replace_el->right();

                    if (replace_el->right())
                        replace_el->right()->up() = replace_el->up();
                    replace_el->right() = tree_el->right();
                }
                else
                {
                    if (replace_el->right())
                        replace_el->right()->up() = replace_el->up();
                }

                delete_replace(tree_el, replace_el, root);
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
                std::shared_ptr<Bin_Tree_Element> up_el = tree_el->up();

                if (up_el->left() == tree_el)
                    tree_el->up()->left() = nullptr;
                else
                {
                    tree_el->up()->right() = nullptr;
                 }
            }
        } 
        else  
        {
                if (tree_el->left())
                {
                    tree_el->left()->up() = tree_el->up();
                    *tree_el = *(tree_el->left());
                    if (tree_el->left())
                        tree_el->left()->up() = tree_el;
                }
                else
                {
                    tree_el->right()->up() = tree_el->up();
                    *tree_el = *(tree_el->right());
                    if (tree_el->right())
                        tree_el->right()->up() = tree_el;
                }
        }

        recreate_tree_f(root, tree_func);
}

void delete_element(std::shared_ptr<Bin_Tree_Element> tree_el, string& label, highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, std::shared_ptr<Bin_Tree_Element> root, size_t delete_el)
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
        delete_tree_el(tree_el, label, highlight, tree_func, root);
    }
    else
    {
        if (tree_el->element() > delete_el)
            delete_element(tree_el->left(), label, highlight, tree_func, root, delete_el);
        else
            delete_element(tree_el->right(), label, highlight, tree_func, root, delete_el);
    }
}

void search_and_rotate(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Tree_Output> tree_func, size_t search_el)
{
    if (search_el == 0)
    {
        cout << "Enter the value to search : ";
        cin >> search_el;
    }

    std::shared_ptr<Bin_Tree_Element> tree_el = root;
    while (tree_el)
    {
        cout << "E : " << tree_el->element() << endl;
        if (tree_el->element() == search_el)
        {
            cout << "Found : " << tree_el->element() << "\t Colour : " << tree_el->colour() << endl;
            rotate(root, tree_el, tree_func);
            break;
        }
        else if (tree_el->element() > search_el)
            tree_el = tree_el->left();
        else
            tree_el = tree_el->right();
    }
}

void config_to_file(ostream& out, const std::shared_ptr<Bin_Tree_Element> node)
{
    out << node->element() << "\n";

    if (node->left())
        config_to_file(out, node->left());

    if (node->right())
        config_to_file(out, node->right());
}

void check_all_prop(std::shared_ptr<Bin_Tree_Element> element, const size_t way_black, size_t cur_way = 0)
{
    if (element->colour() == "black")
        ++cur_way;
    else
        if (((!element->right()) or (element->right()->colour() == "black")) and 
            ((!element->left()) or (element->left()->colour() == "black")))
        {
            cout << "[DEBUG] PASSED -> all_black" << endl;
        }
        else
        {
            cout << "[DEBUG] FAILED -> not all black " << endl;
            throw new string("Incorrect red node.");
        }

    if (element->left())
        check_all_prop(element->left(), way_black, cur_way);

    if (element->right())
        check_all_prop(element->right(), way_black, cur_way);

    if ((!element->left()) or (!element->right()))
    {
        ++cur_way;
        if (way_black == cur_way)
            cout << "[DEBUG] PASSED: " << element->element() << " Black Way : " << way_black << "  " << "Current way : " << cur_way << endl;
        else
        {
            cout << "[DEBUG] FAILED!!!: " << element->element() << " Black Way : " << way_black << "  " << "Current way : " << cur_way << endl;
            throw new string("Incorrect number of black nodes in the way.");
        }
    }
}

void check_rb(std::shared_ptr<Bin_Tree_Element> root, const bool debugf)
{
    if (!debugf)
        return;

    size_t way_black = 1;
    if (root->left())
    {
        std::shared_ptr<Bin_Tree_Element> element = root->left();
        while (element)
        {
            if (element->colour() == "black")
                ++way_black;
            element = element->left();
        }

    }
    ++way_black;

    try {
        check_all_prop(root, way_black);
    } catch (string* error_str) {
        cout << "Error : " << *error_str << endl;
        exit(1);
    }
}

void create_test_result(std::shared_ptr<test_vector> test_ar, const std::shared_ptr<Bin_Tree_Element> node)
{
    test_ar->test_push_back(node->element());

    if (node->left())
        create_test_result(test_ar, node->left());

    if (node->right())
        create_test_result(test_ar, node->right());
}

void save_config(const std::shared_ptr<Bin_Tree_Element> root, string save_file)
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

