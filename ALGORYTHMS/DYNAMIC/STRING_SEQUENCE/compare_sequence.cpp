#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <functional>
#include <memory>

#include "string_sequence.h"

using namespace std;

int main(int argc, char* arg_vec[])
{
    if (argc == 1)
    {
        cout << "HELP: " << endl;
        cout << "./programm file1 file2" << endl;
        exit(0);
    }

    string file_name1, file_name2;
    string str_el1, str_el2;
    istringstream in1(arg_vec[1]);
    in1 >> file_name1;
    istringstream in2(arg_vec[2]);
    in2 >> file_name2;

    cout << "[DEBUG] File name 1: " << file_name1 << endl;
    cout << "[DEBUG] File name 2: " << file_name2 << endl;

    ifstream config_fl1(file_name1);
    ifstream config_fl2(file_name2);

    config_fl1 >> str_el1;
    cout << "[DEBUG] String1 : " << str_el1 << endl;
    for (size_t i = 0; i < str_el1.size(); ++i)
    {
        cout << "[" << str_el1[i] << "]" << " ";
    }
    cout << endl;

    config_fl2 >> str_el2;
    cout << "[DEBUG] String1 : " << str_el2 << endl;
    for (size_t i = 0; i < str_el2.size(); ++i)
    {
        cout << "[" << str_el2[i] << "]" << " ";
    }
    cout << endl;

    config_fl1.close();
    config_fl2.close();

    sequence_operations compare_string(str_el1, str_el2);
    compare_string.compare_sequence();
    compare_string.print_mutual_string();

    return 0;
}
