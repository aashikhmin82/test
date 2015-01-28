#include <iostream>
#include <cstdlib>
using namespace std;

int print_out_array(int **array_to_print, int Xsz, int Ysz);
int create_ndimentional_arr(int **array_to_create, int Xsz, int Ysz);

int main() {
    int matrix1[2][2] = {{1,2},
                         {3,4}};
    int size, Xsize, Ysize;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            cout << matrix1[i][j] << "  ";
        }
        cout << "\n";
    }

    cout << "Enter the array X size: ";
    cin >> Xsize;

    cout << "Enter the array Y size: ";
    cin >> Ysize;

// Begin   one-dimensional Array
    int *matrixt = new int [Xsize];
    for (int i = 0; i < Xsize; i++) {
        matrixt[i] = rand() % (Xsize * 10);
    }

    cout << "Matrix (one-dimension) : \n";
    for (int i = 0; i < Xsize; i++) {
        cout << matrixt[i] << " ";
    }
    cout << "\n";
// END

// Begin n-dimentional Array
    cout << "\nMatrixA :\n";
    int **matrixA = new int *[Xsize];
/*    for (int i = 0; i < Xsize; i++) {
        matrixB[i] = new int[Ysize];
    }

    for (int i = 0; i < Xsize; i++) {
        for (int j = 0; j < Ysize; j++) {
            matrixB[i][j] = rand() % (Xsize * 10);
        }
    }
*/
    create_ndimentional_arr(matrixA, Xsize, Ysize);
    print_out_array(matrixA, Xsize, Ysize);

    cout << "\nMatrixB :\n";
    int **matrixB = new int *[Xsize];
    create_ndimentional_arr(matrixB, Xsize, Ysize);
    print_out_array(matrixB, Xsize, Ysize);

    exit(0);

}

int create_ndimentional_arr(int **array_to_create, int Xsz, int Ysz) {

    for (int i = 0; i < Xsz; i++) {
        array_to_create[i] = new int[Ysz];
    }

    for (int i = 0; i < Xsz; i++) {
        for (int j = 0; j < Ysz; j++) {
            array_to_create[i][j] = rand() % (Xsz * 10);
        }
    }

    return 1;
}

int print_out_array(int **array_to_print, int Xsz, int Ysz) {

    for (int j = 0; j < Ysz; j++) {
        for (int i = 0; i < Xsz; i++) {
            cout << array_to_print[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";

    return 1;
}

