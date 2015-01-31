#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;

int print_out_array(int *array_to_print, int Xsz);
int mix_array(int *array_to_print, int Xsz);
int find_best_value(int *array_to_print, int Xsz);

int main() {
    int size, Xsize;

    cout << "Enter the array X size: ";
    cin >> Xsize;

// Begin   one-dimensional Array
    int *matrixA = new int [Xsize];
    for (int i = 0; i < Xsize; i++) {
        matrixA[i] = i;
    }


// Begin n-dimentional Array
    mix_array(matrixA, Xsize);
//    print_out_array(matrixA, Xsize);
    find_best_value(matrixA, Xsize);

    exit(0);

}

int mix_array(int *array_to_print, int Xsz) {
   
    int tmp_element, rnd_j; 
    for (int j = 0; j < Xsz; j++) {
        rnd_j = rand() % Xsz;
        tmp_element = array_to_print[rnd_j];
        array_to_print[rnd_j] = array_to_print[j];
        array_to_print[j] = tmp_element;
    }

    return 1;
}

int find_best_value(int *array_to_print, int Xsz) {

    int best_value = -1;
    int num_of_attempts = 0;   
    for (int j = 0; j < Xsz; j++) {
        if (array_to_print[j] > best_value) {
            best_value = array_to_print[j];
            num_of_attempts++;
        }
    }
    cout << "\n";
    cout << "Expected result : " << log(Xsz) << "\n";
    cout << "Actual result : " << num_of_attempts << "\n";

    return 1;
}

int print_out_array(int *array_to_print, int Xsz) {

        for (int j = 0; j < Xsz; j++) {
            cout << array_to_print[j] << " ";
        }
    cout << "\n";

    return 1;
}
