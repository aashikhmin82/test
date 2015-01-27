#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    int i,j;
    int size;

    cout << "Enter the buffer size: ";
    cin >> size;

    int *arr1 = (int *) malloc (size * sizeof(int));
    int *arr2 = (int *) malloc (size * sizeof(int));

    for (i=0; i < size; i++) {
        arr1[i] = rand() % size;
        cout << i << " : " << arr1[i] << '\n';
    }

    cout << "Summ array : ";
    for (i = 0; i < (size - 1); i++) {
        arr2[i] = arr1[i+1] - arr1[i];
        cout << arr2[i] << " "; 
    }
    cout << "\n";

    exit(0);
}

