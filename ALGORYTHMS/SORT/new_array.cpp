#include <iostream>
#include <cstdlib>
using namespace std;

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
    int *matrixA = new int [Xsize];
    for (int i = 0; i < Xsize; i++) {
        matrixA[i] = rand() % (Xsize * 10);
    }

    cout << "MatrixA : \n";
    for (int i = 0; i < Xsize; i++) {
        cout << matrixA[i] << " ";
    }
    cout << "\n";
// END

// Begin n-dimentional Array
    cout << "\nMatrixB :\n";
    int **matrixB = new int *[Xsize];
    for (int i = 0; i < Xsize; i++) {
        matrixB[i] = new int[Ysize];
    }

    for (int i = 0; i < Xsize; i++) {
        for (int j = 0; j < Ysize; j++) {
            matrixB[i][j] = rand() % (Xsize * 10);
        }
    }

    for (int j = 0; j < Ysize; j++) {
        for (int i = 0; i < Xsize; i++) {
            cout << matrixB[i][j] << "\t";
        }
        cout << "\n";
    }


    exit(0);
}
