#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int insertion(int *ar1, int ar_size);
int decompose(int *ar1, int ar_size);
int sort_On(int *array_to_sort, int ar_size);
int compare(const void * x1, const void * x2);

int main() 
{
    int i,j;
    int size;
    unsigned int start_time, end_time;
    srand( (unsigned)clock() );

    cout << "Enter the buffer size: ";
    cin >> size;

    int *arr1 = (int *) malloc (size * sizeof(int));
    int *arr2 = (int *) malloc (size * sizeof(int));
    int *arr3 = (int *) malloc (size * sizeof(int));
    int *arr4 = new int [size];

    for (i=0; i < size; i++) {
//        arr1[i] = rand() % size;
        arr4[i] = arr3[i] = arr2[i] = arr1[i] = rand() % size;
//        arr2[i] = arr1[i];
//        arr3[i] = arr1[i];
//        arr4[i] = arr1[i];
//        cout << arr1[i] << ' ';
    }
    cout << '\n';

    start_time = clock();
    insertion(arr1, size);
    end_time = clock();
    int insertion_time = end_time - start_time;

    start_time = clock();
    decompose(arr2, size);
    end_time = clock();
    int decompose_time = end_time - start_time;

    start_time = clock();
    sort_On(arr3, size);
    end_time = clock();
    int On_time = end_time - start_time;

    start_time = clock();
    qsort(arr4, size, sizeof(int), compare);
    end_time = clock();
    int qsort_time = end_time - start_time;

    cout << "Insertion Result : ";
    for (int k = 0; k < size; k++) {
        cout <<  arr1[k] << " ";
    }
    cout << "\n";
    cout << "Time : " << insertion_time << " (mls)\n";

    cout << "Decompose Result : ";
    for (int k = 0; k < size; k++) {
        cout <<  arr2[k] << " ";
    }
    cout << "\n";
    cout << "Time : " << decompose_time << " (mls)\n";

    cout << "O(n) sort Result : ";
    for (int k = 0; k < size; k++) {
        cout <<  arr3[k] << " ";
    }
    cout << "\n";
    cout << "Time : " << On_time << " (mls)\n";

    cout << "C++ Qsort Result : ";
    for (int k = 0; k < size; k++) {
        cout <<  arr4[k] << " ";
    }
    cout << "\n";
    cout << "Time : " << qsort_time << " (mls)\n";


    exit(0);
}

int insertion(int *ar1, int ar_size) 
{

    int k;
    int k_hand;
    int key;

    cout << "Size : " << ar_size << "\t Arr : " << ar1 << "\n";
    for (k = 0; k < ar_size; k++) {
        key = ar1[k];
        k_hand = k - 1;
        while (k_hand >= 0 & ar1[k_hand] > key) {
            ar1[k_hand + 1] = ar1[k_hand];
            k_hand = k_hand - 1;
        }
        ar1[k_hand + 1] = key;
    }

/*    cout << "Func Ar : ";
    for (k = 0; k < ar_size; k++) {
        cout <<  ar1[k] << " ";
    }

    cout << "\n";
*/
    return 1;
}

int decompose(int *ar1, int ar_size) 
{

    int new_size1 = ar_size / 2;
    int new_size2 = ar_size - new_size1;
    int nsize;
    int nj1, nj2;
    int ii = 0;

    int *narr1 = (int *) malloc ((new_size1 + 1) * sizeof(int));
    int *narr2 = (int *) malloc ((new_size2 + 1) * sizeof(int));

    for (ii = 0; ii < new_size1; ii++) {
        narr1[ii] = ar1[ii];
 //       cout << ii << "Decomp : " << narr1[ii] << "\n";
    }
    narr1[ii] = 9999;

    for (ii = 0; ii < new_size2; ii++) {
        narr2[ii] = ar1[ii + new_size1];
//        cout << ii << "Decomp2 : " << narr2[ii] << "\n";
    }
    narr2[ii] = 9999;

/*    if (new_size1 > new_size2) {
        nsize = new_size1;
    } else {
        nsize = new_size2;
    }
*/

    if (new_size1 > 1) {
//    if (ar_size > 1) {
        decompose(narr1, new_size1);
    };
    
    if (new_size2 > 1) {
//    if (ar_size > 1) {
        decompose(narr2, new_size2);
    }

/*    int i1, i2;
    cout << "\n";
    cout << "New Arrey1: ";
    for (i1 = 0; i1 <= new_size1; i1++) {
        cout << i1 << " : " << narr1[i1] << "\t";
    } */
//    i1++;
//    cout << narr1[i1] << " ";

/*    cout << "\n";
    cout << "New Arrey2: ";
    for (i2 = 0; i2 <= new_size2; i2++) {
        cout << i2 << " : " << narr2[i2] << "\t";
    }
    */
//    i2++;
//    cout << narr2[i2] << " ";

//    cout << "\n";

    nj1 = 0;
    nj2 = 0;
//    cout << "Tmp arr : ";
    for (int nj = 0; nj < ar_size; nj++) {
        if (narr1[nj1] > narr2[nj2]) {
 //           cout << "arr[" << nj << "] : " << ar1[nj] << "\t  narr2[" << nj2 << "] : " << narr2[nj2] << " < " << narr1[nj1] << "\n";
            ar1[nj] = narr2[nj2];
            nj2++;
        } else {
  //          cout << "arr[" << nj << "] : " << ar1[nj] << "\t  narr1[" << nj1 << "] : " << narr1[nj1] << " < " << narr2[nj2] << "\n";
            ar1[nj] = narr1[nj1];
            nj1++;
        }
   //     cout << ar1[nj] << " ";
    }

/*    cout << "Res_tmp : ";
    for (int nj = 0; nj < ar_size; nj++) {
            cout << ar1[nj] << " ";
    }
    cout << "\n";
    */

    return 1;
}

int sort_On(int *array_to_sort, int ar_size) 
{

    int *arrayA = new int [ar_size];
    for (int i = 0; i < ar_size; i++) {
        arrayA[i] = 0;
    }

    for (int i = 0; i < ar_size; i++) {
        int ar_index = array_to_sort[i];
        arrayA[ar_index]++;
    }

    int arr_index = 0;
    int j;
    for (int i = 0; i < ar_size; i++) {
        if (arrayA[i] != 0) {
            for (j = 0; j < arrayA[i]; j++) {
                array_to_sort[arr_index] = i;
                arr_index++;
            }
        }
    }

    return 1;
}

int compare(const void * x1, const void * x2) 
{
        return ( *(int*)x1 - *(int*)x2 );
}

