#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
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
void insertion(std::vector<int>& ar1);
// Стандартное название алгоритма — merge sort.
void merge_sort(std::vector<int>& ar1);
// Стандартное название алгоритма — counting sort.
//int counting_sort(int *array_to_sort, int ar_size);
void counting_sort(std::vector<int>& array_to_sort);

//Heapsort
// Декларации max_heapify() и build_max_heap() не нужны, так как эти функции определены до их использования.
void heapsort(std::vector<int>& array_to_sort);

//Quick sort
void quick_sort (std::vector<int>& qsort_array); // Поправил s/int/size_t/ в опредении, но забыл поправить декларацию. (Лишнее подтверждение принципа Don't Repeat Yourself ;-)  )

//Couning sort
void counting_sort1(std::vector<int>& array_to_sort);
void std_sort(std::vector<int>& array);

//Radix sort (counting)
void radix_sort(std::vector<int>& array_to_sort);
void radix_sort_digit(std::vector<int>& array_to_sort, int n);

//Bucket sort
void bucket_sort(std::vector<int>& sort_array);
void print_vec(std::vector<int>& array, std::vector<int>& sorted_array, size_t &ar_index);
void insert_element_to_bucket(std::vector<int>& array, int element);
void bucket_merge(std::vector< std::vector<int> >& sorted_bucket, std::vector<int>& sorted_array, size_t rows);

// Простая функция проверки отсортированности массива по возрастанию.
void sort_check(std::vector<int>& sorted_array, std::string sortT); 
bool check_sorted(std::vector<int>& ar1) {
  for (size_t i = 0; i + 1 < ar1.size(); ++i)
    if (ar1[i] > ar1[i + 1])
      return false;
  return true;
}

int compare(const void * x1, const void * x2);

typedef std::multimap<clock_t, std::string> profile_t;
// Style: лучше либо все имена писать через подчерк, либо все в CamelCase.
// Мне больше нравится первый вариант, так как в стандартной библиотеке используется он.
void insertItem(profile_t& profile, clock_t start_time, clock_t stop_time, std::string sortT);
void printNode(const profile_t& profile);

typedef void (*algorithm_t)(std::vector<int>&);
//void test_algorithm(algorithm_t algorithm, std::string name, const std::vector<int> &array, sort_time *currItem);
void test_algorithm(algorithm_t algorithm, std::string name, const std::vector<int> &array, profile_t& profile);

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

    std::cout << "Enter the buffer size: ";
    std::cin >> size;

    // malloc() — это C, в C++ используется new и new[].
    // Они запустят конструкторы (т.е. инициализируют память) в отличие от malloc().
    // Для int это не принципиально, так у него нет нетривиального конструктора.
    // Но лучше всегда писать new/new[].
    // А ещё лучше, если при выделении памяти (да и ресурсов вообще) используются классы-менеджеры ресурсов.
    // Например, vector для массива объектов или умный указатель (uniq_ptr, shared_ptr) для одиночного объекта.
    // Такой менеджер сам освободит ресурс автоматически в своём деструкторе.
    // Страуструп называет такой подход RAII (Resource Acquisition Is Initialization).
    std::vector<int> arr1(size);

    for (i=0; i < size; i++) {
        arr1[i] = rand() % size;
//        cout << arr1[i] << ' ';
    }



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

    profile_t profile;

    test_algorithm(insertion, "Insert sort", arr1, profile);
    test_algorithm(merge_sort, "Merge sort", arr1, profile);
    test_algorithm(counting_sort, "Couning sort", arr1, profile);
    test_algorithm(std_sort, "C++ std::sort", arr1, profile);
    test_algorithm(heapsort, "Heap sort", arr1, profile);
    test_algorithm(quick_sort, "Quick sort", arr1, profile);
    test_algorithm(counting_sort1, "Counting sort1", arr1, profile);
    test_algorithm(radix_sort, "Radix sort", arr1, profile);
    test_algorithm(bucket_sort, "Bucket sort", arr1, profile);

    printNode(profile);

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

void test_algorithm(algorithm_t algorithm, std::string name, const std::vector<int> &array, profile_t& profile)
{
    struct timespec sort_start_time, sort_stop_time;
    std::vector<int> array_copy = array;

    clock_gettime(CLOCK_REALTIME, &sort_start_time);
    algorithm(array_copy);
    clock_gettime(CLOCK_REALTIME, &sort_stop_time);

    sort_check(array_copy, name + " Result");

//    insertItem(currItem, sort_start_time.tv_nsec, sort_stop_time.tv_nsec, name);
    insertItem(profile, sort_start_time.tv_nsec, sort_stop_time.tv_nsec, name);
 //   currItem = currItem->next;
}

void std_sort(std::vector<int>& array)
{
    std::sort(array.begin(), array.end());
}

void insertion(std::vector<int>& ar1)
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
}

void merge_sort(std::vector<int>& ar1)
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
}

//int counting_sort(int *array_to_sort, int ar_size) 
void counting_sort(std::vector<int>& array_to_sort)
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
    size_t index = (rand() % (end_i - start_i + 1)) + start_i;
    std::swap(qsort_array[index], qsort_array[end_i]);
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
        if (mid_index > 0)
            quick_sort(qsort_array, start_index, mid_index - 1);
        quick_sort(qsort_array, mid_index + 1, end_index);
    }
}

void quick_sort (std::vector<int>& qsort_array)
{
    // Случай пустого массива раньше (без проверки) приводил к падению.
    if (qsort_array.size() > 0)
        quick_sort(qsort_array, 0, qsort_array.size() - 1);
}

void counting_sort1(std::vector<int>& array_to_sort)
{
    // Надо бы честно вычислить max_num.
    // Я просто взял его таким же, каким он передавался сюда раньше.
    int max_num = array_to_sort.size();

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

void insertItem(profile_t& profile, clock_t start_time, clock_t stop_time, std::string sortT)
{
    profile.insert(std::make_pair(stop_time - start_time, sortT));
}

void printNode(const profile_t& profile)
{
    for (profile_t::const_iterator i = profile.begin(); i != profile.end(); ++i)
        std::cout << i->first << " : " << i->second << "\n";
}

void sort_check(std::vector<int>& sorted_array, std::string sortT) 
{
    std::cout << sortT << " : ";
    for (size_t k = 0; k < sorted_array.size(); k++) {
        std::cout <<  sorted_array[k] << " ";
    }
    std::cout << "\n";
    if (check_sorted(sorted_array)) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR\n";
      exit(1);
    }
}

void radix_sort(std::vector<int>& array_to_sort)
{
    int MaxInt = 0;
    for (size_t i=0; i < array_to_sort.size(); i++) {
        if (array_to_sort[i] < 0)
        {
            std::cerr << "radix_sort does not support negative values (for now)\n";
            exit(1);
        }

        if (MaxInt < array_to_sort[i]) {
            MaxInt = array_to_sort[i];
        }
    }
    std::cout << '\n';
    std::cout << "MAX INT : " << MaxInt << "\n";

    size_t MaxDigitsNum = 0;
    while(MaxInt > 0)
    {
        MaxInt /= 10;
        ++MaxDigitsNum;
    }
    std::cout << "MAX Digigs : " << MaxDigitsNum << "\n";

    // Не уверен в использовании size_t, т.к. дальше деление int'ов
    int digit_order = 1;
    for(size_t j = 0; j < MaxDigitsNum; j++) {
        std::cout << "Dig order : " << digit_order << "\n";
        radix_sort_digit(array_to_sort, digit_order);
        digit_order *= 10;
    }
}

void radix_sort_digit(std::vector<int>& array_to_sort, int n)
{
    int digit;
    std::vector<int> counting_array(10, 0);
    std::vector<int> sorted_array(array_to_sort.size());

    for (size_t i = 0; i < array_to_sort.size(); i++) {
        int digit_tmp = array_to_sort[i] / n;
        if (digit_tmp == 0) {
            digit = 0;
        } else {
            digit = digit_tmp % 10;
        }

        ++counting_array[digit];
    }

    int prev_count_arr_element = 0;
    for (size_t i = 0; i < counting_array.size(); i++) {
        counting_array[i] += prev_count_arr_element;
        prev_count_arr_element = counting_array[i];
    }

//    for (size_t i = 0; i < array_to_sort.size(); i++) {
    size_t i = array_to_sort.size();
    while (i > 0) {
        --i;
        int digit_tmp = array_to_sort[i] / n;
        if (digit_tmp == 0) {
            digit = 0;
        } else {
            digit = digit_tmp % 10;
        }
        sorted_array[counting_array[digit] - 1] = array_to_sort[i];
        --counting_array[digit];
    }

    for (size_t i = 0; i < array_to_sort.size(); i++) {
        array_to_sort[i] = sorted_array[i];
    }

}

void bucket_sort(std::vector<int>& sort_array)
{
        const size_t ROWS = 10;
//        size_t ROWS = sort_array.size() / 10; //Maximum 10 numbers in one bucket
        size_t interval = sort_array.size() / (ROWS - 1);
        size_t interval_i;
        std::vector< std::vector<int> > bucket_array(ROWS, std::vector<int>());

        for (size_t j = 0; j < sort_array.size(); j++) {
            interval_i = sort_array[j] / interval;

            insert_element_to_bucket(bucket_array[interval_i], sort_array[j]);

        }

        bucket_merge(bucket_array, sort_array, ROWS);
}

void insert_element_to_bucket(std::vector<int>& array, int element)
{
    size_t k;
    if (array.size() == 0) {
        array.push_back(element);
    } else {
        std::vector<int>::iterator it;
        it = array.begin();
        for (k = 0; k < array.size(); k++) {
            if (array[k] >= element) {
                array.insert(it + k, element);
                break;
            }
        }
        if (k == array.size()) {
            array.push_back(element);
        }
    }
}

void bucket_merge(std::vector< std::vector<int> >& sorted_bucket, std::vector<int>& sorted_array, size_t rows)
{
    size_t ar_index = 0;
    for (size_t k = 0; k < rows; k++) {
        for (size_t j = 0; j < sorted_bucket[k].size(); ++j) {
            sorted_array[ar_index] = sorted_bucket[k][j];
            ++ar_index;
        }
    }
}

