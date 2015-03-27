#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>

using namespace std;

static bool const treat_moved_node_like_new = true;

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
            bin_tree_element->up = root;
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
            bin_tree_element->up = root;
        } 
        else 
        {
            ++level;
            element_num = element_num * 2 + 1;
            insert_element(root->right, bin_tree_element, tree_func, level, element_num);
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

    if (tree_el->left == NULL) 
        element_num = element_num * 2;
    else 
    {
        ++level;
        element_num = element_num * 2;
        recreate_tree_f(tree_el->left, tree_func, level, element_num);
    }

    level = level_tmp;
    element_num = element_num_tmp;

    if (tree_el->right == NULL) 
        element_num = element_num * 2 + 1;
    else 
    {
        ++level;
        element_num = element_num * 2 + 1;
        recreate_tree_f(tree_el->right, tree_func, level, element_num);
    }
}

void add_value (Bin_Tree_Element *first, Tree_Output *tree_func, size_t value_to_add = 0)
{
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
        << "\tadd_value [value]: Enter the size_t value to the tree.\n"
        << "\tsearch_value [value] : Search value.\n"
        << "\tdelete_value [value]: Delete value.\n"
        << "\tprint_tree : Output the Tree.\n" 
        << "\tdump : Dump the Tree.\n" 
        << "\tmin_value : Print Min value.\n"
        << "\tmax_value : Print Max value.\n"
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

void tree_search(const Bin_Tree_Element *root, size_t search_el = 0)
{
    if (search_el == 0)
    {
        cout << "Enter the value to search : ";
        cin >> search_el;
    }

    if (root == NULL)
        cout << "Not found!" << endl;
    else if (root->element == search_el)
        cout << "Found : " << root->element << "\t Colour : " << root->colour << endl;
    else
    {
        if (root->element > search_el)
            tree_search(root->left, search_el);
        else
            tree_search(root->right, search_el);
    }
}

string node_name(const Bin_Tree_Element *node, const Bin_Tree_Element* parent = nullptr, const string& which_child = "")
{
    ostringstream out;
    out << '"' << node;
    if (parent)
    {
        out << ',';
        if (which_child.empty())
            out << (parent->left == node ? "left" : "right");
        else
            out << which_child;
        out << "_child_of_" << parent;
    }
    out << '"';
    return out.str();
}

void dump_node(ostream& out, string const& name, const Bin_Tree_Element *node, size_t level, map<string, bool>& in_previous_graph, map<string, bool>& in_current_graph)
{
    out << "  " << name << " [rank = " << level;
    if (in_previous_graph[name])
        out << ", color = \"grey\", fontcolor = \"grey\"";
    out << ", label = \"";
    if (node)
        out << node->element << "\", shape = box]\n";
    else
        out << "null\"]\n";
    in_current_graph[name] = true;
}

string strip_parent(string const& node_name)
{
    string result = node_name;
    size_t comma = result.find(',');
    if (comma != string::npos)
        result.resize(comma);
    return result;
}

void dump_edge(ostream& out, string const& from, string const& to, map<string, bool>& in_previous_graph, map<string, bool>& in_current_graph)
{
    string edge_name = strip_parent(from) + to;
    out << "  " << from << " -> " << to;
    if (in_previous_graph[edge_name])
        out << " [color = \"grey\"]";
    out << '\n';
    in_current_graph[edge_name] = true;
}

string dump_node_dot(ostream& out, const Bin_Tree_Element *node, size_t level, map<string, bool>& in_previous_graph, map<string, bool>& in_current_graph)
{
    string name = node_name(node, treat_moved_node_like_new and node ? node->up : nullptr);

    dump_node(out, name, node, level, in_previous_graph, in_current_graph);

    if (node)
    {
        string left_child;
        string right_child;

        if (node->left)
            left_child = dump_node_dot(out, node->left, level + 1, in_previous_graph, in_current_graph);
        else
        {
            /* Это нужно чтобы узел "null" в графе был не один общий, а свой в каждом месте. */
            left_child = node_name(nullptr, node, "left");
            dump_node(out, left_child, node->left, level + 1, in_previous_graph, in_current_graph);
        }

        if (node->right)
            right_child = dump_node_dot(out, node->right, level + 1, in_previous_graph, in_current_graph);
        else
        {
            right_child = node_name(nullptr, node, "right");
            dump_node(out, right_child, node->right, level + 1, in_previous_graph, in_current_graph);
        }

        dump_edge(out, name, left_child, in_previous_graph, in_current_graph);
        dump_edge(out, name, right_child, in_previous_graph, in_current_graph);
    }

    return name;
}

void dump_tree_dot(const Bin_Tree_Element *root)
{
    static map<string, bool> in_previous_graph;
    map<string, bool> in_current_graph;
    /*
     * Лучше:
     * 1) set, но так запись проверки проще и естественней.
     * 2) Завести по отдельному контейнеру для узлов и рёбер, но я решил не усложнять.
     * 3) Наверное, стоит обойтись без static, принимая этот контейнер снаружи или переделав это всё в класс.
     *
     * И ещё, этот метод, в паре с идентификацией узлов по их адресам в памяти,
     * работает нестабильно при освобождении-перевыделении памяти под узлы.
     * Так как новый узел может получить адрес старого (уже напечатанного и потом удалённого).
     * Тогда он будет ошибочно напечатан серым цветом, как будто он старый.
     */

    fstream out("tree.dot", fstream::out | fstream::app);
    out << "digraph {\n";
    dump_node_dot(out, root, 0, in_previous_graph, in_current_graph);
    out << "}\n\n";
    out.close();
    system("dot -Tpdf tree.dot > tree.pdf"); /* Тут используется пакет graphviz. */
    cout << "Dumped to tree.dot and tree.pdf .\n";
    /*
     * Выбрал PDF, потому что в нём на каждой странице будет по графу –
     * файл tree.dot каждый раз открывается на дозапись.
     * В картиночных форматах печатается только первый граф.
     * А в PDF можно смотреть как дерево меняется после проводимых над ним операций.
     */

    swap(in_previous_graph, in_current_graph);
}

void delete_replace(Bin_Tree_Element *del_el, Bin_Tree_Element *replace_el)
{
    if ((del_el->up->left != NULL) and (del_el->up->left->element == del_el->element))
        del_el->up->left = replace_el;
    else
        del_el->up->right = replace_el;

    if (del_el->left != NULL)
    {
        replace_el->left = del_el->left;
        del_el->left->up = replace_el;
    }

    if (del_el->right != replace_el)
    {
        if (replace_el->right != NULL)
            replace_el->right->up = replace_el->up;

        replace_el->up->left = replace_el->right;
        replace_el->right = del_el->right;

        del_el->right->up = replace_el;
    }

    replace_el->up = del_el->up;
}

void delete_element(Bin_Tree_Element *tree_el, Tree_Output *tree_func, Bin_Tree_Element *root, size_t delete_el = 0)
{
    if (delete_el == 0)
    {
        cout << "Enter the value to delete : ";
        cin >> delete_el;
    }

    if (tree_el == NULL)
        cout << "Not found!" << endl;
    else if (tree_el->element == delete_el)
    {
        Bin_Tree_Element *up_el = tree_el->up;

        if ((tree_el->left != NULL) and (tree_el->right != NULL))
        {
            if (tree_el->right->left == NULL)
                delete_replace(tree_el, tree_el->right);
            else
            {
                Bin_Tree_Element *min_el = tree_el->right;
                while (min_el->left != NULL)
                    min_el = min_el->left;

                delete_replace(tree_el, min_el);
            }
        }
        else if ((tree_el->left == NULL) and (tree_el->right == NULL)) 
        {
            if ((up_el->left != NULL) and (up_el->left->element == tree_el->element))
                tree_el->up->left = NULL;
            else
                tree_el->up->right = NULL;
        } 
        else  
        {
            if (tree_el->left != NULL)
            {
                if ((up_el->left != NULL) and (up_el->left->element == tree_el->element))
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
                if ((up_el->left != NULL) and (up_el->left->element == tree_el->element))
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

        delete(tree_el);
        recreate_tree_f(root, tree_func);
    }
    else
    {
        if (tree_el->element > delete_el)
            delete_element(tree_el->left, tree_func, root, delete_el);
        else
            delete_element(tree_el->right, tree_func, root, delete_el);
    }

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

//    test_array = {50, 20, 10, 60, 70, 30, 25, 40, 80};

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
    funcs["dump"] = [&first]() { dump_tree_dot(first); };
    funcs["min_value"] = [&first]() { select_min_value(first); };
    funcs["max_value"] = [&first]() { select_max_value(first); };
    funcs["search_value"] = [&first]() { tree_search(first); };
    funcs["delete_value"] = [&first, &tree_output_func]() { delete_element(first, tree_output_func, first); };
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
