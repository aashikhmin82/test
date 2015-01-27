#include <iostream>
#include <cstdlib>
using namespace std;

//int find_max_substr(int *str_ar, int ar_size);
int find_substr(int *str_ar, int ar_size, int from_index, int to_index, int *substr_start, int *substr_end, int *substr_sum);

int main() {
    int i,j;
    int size = 17;
    int size2;
    int substr_start_index = 0;
    int substr_end_index = 0;
    int max_sum = 0;

    cout << "Enter the buffer size: ";
    cin >> size;
    
//    int *arr1 = (int *) malloc (size * sizeof(int));

//    if ( size == 17 ) {

      int  arr1[17] = { 100, 113, 110, 85, 105, 102, 86, 63, 81, 101, 94, 106, 101, 79, 94, 90, 97 };

/*    } else {

        for (i=0; i < size; i++) {
            arr1[i] = rand() % size;
            cout << i << " : " << arr1[i] << '\n';
        }
    }
*/
    int *arr2 = (int *) malloc (size * sizeof(int));

    size2 = size - 1;
    cout << "Summ array : ";
    for (i = 0; i < size2; i++) {
        arr2[i] = arr1[i+1] - arr1[i];
        cout << arr2[i] << " "; 
    }
    cout << "\n";

    find_substr(arr2, size2, 0, (size2 - 1), &substr_start_index, &substr_end_index, &max_sum);

    cout << "MID (main) substr : ";
    for (i = substr_start_index; i < (substr_end_index + 1); i++) {
        cout << arr2[i] << " ";
    }
    cout << "\n";
    cout << "MID (main) : " << max_sum << "\n";

    exit(0);
}

//int find_substr(int *str_ar, int ar_size, int from_index, int to_index) {
int find_substr(int *str_ar, int ar_size, int from_index, int to_index, int *substr_start, int *substr_end, int *substr_sum) {
    int i;
    int mid = ar_size / 2;
    int n_size1 = ar_size / 2;
    int n_size2 = ar_size - n_size1;
    int n_from_index1 = from_index;
    int n_from_index2 = from_index + n_size1;
    int n_to_index1 = to_index - n_size2;
    int n_to_index2 = to_index;
    int substr_start1 = 0, substr_start2 = 0, substr_end1 = 0, substr_end2 = 0, substr_sum1 = 0, substr_sum2 = 0;
    int left_sum, right_sum, left_index, right_index;
    int left_subsum, right_subsum;
    int left_substr, right_substr, mid_substr;
    int best_sum;
    left_sum = -9999;
    right_sum = -9999;

    if (ar_size < 3) {
        left_subsum = str_ar[from_index];
        right_subsum = str_ar[to_index];
        left_index = from_index;
        right_index = to_index;
        mid_substr = left_sum + right_sum; 

        if (left_subsum > mid_substr && right_subsum > mid_substr) {
            *substr_start = left_index;
            *substr_end = right_index;
            *substr_sum = left_subsum;
        } else if (right_subsum > mid_substr && right_subsum > mid_substr) {
            *substr_start = left_index;
            *substr_end = right_index;
            *substr_sum = right_subsum;
        } else {
            *substr_start = left_index;
            *substr_end = right_index;
            *substr_sum = mid_substr;
        }
    } else {

        int sum = 0;
        find_substr(str_ar, n_size1, n_from_index1, n_to_index1, &substr_start1, &substr_end1, &substr_sum1);
        left_subsum = substr_sum1;


        find_substr(str_ar, n_size2, n_from_index2, n_to_index2, &substr_start2, &substr_end2, &substr_sum2);
        right_subsum =  substr_sum2;


        for (i = mid; i >= 0; i--) {
            sum = sum + str_ar[i];
            if (sum > left_sum) {
                left_sum = sum;
                left_index = i;
            }
        }

        sum = 0;
        for (i = mid + 1; i < ar_size; i++) {
            sum = sum + str_ar[i];
            if (sum > right_sum) {
                right_sum = sum;
                right_index = i;
            }
        }

        left_substr = str_ar[from_index];
        right_substr = str_ar[to_index];
        mid_substr = left_sum + right_sum;

        if (left_subsum > mid_substr && right_subsum > mid_substr) {
            *substr_start = substr_start1;
            *substr_end = substr_end1;
            *substr_sum = substr_sum1;
        } else if (right_subsum > mid_substr && right_subsum > mid_substr) {
            *substr_start = substr_start2;
            *substr_end = substr_end2;
            *substr_sum = substr_sum2;
        } else {
            *substr_start = left_index;
            *substr_end = right_index;
            *substr_sum = mid_substr;
        }
    }

/*    *substr_start = left_index;
    *substr_end = right_index;
    *substr_sum = best_sum;
*/
    cout << "MID substr : ";
    for (i = left_index; i < (right_index + 1); i++) {
        cout << str_ar[i] << " ";
    }
    cout << "\n";

//    cout << "left_sum : " << left_sum << "\t Mid : " << mid_substr << "\t right_sum : " << right_sum << "\n";
//    cout << "Left : " << left_substr << "\t Mid : " << mid_substr << "\t Right : " << right_substr << "\n";

    return 0;
}


