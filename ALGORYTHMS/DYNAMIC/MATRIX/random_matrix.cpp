#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "best_sequence.h"

using namespace std;

size_t nrand(size_t n)
{
    random_device rd;

    default_random_engine e1(rd());
    uniform_int_distribution<int> uniform_dist(1, n);
    size_t r = uniform_dist(e1);

    return r;
}

int main()
{
    size_t max_matrix = 15;
    size_t max_rows = 100;
    srand( (unsigned)time(NULL) );
    size_t rows_prev = nrand(max_rows) + 1;
    size_t columns = 0;
    size_t columns_prev = nrand(max_rows) + 1;

    vector <size_t> matrix;
    matrix.push_back(rows_prev);
    matrix.push_back(columns_prev);

    for (size_t i = 0; i <= nrand(max_matrix); ++i)
    {
        columns = nrand(max_rows) + 1;
        rows_prev = columns_prev;
        columns_prev = columns;
        matrix.push_back(columns_prev);
    }

    best_sequence matrix_test1(matrix);
    matrix_test1.min_sum();
    matrix_test1.print_matrix();

    size_t sum = matrix_test1.sum();
    size_t min_sum = matrix_test1.print_min_sum_tab();
    if (sum < min_sum)
    {
        cout << "[ERROR] min_sum is not correct." << endl;

        cout << "[ERROR] Matrix : ";
        for (auto matrix_el : matrix)
            cout << matrix_el << " ";
        cout << endl;

        exit(1);
    }

    cout << "\n\n======================================================================" << endl;
    vector <size_t> matrix2 = {30, 35, 15, 5, 10, 20, 25};
    best_sequence matrix_test2(matrix2);
    matrix_test2.min_sum();
    matrix_test2.print_matrix();
    matrix_test2.sum();
    if (matrix_test2.print_min_sum_tab() != 15125)
        cout << "[ERROR] Test Failed." << endl;

    return 0;
}
