#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <chrono>
#include <random>
#include <algorithm>
#include <iterator>

using namespace std;
using std::chrono::duration;
using std::chrono::system_clock;

//g++ -std=c++11 select.cpp -o <some_file>

typedef std::vector<size_t> vector_t;

//Select min and max values
void select_max_min (const vector_t& statistics);

//Random select i-value
void randomized_select(vector_t& statistics, size_t start_i, size_t end_i, size_t search_i);
size_t partition(vector_t& qsort_array, size_t start_i, size_t end_i, size_t index = std::numeric_limits<size_t>::max());

//Select i-value
void select_by_index(vector_t& statistics, size_t start_i, size_t end_i, size_t search_i);
size_t median_array_search(const vector_t& statistics, vector_t& median_array_index);
void insert_sort(const vector_t& statistics, vector_t& array_to_insert_index, size_t index);

//Output result create
typedef std::multimap<std::chrono::duration<double>, std::string> profile_t;
typedef void (*algorithm_t)(vector_t&, size_t, size_t, size_t);
void test_select_algorithm(algorithm_t algorithm, std::string name, const vector_t &array, profile_t& profile, size_t search_index);
void insert_to_timing_result(profile_t& profile, std::string algorithm_name, duration<double> elapsed_seconds);

//Print final result
void print_timing_result(const profile_t& profile);

int main() 
{
    size_t size;
    size_t search_index;

    srand( static_cast<int>(time(nullptr)) );

    std::cout << "Enter the array size : ";
    std::cin >> size;

//Initialize array with values
    vector_t test_array (size);
    for (size_t i = 0; i < test_array.size(); i++) 
    {
        test_array[i] = i;
    }

//Random mixing of the array
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(test_array.begin(), test_array.end(), g);

//Output of the array
    std::cout << "Test Array : ";
    for (auto test_array_value : test_array) 
    {
        std::cout << test_array_value << " ";
    }
    std::cout << "\n";

    select_max_min(test_array);

    std::cout << "\nEnter the index to search : ";
    std::cin >> search_index;

    profile_t profile;
    test_select_algorithm(randomized_select, "Randomized select", test_array, profile, search_index);
    test_select_algorithm(select_by_index, "Select", test_array, profile, search_index);

    print_timing_result(profile);

    return 0;
}

void test_select_algorithm(algorithm_t algorithm, std::string name, const vector_t &array, profile_t& profile, size_t search_index)
{
    vector_t array_copy = array;

    auto select_start_time = system_clock::now();
    algorithm(array_copy, 0, array_copy.size() - 1, search_index);
    auto select_end_time = system_clock::now();
    duration<double> elapsed_seconds = select_end_time - select_start_time;

    insert_to_timing_result(profile, name, elapsed_seconds);
}

void insert_to_timing_result(profile_t& profile, std::string algorithm_name, duration<double> elapsed_seconds)
{
        profile.insert(std::make_pair(elapsed_seconds, algorithm_name));
}

void print_timing_result(const profile_t& profile)
{
    std::cout << "\n"; 
    for (const auto& i : profile)
        std::cout << i.first.count() << "s : " << i.second << "\n";
}

void select_max_min (const vector_t& statistics)
{
    size_t max_val_index, min_val_index;
    size_t start_i;
    size_t even_odd = statistics.size() % 2;

    if (even_odd == 1) {
        max_val_index = 0; min_val_index = 0;
        start_i = 1;
    } else {
        start_i = 2;
        if (statistics[0] > statistics[1]) {
            max_val_index = 0; min_val_index = 1;
        } else {
            max_val_index = 1; min_val_index = 0;
        }
    }

    for (size_t i = start_i; i < statistics.size(); i+=2) {
        size_t j = i, k = i + 1;
        if (statistics[j] < statistics[k]) {
            std::swap(j,k);
        }
            if (statistics[j] > statistics[max_val_index]) {
                max_val_index = j;
            }
            if (statistics[k] < statistics[min_val_index]) {
                min_val_index = k;
            }
    }

    std::cout << "Min value : " << statistics[min_val_index] << "\tIndex : " << min_val_index << "\n";
    std::cout << "Max value : " << statistics[max_val_index] << "\tIndex : " << max_val_index << "\n";
}

void randomized_select(vector_t& statistics, size_t start_i, size_t end_i, size_t search_i)
{
    size_t divis_i;
    
    divis_i = partition(statistics,start_i, end_i);
    if (start_i == end_i or search_i == divis_i) {
        std::cout << "Value : " << statistics[divis_i] << "\n";
    } else if (search_i < divis_i) {
        randomized_select(statistics, start_i, divis_i, search_i);
    } else {
        randomized_select(statistics, divis_i, end_i, search_i);
    }

}

size_t partition(vector_t& qsort_array, size_t start_i, size_t end_i, size_t index)
{
    if (index == std::numeric_limits<size_t>::max())
        index = (rand() % (end_i - start_i + 1)) + start_i;

    std::swap(qsort_array[index], qsort_array[end_i]);
    size_t pivot = qsort_array[end_i];
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

void select_by_index(vector_t& statistics, size_t start_i, size_t end_i, size_t search_i)
{
    size_t pivot_median;

    vector_t median_array_index;

    for (size_t i = start_i; i <= end_i; i++) {
        median_array_index.push_back(i);
    }

    median_array_search(statistics, median_array_index);

    pivot_median = partition(statistics,start_i, end_i, median_array_index[0]);

    if (pivot_median == search_i) {
        std::cout << "Found! Search i : " << search_i << "  -> Value : " << statistics[search_i] << "\n";

    } else if (pivot_median < search_i) {
        select_by_index(statistics, pivot_median, end_i, search_i);
    } else {
        select_by_index(statistics, start_i, pivot_median, search_i);
    }

}

size_t median_array_search(const vector_t& statistics, vector_t& median_array_index) 
{
    if (median_array_index.size() == 1) {
        return 1;
    }
    size_t j = 0;
    vector_t array_for_five_index;
    vector_t median_array_index_tmp;

    for (size_t i = 0; i < median_array_index.size(); i++) {
        insert_sort(statistics, array_for_five_index, median_array_index[i]);

        if (array_for_five_index.size() == 5) {
            median_array_index_tmp.push_back(array_for_five_index[2]);
            ++j;
            array_for_five_index.clear();
        }
    }

    if (array_for_five_index.size() != 0) {
        if (array_for_five_index.size() == 1) {
            median_array_index_tmp.push_back(array_for_five_index[0]);
        } else {
            median_array_index_tmp.push_back(array_for_five_index[(array_for_five_index.size() / 2) - 1 + (array_for_five_index.size() % 2)]);
        }
    }
    median_array_index = median_array_index_tmp;

    median_array_search(statistics, median_array_index); 

    return 1;

}

void insert_sort(const vector_t& statistics, vector_t& array_to_insert_index, size_t index)
{
    size_t k;
    if (array_to_insert_index.size() == 0) {
        array_to_insert_index.push_back(index);
    } else {
        for (k = 0; k < array_to_insert_index.size(); k++) {
            if (statistics[array_to_insert_index[k]] >= statistics[index]) {
                array_to_insert_index.insert(array_to_insert_index.begin() + k, index);
                break;
            }
        }
        if (k == array_to_insert_index.size()) {
            array_to_insert_index.push_back(index);
        }
    }
}
