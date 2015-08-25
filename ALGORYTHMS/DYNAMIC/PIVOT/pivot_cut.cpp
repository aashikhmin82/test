#include <iostream>
#include <vector>

#include "cut_method.h"

using namespace std;

int main()
{
    vector <size_t> prices = {0,3,5,8,7,9,14,26,18,20,24,25,26,27,28,29,30,31,32,33,34,35,36};
//    size_t pivot_to_cut = 18;
//    size_t pivot_to_cut = 9;
    size_t pivot_to_cut = 16;

    dynamic_db pivot1(prices);
    pivot1.print_price();

    cout << "Price : " << endl;
    for (auto price_val : prices)
    {
        cout << price_val << endl;
    }
//    size_t max_price = pivot1.cut_long(6);
//    cout << "MAX VALUE : " << max_price << endl;
//    pivot1.print_scheme();
    //pivot1.cut(7);
//    pivot1.cut(1);
    pivot1.cut(pivot_to_cut);
    pivot1.print_result();
//    pivot1.debug_print_db();
//    pivot1.cut(2);

    return 0;
}
