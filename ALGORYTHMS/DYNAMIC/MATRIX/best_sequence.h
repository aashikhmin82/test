#include <iostream>
#include <vector>
#include <cassert>
#include <limits>

using namespace std;

class best_sequence
{
    private:
        const vector <size_t> matrix;
        vector <vector <size_t>> min_sum_tab;

    public:
        best_sequence (const std::vector<size_t> input_matrix) : matrix(input_matrix) { 
            assert(input_matrix.size() >= 2);
        }

        void print_matrix() const
        {
            size_t row = matrix[0];

            for (size_t i = 1; i < matrix.size(); ++i)
            {
                cout << "[DEBUG][BS] : " << row << " * " << matrix[i] << endl;
                row = matrix[i];
            }
        }

        void min_sum()
        {
            size_t row = matrix[0];
            size_t columns = matrix[1];
//            vector <vector <size_t>> min_sum_tab_tmp(matrix.size(), vector<size_t>(matrix.size(),0));
            vector <vector <size_t>> min_sum_tab_tmp(matrix.size(), vector<size_t>(matrix.size(),numeric_limits<size_t>::max()));

            for (size_t i = 2; i < matrix.size(); ++i)
            {
                min_sum_tab_tmp[i-1][i] = row * columns * matrix[i];
                row = columns;
                columns = matrix[i];
            }

            for (size_t j = 3; j < matrix.size(); ++j)
            {
//                cout << "[DEBUG][MinSumT] Level : " << j << endl;
                for (size_t i = j; i < matrix.size(); ++i)
                {
//                    cout << "[DEBUG][MinSumT] " << i - j + 1 << " x " << i << endl;

                    size_t low_matrix = i - j + 1;
                    size_t high_matrix = i;
                    for (size_t k = low_matrix; k < high_matrix; ++k)
                    {
//                        cout << "[DEB] [" << low_matrix << "," << k << "] + [" << k + 1 << "," << high_matrix << "] + " << matrix[low_matrix - 1] << "*" << matrix[k] << "*" << matrix[high_matrix];
//                        cout << " <-> " << min_sum_tab_tmp[low_matrix][k] << " + " << min_sum_tab_tmp[k + 1][high_matrix] << " + " << matrix[low_matrix - 1] * matrix[k] * matrix[high_matrix];

                        size_t tmp_min_sum = min_sum_tab_tmp[low_matrix][k] + min_sum_tab_tmp[k + 1][high_matrix] + matrix[low_matrix - 1] * matrix[k] * matrix[high_matrix];
//                        cout << " = " << tmp_min_sum << endl;

                        min_sum_tab_tmp[low_matrix][high_matrix] = min(min_sum_tab_tmp[low_matrix][high_matrix],tmp_min_sum);
                    }
                }
            }

            min_sum_tab = min_sum_tab_tmp;
        }

        size_t print_min_sum_tab() const
        {
            for (size_t i = 2; i < matrix.size(); ++i)
            {
                cout << "[DEBUG][MinSumTab] " << i-1 << "," << i << " : " << min_sum_tab[i-1][i] << endl;
            }

            cout << "[DEBUG] min_sum_tab size : " << min_sum_tab.size() << "  MIN SUM : " << min_sum_tab[1][min_sum_tab.size() - 1 ] << endl;

            return min_sum_tab[1][min_sum_tab.size() - 1 ];
        }

        size_t sum()
        {
            size_t row = matrix[0];
            size_t columns = matrix[1];
            size_t min_sum = 0;
            
            for (size_t i = 2; i < matrix.size(); ++i)
            {
                min_sum += row * columns * matrix[i];
                columns = matrix[i];
            }

            cout << "[DEBUG] SUM : " << min_sum << endl;
            return min_sum;
        }
};
