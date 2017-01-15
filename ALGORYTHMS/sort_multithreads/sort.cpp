#include <iostream>
#include <vector>

#include "multithread_qsort.h"

using namespace std;

int main() {
    vector<size_t> test_vec { 10,4,8,2,5,1,6,7,3,21,15,17,44,40 };

    cout << "Test Vector : " << endl;
    for (const auto vec_el : test_vec) {
        cout << vec_el << " ";
    }
    cout << endl;

    size_t thread_amount = 2;
    my_multithread_sort(test_vec, thread_amount);

    cout << "Sorted Vector : " << endl;
    for (const auto vec_el : test_vec) {
        cout << vec_el << " ";
    }
    cout << endl;

    return 0;
}
