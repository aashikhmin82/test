#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>
#include <functional>

using namespace std;

class Bin_Tree_Element
{
    public:
        size_t element;
        std::string colour;
        Bin_Tree_Element *up = NULL, 
                         *left = NULL, 
                         *right = NULL;

        Bin_Tree_Element(const size_t& ar_el, const std::string ar_col) : element(ar_el), colour(ar_col) {}
};

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
            size_t element_array_num = element_num - pow(2, level);
            bin_tree_struct[level][element_array_num] = &obj_element;
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
                    if ((*row_iter) != nullptr) 
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

void insert_element(Bin_Tree_Element *root, Bin_Tree_Element *bin_tree_element, Tree_Output *tree_func, size_t level = 1, size_t element_num = 1)
{
    if (root->element > bin_tree_element->element) 
    {
        if (root->left == NULL) 
        {
            element_num = element_num * 2;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            root->left = bin_tree_element;
        } 
        else 
        {
            ++level;
            element_num = element_num * 2;
            insert_element(root->left, bin_tree_element, tree_func, level, element_num);
        }
    } 
    else 
    {
        if (root->right == NULL) 
        {
            element_num = element_num * 2 + 1;
            tree_func->create_tree_to_output(level, element_num, *bin_tree_element);
            root->right = bin_tree_element;
        } 
        else 
        {
            element_num = element_num * 2 + 1;
            ++level;
            insert_element(root->right, bin_tree_element, tree_func, level, element_num);
        }
    }
}

void add_value (Bin_Tree_Element *first, Tree_Output *tree_func)
{
    size_t value_to_add;
    cout << "Value to add : ";
    cin >> value_to_add;
    Bin_Tree_Element *tree_element = new Bin_Tree_Element(value_to_add, "NON");
    insert_element(first, tree_element, tree_func);

}

void print_tree_f(Tree_Output *tree_func)
{
    tree_func->print_tree();
}

void help()
{
    cout << "You can enter :\n" 
        << "\thelp : Output this message.\n"
        << "\tadd_value : Enter the size_t value to the tree.\n"
        << "\tmin_value : Print Min value.\n"
        << "\tmax_value : Print Max value.\n"
        << "\tprint_tree : Output the Tree.\n" 
        << "\texit : to exit" << endl;
}

void select_max_value(const Bin_Tree_Element *root)
{
    if (root->right != NULL)
        select_max_value(root->right);
    else
        cout << "MAX Value : " << root->element << endl;
}

void select_min_value(const Bin_Tree_Element *root)
{
    if (root->left != NULL)
        select_min_value(root->left);
    else
        cout << "MIN Value : " << root->element << endl;
}

size_t nrand(size_t n)
{
    const size_t bucket_size = RAND_MAX / n;
    size_t r;

    do r = rand() / bucket_size;
    while(r >= n);

    return r;
} 

int main()
{
    std::cout << "Binary Tree" << std::endl;
    Tree_Output *tree_output_func = new Tree_Output;
    Tree_Output *tree_output_func1 = new Tree_Output;
    *tree_output_func = *tree_output_func1;

    srand( static_cast<size_t>(time(nullptr)) );

    size_t size;
    std::cout << "Enter the Size : ";
    std::cin >> size;

    std::vector<size_t> test_array(size);
    for (auto& test_element : test_array) {
//        test_element = rand() % 100;
        test_element = nrand(100);
    }
//    test_array = {50, 20, 10, 60, 70, 30, 40, 80, 90, 100};

    size_t level = 0;
    size_t element_number = 1;
    Bin_Tree_Element *first = new Bin_Tree_Element(test_array[0], "NON");
    tree_output_func->create_tree_to_output(level, element_number, *first);

    std::cout << "Test array : ";
    for (auto& test_element : test_array) {
        std::cout << test_element << " ";
    }
    std::cout << std::endl;

    std::cout << "Test array : ";
    level = 1;
    element_number = 1;
    for (size_t i = 1; i < test_array.size(); ++i) 
    {
        Bin_Tree_Element *tree_element = new Bin_Tree_Element(test_array[i], "NON");
        insert_element(first, tree_element, tree_output_func);
    }
    std::cout << std::endl;

    tree_output_func->print_tree();

    std::map<std::string, std::function<void()>> funcs;
    funcs["help"] = []() {  help(); };
    funcs["add_value"] = [&first, &tree_output_func]() { add_value(first, tree_output_func); };
    funcs["print_tree"] = [&tree_output_func]() { print_tree_f(tree_output_func); };
    funcs["min_value"] = [&first]() { select_min_value(first); };
    funcs["max_value"] = [&first]() { select_max_value(first); };
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
