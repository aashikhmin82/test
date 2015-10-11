#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

class sequence_operations
{
    private:
        struct compare_params
        {
            size_t equal_count = 0; // 0 - | ; 1 - \ ; 2 - -
            size_t check_before = 0;
        };
        vector <vector <compare_params>> compare_matrix;
        string string1 = "", string2 = "", result_string = "";

    public:
        sequence_operations (const string test_string1, const string test_string2) : string1(test_string1), string2(test_string2) 
        {
            vector <vector <compare_params>> compare_matrix_tmp (string1.size() + 1, vector <compare_params>(string2.size() + 1));

            size_t equal_count_tmp = 0;
            for (size_t i = 0; i < string1.size(); ++i)
            {
                for (size_t j = 0; j < string2.size(); ++j)
                {
                    if ((i > 0) and (j == 0)) 
                        equal_count_tmp = compare_matrix_tmp[i-1][j].equal_count;

                    if ((i > 0) and (j > 0) and (compare_matrix_tmp[i][j - 1].check_before == 0))
                        equal_count_tmp = compare_matrix_tmp[i-1][j].equal_count;

                    cout << string2[j];
                    if (string1[i] == string2[j]) 
                    {
                        cout << "\n[DEBUG Class] [" << string1[i] << "] [" << string2[j] << "]";
                        cout << " Ct: " << compare_matrix_tmp[i][j].equal_count << endl;

                        if ((i == 0) or (j == 0))
                            equal_count_tmp = 1;
                        else
                            equal_count_tmp = compare_matrix_tmp[i-1][j].equal_count + 1;

                        compare_matrix_tmp[i][j].check_before = 1;
                    }
                    else
                    {
                        size_t priv_equal_count = 0;
                        if (i > 0)
                            priv_equal_count = compare_matrix_tmp[i - 1][j].equal_count;

                        if (equal_count_tmp == priv_equal_count)
                            compare_matrix_tmp[i][j].check_before = 0;
                        else
                            compare_matrix_tmp[i][j].check_before = 2;
                    }
                    compare_matrix_tmp[i][j].equal_count = equal_count_tmp;
                }
            }
            cout << endl;
            compare_matrix = compare_matrix_tmp;
        }

        string return_common_subsequence()
        {
            cout << "0 ";
            for (size_t k = 0; k < string2.size(); ++k)
                cout << string2[k] << " ";
            cout << endl;

            for (size_t i = 0; i < string1.size(); ++i)
            {
                cout << string1[i] << " ";

                for (size_t j = 0; j < string2.size(); ++j)
                    cout << compare_matrix[i][j].equal_count << "(" << compare_matrix[i][j].check_before << ")" << " ";
                cout << endl;
            }

            cout << "\n\n";
            for (size_t i = string1.size(); i > 0; --i)
            {
                cout << string1[i - 1] << " ";

                for (size_t j = string2.size(); j > 0; --j)
                    cout << compare_matrix[i - 1][j - 1].equal_count << " ";

                cout << endl;
            }

            size_t k_row = string1.size() - 1;
            size_t k_column = string2.size() - 1;
            size_t k_row_tmp = k_row + 1;
            while (k_row_tmp > 0)
            {
                while (compare_matrix[k_row][k_column].check_before == 2)
                    --k_column;

                if (compare_matrix[k_row][k_column].check_before == 1)
                {
                    cout << "[DEBUG_PRINT] : " << string1[k_row] << endl;
                    result_string.push_back(string1[k_row]);
                    --k_column;
                }

                --k_row;
                --k_row_tmp;
            }

            reverse (result_string.begin(), result_string.end());
            cout << "Equal String : " << result_string << endl;
            cout << "String 1 : " << string1 << "  (" << result_string.size() * 100 / string1.size() << "%)" << endl;
            cout << "String 2 : " << string2 << "  (" << result_string.size() * 100 / string2.size() << "%)" << endl;

            return(result_string);
        }
};
