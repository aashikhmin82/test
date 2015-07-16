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

bool compare_vec(vector <size_t>& res_vec, vector <size_t>& exp_vec)
{
    size_t i = 0;
    for (auto res_vec_el : res_vec)
    {
        if (res_vec_el != exp_vec[i])
        {
            cout << "Res: " << res_vec_el << " <=> Expect : " << exp_vec[i] << endl;
            return false;
        }

        ++i;
    }

    return true;
}

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

    srand( static_cast<size_t>(time(nullptr)) );

    std::vector< vector<size_t> > tests_data(6);
    std::vector< vector<size_t> > expect_add_res(6);
    std::vector< vector<size_t> > del_el(6);
    std::vector< vector<size_t> > expect_del_res(6);

    size_t num_of_tests = 0;
/*
    tests_data[num_of_tests] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    expect_add_res[num_of_tests] = {8, 4, 2, 1, 3, 6, 5, 7, 12, 10, 9, 11, 16, 14, 13, 15, 18, 17, 19, 20};
    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    expect_del_res[num_of_tests] = {9, 6, 3, 12, 18, 15};
    ++num_of_tests;
    */

//    tests_data[num_of_tests] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
//    expect_add_res[num_of_tests] = {13, 9, 5, 3, 2, 1, 4, 7, 6, 8, 11, 10, 12, 17, 15, 14, 16, 19, 18, 20};
//    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    tests_data[num_of_tests] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    expect_add_res[num_of_tests] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    expect_del_res[num_of_tests] = {3, 6, 9, 12, 15, 18};
    ++num_of_tests;

    tests_data[num_of_tests] = {50, 20, 10, 60, 70, 30, 25, 40, 80};
    expect_add_res[num_of_tests] = {50, 20, 10, 30, 25, 40, 60, 70, 80};
    del_el[num_of_tests] = {50, 10, 30, 40, 70};
    expect_del_res[num_of_tests] = {60, 20, 25, 80};
    ++num_of_tests;

    tests_data[num_of_tests] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    expect_add_res[num_of_tests] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    expect_del_res[num_of_tests] = {18, 15, 12, 9, 6, 3};
    ++num_of_tests;

    //RedBlack num_of_test > 2
    tests_data[num_of_tests] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    expect_add_res[num_of_tests] = {13, 9, 5, 3, 2, 1, 4, 7, 6, 8, 11, 10, 12, 17, 15, 14, 16, 19, 18, 20};
    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    expect_del_res[num_of_tests] = {15, 9, 6, 3, 12, 18};
    ++num_of_tests;

    tests_data[num_of_tests] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    expect_add_res[num_of_tests] = {8, 4, 2, 1, 3, 6, 5, 7, 12, 10, 9, 11, 16, 14, 13, 15, 18, 17, 19, 20};
    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
    expect_del_res[num_of_tests] = {12, 6, 3, 9, 18, 15};
    ++num_of_tests;

//    tests_data[num_of_tests] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
//    expect_add_res[num_of_tests] = {13, 9, 5, 3, 2, 1, 4, 7, 6, 8, 11, 10, 12, 17, 15, 14, 16, 19, 18, 20};
//    del_el[num_of_tests] = {1,2,4,5,7,8,10,11,13,14,16,17,19,20};
//    expect_del_res[num_of_tests] = {15, 9, 6, 3, 12, 18};
//    ++num_of_tests;
    //    std::vector<size_t> test_array = {50, 20, 10, 60, 70, 30, 25, 40, 80};
    //    test_array = {70, 30, 25, 40};

    tests_data[num_of_tests] = {60,66,62,10,50,5,90,11,55,80,85,91,75,6,12,65,14,92,35,40,70,1,15,18,68,3,7,44,45,2,8};
    expect_add_res[num_of_tests] = {62,12,10,5,2,1,3,7,6,8,11,50,35,15,14,18,44,40,45,60,55,80,66,65,70,68,75,90,85,91,92};
    del_el[num_of_tests] = {62,10,50,5,90,11,55,80,85,91,75,6,12,65,14,92,35,40,70,1,15,18,68,3,7};
    expect_del_res[num_of_tests] = {44,8,2,60,45,66};
    ++num_of_tests;

    size_t indx = 0;
    while (indx < num_of_tests)
    {
        if (indx > 2)
        {
            cout << "RED BLACK is ON" << endl;
            rbtree = 1;
            debugf = 1;
        }
        cout << endl;
        cout << "Test NUMBER : " << indx << endl;

        std::cout << "Binary Tree" << std::endl;
        std::shared_ptr<Tree_Output> tree_output_func (make_shared<Tree_Output>());
        std::shared_ptr<Tree_Output> tree_output_func1 (make_shared<Tree_Output>());
        *tree_output_func = *tree_output_func1;

        std::vector<size_t> test_array = tests_data[indx];
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
            std::shared_ptr<Bin_Tree_Element> tree_element (make_shared<Bin_Tree_Element>(test_array[i], node_colour::red));

            if (i == 0)
            {
                string label = "Creating tree from scratch.";
                dump_tree(nullptr, highlight, "creation", label);
                first = tree_element;
                first->colour() = node_colour::black;
                tree_output_func->create_tree_to_output(0, element_number, first);
            }
            else
            {
                string dummy;
                insert_element(first, tree_element, dummy, highlight, tree_output_func, rbtree);
                check_rb(first, debugf);
            }

            ostringstream out;
        }
        std::cout << std::endl;

        std::shared_ptr<test_vector> test_result (make_shared<test_vector>());
        create_test_result(test_result, first);
        vector <size_t> test_result_vec = test_result->get_test_ar();

        cout << "Test Result : " << endl;
        for (auto test_el : test_result_vec)
        {
            cout << test_el << " ";
        }
        cout << endl;

        cout << "Expected Result : ";
        for (auto exp_el : expect_add_res[indx])
        {
            cout << exp_el << " ";
        }
        cout << endl;

        if (compare_vec(test_result_vec, expect_add_res[indx]))
            cout << "PASSED" << endl;
        else
        {
            cout << "FAILED" << endl;
            exit(1);
        }

        highlight_t highlight;
        string label;
        
        cout << "Delete : ";
        for (auto del_element : del_el[indx])
        {
            cout << del_element << " ";
            delete_element(first, label, highlight, tree_output_func, first, rbtree, del_element);

            check_rb(first, debugf);
        }
        cout << endl;

        std::shared_ptr<test_vector> test_del_result (make_shared<test_vector>());
        print_tree_f(tree_output_func);
        create_test_result(test_del_result, first);
        vector <size_t> test_del_result_vec = test_del_result->get_test_ar();

        cout << "Test Delete Result : " << endl;
        for (auto test_el : test_del_result_vec)
        {
            cout << test_el << " ";
        }
        cout << endl;

        if (compare_vec(test_del_result_vec, expect_del_res[indx]))
            cout << "PASSED" << endl;
        else
        {
            cout << "FAILED" << endl;
            exit(1);
        }

        ++indx;
    }
}

