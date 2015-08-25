#include <iostream>
#include <vector>
#include <cassert>

#include "cut_method.h"

using namespace std;

void check_result (size_t pivot_to_cut, vector<size_t> test_result, dynamic_db pivot_obj)
{
    pivot_obj.cut(pivot_to_cut);
    vector<size_t> result = pivot_obj.print_result();
    size_t indx = 0;
    cout << "Check autotest result : " << endl;
    for (auto vec_el : result)
    {
        cout << indx << " : " << vec_el << endl;
        assert(vec_el == test_result[indx]);
        indx++;
    }
}

int main()
{
    vector <size_t> prices = {0,1,5,8,9,10,17,17,20,24,30};

    dynamic_db pivot1(prices);
    pivot1.print_price();

    cout << "Price : " << endl;
    for (auto price_val : prices)
    {
        cout << price_val << endl;
    }

//Test 1: 
    size_t pivot_to_cut1 = 7;
    vector<size_t> test_result1 = {1,6};
    check_result(pivot_to_cut1, test_result1, pivot1);
    pivot1.erase_old_pivot();
//    pivot1.cut(2);

//Test 2: 
    size_t pivot_to_cut2 = 4;
    vector<size_t> test_result2 = {2,2};
    check_result(pivot_to_cut2, test_result2, pivot1);
    pivot1.erase_old_pivot();
//    pivot1.cut(2);

    return 0;
}
