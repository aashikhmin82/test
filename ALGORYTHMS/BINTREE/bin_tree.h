#pragma once

#include <cstddef>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>
#include <memory>

#include "dot.h"

class test_vector
{
    private:
        std::vector <size_t> test_ar;

    public:
        void test_push_back(size_t tree_el)
        {
            test_ar.push_back(tree_el);
        }

        std::vector <size_t> get_test_ar()
        {
            return test_ar;
        }
};

class Tree_Output
{
    private:
        std::vector< std::vector<std::shared_ptr<Bin_Tree_Element> > > bin_tree_struct;

    public:
        void create_tree_to_output(const size_t level, size_t element_num, std::shared_ptr<Bin_Tree_Element>& obj_element)
        {
            if (bin_tree_struct.size() < level + 1)
            {
//                std::vector<std::shared_ptr<Bin_Tree_Element> > row(pow(2,level),nullptr);
                std::vector<std::shared_ptr<Bin_Tree_Element> > row(1 << level,nullptr);
                bin_tree_struct.push_back(row);
            }

            size_t element_array_num = 0;
            if (element_num != 0)
//                element_array_num = element_num - pow(2, level);
                element_array_num = element_num - (1 << level);

            bin_tree_struct[level][element_array_num] = obj_element;
        }

        void delete_old_tree()
        {
            bin_tree_struct.clear();
        }

        void print_tree()
        {
            size_t i = 0;
//            size_t spaces_amount = (pow(2,bin_tree_struct.size() - 1) * 6 + pow(2,bin_tree_struct.size() - 1)) / 2;
            size_t spaces_amount = ((1 << (bin_tree_struct.size() - 1)) - 1) * 6 + (1 << (bin_tree_struct.size() - 1)) / 2;

            std::vector< std::vector<std::shared_ptr<Bin_Tree_Element> > >::iterator matrix_iter = bin_tree_struct.begin();
            while (matrix_iter != bin_tree_struct.end())
            {
                std::string spaces(spaces_amount, ' ');
                std::cout << i << " : " << spaces;
//                spaces_amount = spaces_amount - (5 * pow(2,i) / 2);
                spaces_amount = spaces_amount - (5 * (1 << i) / 2);
                ++i;
                std::vector<std::shared_ptr<Bin_Tree_Element> >::iterator row_iter = (*matrix_iter).begin();
                while (row_iter != (*matrix_iter).end())
                {
                    if (*row_iter)
                        std::cout << (*row_iter)->element() << "(" << (*row_iter)->colour() << ") ";
                    else
                        std::cout << "NULL ";

                    ++row_iter;
                }
                std::cout << std::endl;
                ++matrix_iter;
            }
        }
};

void recreate_tree_f(std::shared_ptr<Bin_Tree_Element> tree_el, std::shared_ptr<Tree_Output> tree_func, size_t level = 0, size_t element_num = 0);

void rotate(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Bin_Tree_Element> tree_el, std::shared_ptr<Tree_Output> tree_func);
void fix_rbtree(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Bin_Tree_Element> tree_element, std::shared_ptr<Tree_Output> tree_func);

void insert_element(std::shared_ptr<Bin_Tree_Element> tree_element, std::shared_ptr<Bin_Tree_Element> bin_tree_element, 
        std::string& label, dot::highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, size_t rbtree, 
                size_t level = 1, size_t element_num = 1, std::shared_ptr<Bin_Tree_Element> root = nullptr);

void add_value (std::shared_ptr<Bin_Tree_Element> first, std::string& label, dot::highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, bool rbtree, size_t value_to_add = 0);


void print_tree_f(std::shared_ptr<Tree_Output> tree_func);
void help();

void select_max_value(const std::shared_ptr<Bin_Tree_Element> root);
void select_min_value(const std::shared_ptr<Bin_Tree_Element> root);

void tree_search(const std::shared_ptr<Bin_Tree_Element> root, std::string& label, dot::highlight_t& highlight, size_t search_el = 0);

void delete_replace(std::shared_ptr<Bin_Tree_Element> del_el, std::shared_ptr<Bin_Tree_Element> replace_el, std::shared_ptr<Bin_Tree_Element> root);
void delete_tree_el(std::shared_ptr<Bin_Tree_Element> tree_el, std::string& label, dot::highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, std::shared_ptr<Bin_Tree_Element> root, bool rbtree);
void delete_element(std::shared_ptr<Bin_Tree_Element> tree_el, std::string& label, dot::highlight_t& highlight, std::shared_ptr<Tree_Output> tree_func, std::shared_ptr<Bin_Tree_Element> root, bool rbtree, size_t delete_el = 0);
void search_and_rotate(std::shared_ptr<Bin_Tree_Element> root, std::shared_ptr<Tree_Output> tree_func, size_t search_el = 0);
void config_to_file(std::ostream& out, const std::shared_ptr<Bin_Tree_Element> node);
void save_config(const std::shared_ptr<Bin_Tree_Element> root, std::string save_file = "test.txt");
size_t nrand(size_t n);
void create_test_result(std::shared_ptr<test_vector> test_ar, const std::shared_ptr<Bin_Tree_Element> node);
void check_rb(std::shared_ptr<Bin_Tree_Element> root, bool debugf);
