#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <bitset>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>

typedef int clockid_t;

#define CLOCK_REALTIME 0

// OS X does not have clock_gettime, use clock_get_time
int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);

    tp->tv_sec = mts.tv_sec;
    tp->tv_nsec = mts.tv_nsec;

    return 0;
}
#endif

// Охота тебе постоянно писать 'std::'?
//using namespace std;

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
int insertion(std::vector<int>& ar1);
// Стандартное название алгоритма — merge sort.
int merge_sort(std::vector<int>& ar1);
// Стандартное название алгоритма — counting sort.
//int counting_sort(int *array_to_sort, int ar_size);
int counting_sort(std::vector<int>& array_to_sort); 

//Heapsort
// Декларации max_heapify() и build_max_heap() не нужны, так как эти функции определены до их использования.
void heapsort(std::vector<int>& array_to_sort);

//Quick sort
void quick_sort (std::vector<int>& qsort_array, size_t start_index, size_t end_index); // Поправил s/int/size_t/ в опредении, но забыл поправить декларацию. (Лишнее подтверждение принципа Don't Repeat Yourself ;-)  )

//Couning sort
void counting_sort1(std::vector<int>& array_to_sort, size_t max_num);

// Простая функция проверки отсортированности массива по возрастанию.
bool check_sorted(std::vector<int>& ar1) {
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

    // Полезный флаг компилятора '-Wall' включает все предупреждения.
    // Тут он говорит, что переменная j не нужна:
    // c++ -Wall    insert_alg.cpp   -o insert_alg
    // insert_alg.cpp:90:14: warning: unused variable 'j' [-Wunused-variable]
    size_t i,j;
    size_t size;
    // Типы надо брать как в документации на функцию, которую ты используешь.
    // man 3 clock:
    // "clock_t clock(void);"
    clock_t start_time, end_time;
    // Тут наверное, имелось в виду time() (или лучше clock_gettime(), у которого разрешение выше).
    // Чтобы посеять реальное время, а не CPU-время от начала работы программы.
    // Которое слабо меняется от запуска к запуску.
//    srand( (unsigned)clock() );
    srand( (unsigned)time(NULL) );

    size_t maxN;
    struct timespec sort_start_time, sort_stop_time;

    std::cout << "Enter the buffer size: ";
    std::cin >> size;

    maxN = size;

    // malloc() — это C, в C++ используется new и new[].
    // Они запустят конструкторы (т.е. инициализируют память) в отличие от malloc().
    // Для int это не принципиально, так у него нет нетривиального конструктора.
    // Но лучше всегда писать new/new[].
    // А ещё лучше, если при выделении памяти (да и ресурсов вообще) используются классы-менеджеры ресурсов.
    // Например, vector для массива объектов или умный указатель (uniq_ptr, shared_ptr) для одиночного объекта.
    // Такой менеджер сам освободит ресурс автоматически в своём деструкторе.
    // Страуструп называет такой подход RAII (Resource Acquisition Is Initialization).
    std::vector<int> arr1(size);
    std::vector<int> arr_to_sort(size);
    std::vector<int> arr2(size);
    std::vector<int> arr3(size);
    std::vector<int> arr4(size);
//    int *arr3 = (int *) malloc (size * sizeof(int));
//    int *arr4 = new int [size];

    std::vector<int> test_array_heap(size);
    std::vector<int> test_array_quicksort(size);
    std::vector<int> test_counting_sort (size);

    for (i=0; i < size; i++) {
//        arr1[i] = rand() % size;
        arr_to_sort[i] = arr4[i] = arr3[i] = arr2[i] = arr1[i] = rand() % size;
        test_array_heap[i] = arr4[i];
        test_array_quicksort[i] = arr4[i];
        test_counting_sort[i] = arr4[i];
//        arr2[i] = arr1[i];
//        arr3[i] = arr1[i];
//        arr4[i] = arr1[i];
//        cout << arr1[i] << ' ';
    }
    std::cout << '\n';

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

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    insertion(arr1);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t insertion_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    merge_sort(arr2);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t decompose_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    counting_sort(arr3);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t On_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    // Если пишешь на C++, проще использовать std::sort.
    // У него и компаратор по умолчанию есть, свой писать не нужно.
//    qsort(arr4, size, sizeof(int), compare);
    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    std::sort(arr_to_sort.begin(), arr_to_sort.end());
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t qsort_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    heapsort(test_array_heap);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t heap_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    quick_sort(test_array_quicksort,0,test_array_quicksort.size() - 1);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t quicksort_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    counting_sort1(test_counting_sort, maxN);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);
    clock_t counting_sort1_time = sort_stop_time.tv_nsec - sort_start_time.tv_nsec;

    std::cout << "Insertion Result : ";
    for (size_t k = 0; k < size; k++) {
        std::cout <<  arr1[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(arr1)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << insertion_time << " (nsec)\n";

    std::cout << "\nDecompose Result : ";
    for (size_t k = 0; k < size; k++) {
        std::cout <<  arr2[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(arr2)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << decompose_time << " (nsec)\n";

    std::cout << "\nO(n) sort Result : ";
    for (int k = 0; k < size; k++) {
        std::cout <<  arr3[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(arr3)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << On_time << " (nsec)\n";

    std::cout << "\nC++ std::sort Result : ";
    for (int k = 0; k < size; k++) {
        std::cout <<  arr_to_sort[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(arr_to_sort)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << qsort_time << " (nsec)\n";

    std::cout << "\nHeap sort Result : ";
    for (int k = 0; k < size; k++) {
        std::cout <<  test_array_heap[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(test_array_heap)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << heap_time << " (nsec)\n";

    std::cout << "\nQuicksort Result : ";
    for (int k = 0; k < size; k++) {
        std::cout <<  test_array_quicksort[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(test_array_quicksort)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << quicksort_time << " (nsec)\n";

    std::cout << "\nCouning sort1 Result : ";
    for (int k = 0; k < size; k++) {
        std::cout <<  test_counting_sort[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(test_counting_sort)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      return 1;
    }
    std::cout << "Time : " << counting_sort1_time << " (nsec)\n";


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

int insertion(std::vector<int>& ar1) 
{

    size_t k;
    int key;

    std::cout << "Size : " << ar1.size() << "\t Arr : " << &ar1[0] << "\n";
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

int merge_sort(std::vector<int>& ar1) 
{

    size_t new_size1 = ar1.size() / 2;
    size_t new_size2 = ar1.size() - new_size1;
    size_t nsize;
    size_t nj1, nj2;
    size_t ii = 0;

    // '+ 1' был ради ненужного больше хака.
    std::vector<int> narr1(new_size1);
    std::vector<int> narr2(new_size2);

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

//int counting_sort(int *array_to_sort, int ar_size) 
int counting_sort(std::vector<int>& array_to_sort)
{
    // ar_size не удачное имя параметра, лучше max_value.
    // Но я бы вообще обошёлся без него.
    // Нужно просто динамически увеличивать arrayA с помощью метода resize().
    // Ещё более продвинутый вариант - использовать для счётчиков не вектор, а хеш-мап.
    // Но до хеш-мапа ты ещё не дошёл.

    std::vector<int> arrayA; // Лучше vector<int>.

    for (size_t i = 0; i < array_to_sort.size(); i++) {
        int ar_index = array_to_sort[i];
        if (ar_index < 0) {
            std::cerr << "Negative values are not supported in counting sort.\n";
            exit(1);
        }
        if (ar_index >= arrayA.size())
            arrayA.resize(ar_index + 1, 0);
        // Не очень красиво, что массив индексируется int'ом, а не size_t, но пока оставим так.
        arrayA[ar_index]++;
    }

    size_t arr_index = 0;
    for (size_t i = 0; i < arrayA.size(); i++) {
        if (arrayA[i] != 0) {
            for (size_t j = 0; j < arrayA[i]; j++) {
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

// ИМХО, лучше без указателей.
// Аргумент принимать по значенияю, а результат возвращать в return'е.
// Ты же не пользуешься тем что здесь указатели.
// В POSIX-интерфейсах указатели (на структуры) часто используются чтобы можно было структуру менять прозрачно для вызывающего кода.
// В частности, чтобы не нужно было его перекомпилировать.
// В C++ для этого используется идиома pointer to implementation (на эту тему тебе тоже пока не имеет смыл заморачиваться).
// Ещё в POSIX-интерфейсах указатели на выходные значения используются, т.к. возвращаемые в return значения заняты под индикаторы ошибок.
// Ну и в C++ лучше использовать ссылки там, где ты не ожидаешь nullptr.
//
// Имена to_left() и to_right() не очень информативные.
// ИМХО, left/right_child_index() в контексте кучи понятнее.
size_t left_child_index(size_t num)
{
    return (num << 1) + 1;
}

size_t right_child_index(size_t num)
{
    return (num << 1) + 2;
}

void max_heapify(std::vector<int>& ar1, size_t index, size_t heap_size)
{
    size_t left_i = left_child_index(index);
    size_t right_i = right_child_index(index);
    size_t l_largest, r_largest, largest;

    if ((right_i <= heap_size) and (ar1[right_i] > ar1[index])) {
        largest = right_i;
    } else {
        largest = index;
    }

    if ((left_i <= heap_size) and (ar1[left_i] > ar1[index])) {
        largest = left_i;
    }

    if (largest != index) {
        // Было: if ((ar1[left_i] < ar1[right_i]) and (right_i <= heap_size)){
        // Это ошибка доступа к памяти.
        // Ты сначала потенциально обратился за границу массива по индексу right_i, а потом уже сравнил этот индекс с верхней границей м ассива.
        // and и or вычисляют свои аргументы слева направо
        // И если первый аргумент позволяет дать ответ сразу, второй аргумент не вычисляется.
        // Это используется в подобных случаях, но сначала надо проверить индекс, а потом уже индексироваться.
        // (Кстати, тут неявно используется что left_i < right_i, чтобы left_i не сравнивать с heap_size.)
        if ((right_i <= heap_size) and (ar1[left_i] < ar1[right_i])){
            largest = right_i;
        }
        // Есть специальная функция std::swap().
        std::swap(ar1[index], ar1[largest]);
        max_heapify(ar1, largest, heap_size);
    }

}

void build_max_heap(std::vector<int>& array_heap)
{
    // Хитрый вариант для убывающего цикла по беззнаковой переменной (из-за желания индексировать типом size_t):
    for (size_t i = array_heap.size(); i-- > 0; ) {
        max_heapify(array_heap, i, array_heap.size() - 1);
    }
}

void heapsort(std::vector<int>& array_to_sort)
{
    // Внутри цикла array_size == i, так что можно обойтись без неё.
    build_max_heap(array_to_sort);
    for (size_t i = array_to_sort.size(); i-- > 1; ) {
        std::swap(array_to_sort[0], array_to_sort[i]);
        max_heapify(array_to_sort, 0, i - 1);

    }
}

int partition(std::vector<int>& qsort_array, size_t start_i, size_t end_i)
{
    // Каноническое название, кажется pivot.
    // TODO: Лучше бы его рандомизировать.
    int pivot = qsort_array[end_i]; // А!!! Я тут тип ошибочно менял. Он должен быть того же типа, что и элементы массива.
    // Лучше size_t.
    // Ради него надо убрать '- 1', но без неё тоже лучше, так как не придётся после цикла 3 раза (!) добавлять эту единицу.
    size_t left_i = start_i;

    for (size_t j = start_i; j < end_i; j++) {
        if (qsort_array[j] <= pivot) {
            std::swap(qsort_array[j], qsort_array[left_i]);
            ++left_i;
        }
    }

    qsort_array[end_i] = qsort_array[left_i];
    qsort_array[left_i] = pivot;

    return left_i;
}

void quick_sort (std::vector<int>& qsort_array, size_t start_index, size_t end_index)
{
    if (start_index < end_index) {
        size_t mid_index = partition(qsort_array, start_index, end_index);
        quick_sort(qsort_array, start_index, mid_index - 1);
        quick_sort(qsort_array, mid_index + 1, end_index);
    }
}

void counting_sort1(std::vector<int>& array_to_sort, size_t max_num)
{
    std::vector<int> counting_array(max_num, 0);
    std::vector<int> sorted_array(array_to_sort.size());

    // Для индексации массива лучше size_t:
    // http://www.cplusplus.com/reference/cstring/size_t/
    // http://en.wikipedia.org/wiki/C_data_types#Size_and_pointer_difference_types
    // http://www.unix.org/whitepapers/64bit.html
    //
    // Тут на русском :-) :
    // http://www.viva64.com/ru/t/0044/
    // http://www.viva64.com/ru/a/0050/
    for (size_t i = 0; i < array_to_sort.size(); i++) {
        ++counting_array[array_to_sort[i]];
    }

    int prev_count_arr_element = 0;
    for (size_t i = 0; i < counting_array.size(); i++) {
        counting_array[i] += prev_count_arr_element;
        prev_count_arr_element = counting_array[i];
    }

    for (size_t i = 0; i < array_to_sort.size(); i++) {
        sorted_array[counting_array[array_to_sort[i]] - 1] = array_to_sort[i];
        --counting_array[array_to_sort[i]];
    }

    for (size_t i = 0; i < array_to_sort.size(); i++) {
        array_to_sort[i] = sorted_array[i];
    }

}

