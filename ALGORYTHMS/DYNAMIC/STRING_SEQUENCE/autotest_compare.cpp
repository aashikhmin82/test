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
    //Test 1
    string str_el1 = "ABCBDAB", str_el2 = "BDCABA";

    sequence_operations compare_string(str_el1, str_el2);
    compare_string.compare_sequence();
    string result_string = compare_string.print_mutual_string();

    if (result_string == "BCBA")
    {
        cout << "Test1: OK" << endl;
    }
    else
    {
        cout << "Test1: Failed" << endl;
        exit(1);
    }

    //Test 2
    str_el1 = "THISISAUTOTEST"; str_el2 = "THISISAUTOTEST";

    sequence_operations compare_string1(str_el1, str_el2);
    compare_string1.compare_sequence();
    string result_string1 = compare_string1.print_mutual_string();

    if (result_string1 == "THISISAUTOTEST")
    {
        cout << "Test2: OK" << endl;
    }
    else
    {
        cout << "Test2: Failed" << endl;
        exit(1);
    }

    //Test 3
    str_el1 = "THISISAUTOTEST"; str_el2 = "AAAAAAATAAAHAIAAASAIAAASAAAAAAAUAATAIAAAAAAAAOAAAAATAAAEAAAAASATA";

    sequence_operations compare_string2(str_el1, str_el2);
    compare_string2.compare_sequence();
    string result_string2 = compare_string2.print_mutual_string();

    if (result_string2 == "THISISAUTOTEST")
    {
        cout << "Test3: OK" << endl;
    }
    else
    {
        cout << "Test3: Failed" << endl;
        exit(1);
    }

    return 0;
}
