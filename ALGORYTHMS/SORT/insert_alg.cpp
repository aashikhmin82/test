#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// Лучше использовать vector.
// У него 2 преимущества:
// 1. Он автоматически следит за выделенной памятью (реаллоцирует и удаляет).
//    Поэтому меньше возможностей ошибиться, так как от тебя требуется меньше ручных действий.
//    И память не утечёт.
// 2. Плюс, ты передаёшь две связанные между собой переменные:
//    указатель на блок памяти и размер этого блока.
//    По сути это единый объект (как оно сделано в vector).
//    В виде единого объекта они устойчивее к ошибкам.
//    Передавая по отдельности можно случайно передать размер не того блока.
//    Или, например, постоянная ошибка при вызове memset() — перепутать размер блока и символ, которым этот блок надо заполнить.
//    С единым объектом этого не произойдёт.
int insertion(vector<int>& ar1);
// Стандартное название алгоритма — merge sort.
int merge_sort(vector<int>& ar1);
// Стандартное название алгоритма — counting sort.
int counting_sort(int *array_to_sort, int ar_size);

// Простая функция проверки отсортированности массива по возрастанию.
bool check_sorted(vector<int>& ar1) {
  for (size_t i = 0; i + 1 < ar1.size(); ++i)
    if (ar1[i] > ar1[i + 1])
      return false;
  return true;
}

int compare(const void * x1, const void * x2);

int main() 
{
    // Для размеров того, что влезает в память, используется тип size_t (и в C и в C++).
    // На 64-битной машине int может быть 32-х битным, а вот size_t обязан быть 64-х битным.
    // Т.е. если у тебя будет массив байтов > 4GiB (что не редкость в базах данных), его размер может не поместиться в int, но гарантированно поместится в size_t.
    // Есть ещё два варианта указать правильный тип.
    // Оба исходят из идеи, что тип должен быть в точности таким, какой возвращает vector::size().
    // 1. Вариант, работающий в любом C++, — вектор имеет определение типа vector::size_type.
    //    Можно написать "vector::size_type size = ar1.size();".
    // 2. Для удобства, и чтобы не замусоривать код и не отвлекать внимание на конкретный тип, в C++11 (стандарт C++, принятый в 2011 году) сделали автоматическое выведение типа.
    //    Можно написать: "auto size = ar1.size();" и тип выражения справа будет определять тип переменной.
    //    Только компилировать нужно "gcc -std=c++11 ...".
    size_t i,j;
    size_t size;
    // Типы надо брать как в документации на функцию, которую ты используешь.
    // man 3 clock:
    // "clock_t clock(void);"
    clock_t start_time, end_time;
    // Тут наверное, имелось в виду time() (или лучше clock_gettime(), у которого разрешение выше).
    // Чтобы посеять реальное время, а не CPU-время от начала работы программы.
    // Которое слабо меняется от запуска к запуску.
    srand( (unsigned)clock() );

    cout << "Enter the buffer size: ";
    cin >> size;

    // malloc() — это C, в C++ используется new и new[].
    // Они запустят конструкторы (т.е. инициализируют память) в отличие от malloc().
    // Для int это не принципиально, так у него нет нетривиального конструктора.
    // Но лучше всегда писать new/new[].
    // А ещё лучше, если при выделении памяти (да и ресурсов вообще) используются классы-менеджеры ресурсов.
    // Например, vector для массива объектов или умный указатель (uniq_ptr, shared_ptr) для одиночного объекта.
    // Такой менеджер сам освободит ресурс автоматически в своём деструкторе.
    // Страуструп называет такой подход RAII (Resource Acquisition Is Initialization).
    vector<int> arr1(size);
    vector<int> arr2(size);
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

    // Если пользоваться низкоуровневыми функциями, то лучше использовать современный clock_gettime().
    // С CLOCK_MONOTONIC для измерения интервалов реального времени и CLOCK_PROCESS_CPUTIME_ID/CLOCK_THREAD_CPUTIME_ID для измерения потребления CPU.
    // clock() стоит использовать только если ты хочешь переносимость со старыми системами и для только если измеряется небольшой интервал.
    // man 3 clock:
    // "
    // Note  that  the  time  can  wrap  around.   On  a  32-bit   system   where
    // CLOCKS_PER_SEC  equals  1000000  this  function will return the same value
    // approximately every 72 minutes.
    // "
    //
    // А в C++11 лучше всего использовать std::chrono: http://en.cppreference.com/w/cpp/chrono .
    start_time = clock();
    insertion(arr1);
    end_time = clock();
    clock_t insertion_time = end_time - start_time;

    start_time = clock();
    merge_sort(arr2);
    end_time = clock();
    clock_t decompose_time = end_time - start_time;

    start_time = clock();
    counting_sort(arr3, size);
    end_time = clock();
    clock_t On_time = end_time - start_time;

    start_time = clock();
    // Если пишешь на C++, проще использовать std::sort.
    // У него и компаратор по умолчанию есть, свой писать не нужно.
    qsort(arr4, size, sizeof(int), compare);
    end_time = clock();
    clock_t qsort_time = end_time - start_time;

    cout << "Insertion Result : ";
    for (size_t k = 0; k < size; k++) {
        cout <<  arr1[k] << " ";
    }
    cout << "\n";
    if (check_sorted(arr1)) {
      cout << "OK\n";
    } else {
      cout << "ERROR\n";
      return 1;
    }
    cout << "Time : " << insertion_time << " (mls)\n";

    cout << "Decompose Result : ";
    for (size_t k = 0; k < size; k++) {
        cout <<  arr2[k] << " ";
    }
    cout << "\n";
    if (check_sorted(arr2)) {
      cout << "OK\n";
    } else {
      cout << "ERROR\n";
      return 1;
    }
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


    // Скорее стилистическая правка.
    // return из main() — это то же самое, что и exit(0), но не акцентирует на себе внимание.
    // exit() работает из любой точки программы, поэтому используется для того чтобы грубо оборвать выполнение в любой момент.
    // Видя exit, начинаешь подозревать, что это именно обрыв исполнения.
    return 0;

    // Здесь утекла бы память, выделенная через malloc().
    // В конце программы это не принципиально, но всё равно выглядит неряшливо.
    // Если бы память выделялась через new, она бы тоже утекла.
    // А вот vector и умные указатели (uniq_ptr, shared_ptr) сами освободят память при выходе из блока (фигурных скобок), где они были определены.
}

int insertion(vector<int>& ar1) 
{

    size_t k;
    int key;

    cout << "Size : " << ar1.size() << "\t Arr : " << &ar1[0] << "\n";
    for (k = 0; k < ar1.size(); k++) {
        key = ar1[k];
        // Было написано:
        // while (... >= 0 & ... > key)
        // Должно быть && (логическое И), а не & (побитовое И).
        // Это очень коварная опечатка.
        // & может давать во многих случаях тот же результат, что и &&.
        // (Когда в обоих булевых переменных true представлено единицей.)
        // Но иногда, в случайные моменты, выдавать другой результат.
        // (Например, в одном bool true представлено единицей, а в другом двойкой — true это любое ненулевое значение. Тогда a && b == true, в то время как a & b == false.)
        // Такую ошибку очень сложно поймать, в том числе из-за того что & и && визуально похожи.
        // Поэтому я стараюсь не писать && вообще, в C++ есть эквивалентное ключевое слово для булевого И — and.
        // (И, для единообразия or вместо || и not вместо !.)
        //
        // Лучше for, так как в этом случае есть и стартовое значение индекса, и условие, и модификация индекса в конце итерации цикла.
        // Ещё я уменьшил время жизни k_hand до минимально необходимого, перенеся её определение сюда.
        size_t k_hand;
        for (k_hand = k; k_hand > 0 and ar1[k_hand - 1] > key; --k_hand) {
            ar1[k_hand] = ar1[k_hand - 1];
        }
        ar1[k_hand] = key;
    }

/*    cout << "Func Ar : ";
    for (k = 0; k < ar1.size(); k++) {
        cout <<  ar1[k] << " ";
    }

    cout << "\n";
*/

    // Вроде не пользуешься возвращаемым значением, да и функция всегда возвращает 1.
    // Имеет смысл выкинуть return'ы и объявить что функция возвращает void.
    return 1;
}

int merge_sort(vector<int>& ar1) 
{

    size_t new_size1 = ar1.size() / 2;
    size_t new_size2 = ar1.size() - new_size1;
    size_t nsize;
    size_t nj1, nj2;
    size_t ii = 0;

    // '+ 1' был ради ненужного больше хака.
    vector<int> narr1(new_size1);
    vector<int> narr2(new_size2);

    for (ii = 0; ii < new_size1; ii++) {
        narr1[ii] = ar1[ii];
 //       cout << ii << "Decomp : " << narr1[ii] << "\n";
    }
    // Это какой-то грязный хак!
    // К тому же, он не работает если есть элементы > 9999.
    // Существует numeric_limits<int>::max(), который можно было бы использовать.
    // Но такие приёмы вообще не стоит применять.
    // Вместо него я просто добавил пару условий при слиянии:
    // if (nj2 < narr2.size() and (nj1 >= narr1.size() or narr1[nj1] > narr2[nj2]))
    //narr1[ii] = 9999;

    for (ii = 0; ii < new_size2; ii++) {
        narr2[ii] = ar1[ii + new_size1];
//        cout << ii << "Decomp2 : " << narr2[ii] << "\n";
    }
    //narr2[ii] = 9999;

/*    if (new_size1 > new_size2) {
        nsize = new_size1;
    } else {
        nsize = new_size2;
    }
*/

    if (new_size1 > 1) {
//    if (ar1.size() > 1) {
        merge_sort(narr1);
    };
    
    if (new_size2 > 1) {
//    if (ar1.size() > 1) {
        merge_sort(narr2);
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
    for (size_t nj = 0; nj < ar1.size(); nj++) {
        if (nj2 < narr2.size() and (nj1 >= narr1.size() or narr1[nj1] > narr2[nj2])) {
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
    for (int nj = 0; nj < ar1.size(); nj++) {
            cout << ar1[nj] << " ";
    }
    cout << "\n";
    */

    return 1;
}

int counting_sort(int *array_to_sort, int ar_size) 
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

