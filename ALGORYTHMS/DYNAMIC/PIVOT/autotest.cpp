#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <ctime>

#include "cut_method.h"

using namespace std;

void check_result (size_t pivot_to_cut, vector<size_t> test_result, dynamic_db pivot_obj)
{
    pivot_obj.cut(pivot_to_cut);
    vector<size_t> result = pivot_obj.print_result();
    size_t index = 0;
    cout << "Check autotest result : " << endl;
    for (auto vec_el : result)
    {
        cout << index << " : " << vec_el << endl;
        assert(vec_el == test_result[index]);
        index++;
    }
}

void check_rand_result (size_t pivot_to_cut, dynamic_db pivot_obj)
{
    pivot_obj.cut(pivot_to_cut);
    vector<size_t> result = pivot_obj.print_result();
    size_t index = 0;
    size_t pivot_res = 0;
    cout << "Check Rand autotest result : " << endl;
    for (const auto vec_el : result)
    {
        cout << index << " : " << vec_el << endl;
        pivot_res = pivot_res + vec_el;
//        assert(vec_el == test_result[index]);
        index++;
    }

    assert(pivot_to_cut == pivot_res);
    cout << "Pivot Result : " << pivot_res << endl;
}

size_t nrand(size_t n)
{
    const size_t bucket_size = RAND_MAX / n;
    size_t r = 0;

    srand( (unsigned)time(NULL) );
    do r = rand() / bucket_size;
    while(r >= n);

    return r;
}

int main()
{
    vector <size_t> prices = {0,1,5,8,9,10,17,17,20,24,30};

    dynamic_db pivot1(prices);
    pivot1.print_price();

    cout << "Price : " << endl;
    for (const auto price_val : prices)
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

    prices = {0,1,3,5,8,9,10,17,17,20,24,30,26,27,28,29,30,31,32,33,34,35,36};
//    size_t rand_size = nrand(22);
    size_t rand_size = nrand(prices.size());
    cout << "RAND SIZE : " << rand_size << endl;


//Test 3: 
    size_t pivot_to_cut3 = rand_size;
//    vector<size_t> test_result2 = {2,2};
    check_rand_result(pivot_to_cut3, pivot1);
    pivot1.erase_old_pivot();

    return 0;
}
