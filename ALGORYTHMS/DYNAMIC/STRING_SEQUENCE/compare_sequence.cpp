#include <iostream>
#include <fstream>
#include <sstream>

#include "string_sequence.h"

using namespace std;

int main(int argc, char* arg_vec[])
{
    if (argc <= 2 )
    {
        cerr << "HELP: " << endl;
        cerr << "./programm file1 file2" << endl;
        return 1;
    }

    string file_name1 = arg_vec[1], file_name2 = arg_vec[2];
    string str_el1 = "", str_el2 = "";

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
//    compare_string.longest_common_subsequence();
    compare_string.return_common_subsequence();

    return 0;
}
