#include <iostream>
#include <cstdlib>
using namespace std;

//int find_max_substr(int *str_ar, int ar_size);
int find_substr(int *str_ar, int ar_size, int from_index, int to_index);

int main() {
    int i,j;
    int size;
    int size2;

    cout << "Enter the buffer size: ";
    cin >> size;

    int *arr1 = (int *) malloc (size * sizeof(int));
    int *arr2 = (int *) malloc (size * sizeof(int));

    for (i=0; i < size; i++) {
        arr1[i] = rand() % size;
        cout << i << " : " << arr1[i] << '\n';
    }

    size2 = size - 1;
    cout << "Summ array : ";
    for (i = 0; i < size2; i++) {
        arr2[i] = arr1[i+1] - arr1[i];
        cout << arr2[i] << " "; 
    }
    cout << "\n";

    find_substr(arr2, size2, 0, (size2 - 1));

    exit(0);
}

int find_substr(int *str_ar, int ar_size, int from_index, int to_index) {
    int i;
    int mid = ar_size / 2;
    int left_sum, right_sum, left_index, right_index;
    int left_substr, right_substr, mid_substr;
    left_sum = -9999;
    right_sum = -9999;

    int sum = 0;
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

    cout << "MID substr : ";
    for (i = left_index; i < (right_index + 1); i++) {
        cout << str_ar[i] << " ";
    }
    cout << "\n";

    cout << "left_sum : " << left_sum << "\t Mid : " << mid_substr << "\t right_sum : " << right_sum << "\n";
    cout << "Left : " << left_substr << "\t Mid : " << mid_substr << "\t Right : " << right_substr << "\n";

    return 0;
}


