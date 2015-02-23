#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cmath>
#include <chrono>

using namespace std;
using std::chrono::duration;
using std::chrono::system_clock;

size_t letter_size = 26;
char letter_heap[] = "abcdefghijklmnopqrstuvwxyz";

typedef std::vector<std::string> vec_string_1d_t;
typedef std::vector< vec_string_1d_t > vec_string_2d_t;
//vec_string_1d_t string_array;
vec_string_1d_t string_array;
//vec_string_2d_t work_hash_array;
vec_string_2d_t work_hash_array;
//vec_string_2d_t hash_by_divis_array;

//Params for universal hash
size_t max_str_val, param_a, param_b;
void create_universal_hash_arr(vec_string_2d_t& hash_by_divis_array);
size_t hash_universal(size_t str_num, size_t m);

size_t hash_div(size_t str_num, size_t m);
size_t hash_multiplic(size_t str_num, size_t m);
size_t str_to_size_t (std::string string, size_t max_letters);

//Create hash 
void create_hash_div_arr(vec_string_2d_t& hash_by_divis_array);
void create_hash_multiplication_arr(vec_string_2d_t& hash_by_divis_array);

//Search
size_t ordinary_search(std::string search_string);
size_t hash_div_search(std::string search_string);
size_t hash_multiplic_search(std::string search_string);
size_t hash_universal_search(std::string search_string);

//Output result create
typedef std::multimap<duration<double>, std::string> profile_t;
typedef size_t (*algorithm_t)(std::string);
void test_search_algorithm(algorithm_t algorithm, std::string name, profile_t& profile, std::string search_string);
void insert_to_timing_result(profile_t& profile, std::string algorithm_name, duration<double> elapsed_seconds);

//Print final result
void print_timing_result(const profile_t& profile);

//Debug
void hash_debug_output(vec_string_2d_t& hash_array, std::string hash_name);

int main()
{
    size_t size;

//    srand( (unsigned)time(NULL) );
    srand( static_cast<size_t>(time(nullptr)) );

    std::cout << "Enter the array size : ";
    std::cin >> size;

    //Initialize array with values
    for (size_t i = 0; i < size; i++)
    {
        std::string str_rnd;
        for (size_t j = 0; j <= static_cast<size_t>(rand() % 5) + 1; j++) 
        {
            str_rnd.push_back(letter_heap[rand() % letter_size]);
        }

        string_array.push_back(str_rnd);
    }

//Params for Universal hash
   max_str_val = str_to_size_t ("zzzzzz", letter_size);
   param_a = rand() % (max_str_val - 1);
   param_b = (rand() % (max_str_val - 2)) + 1;

    vec_string_2d_t hash_by_divis_array(size);

    //Hash array create
//Hash by division
    create_hash_div_arr(hash_by_divis_array);

    //DEBUG
    hash_debug_output(hash_by_divis_array, "Hash by division");

    std::string search_string; 
    std::cout << "Enter the string to search : ";
    std::cin >> search_string;

    work_hash_array = hash_by_divis_array;
    hash_by_divis_array.clear();

    //Search
    profile_t profile;
    test_search_algorithm(ordinary_search, "Ordinary search", profile, search_string);
    test_search_algorithm(hash_div_search, "Div Hash search", profile, search_string);

    work_hash_array.clear();
////////////////////////////////////

// Multiplication
    vec_string_2d_t hash_by_divis_array1(size);
    create_hash_multiplication_arr(hash_by_divis_array1);

    //DEBUG
    hash_debug_output(hash_by_divis_array1, "Hash by multiplication");

    work_hash_array = hash_by_divis_array1;
    hash_by_divis_array1.clear();
    test_search_algorithm(hash_multiplic_search, "Muliplication Hash search", profile, search_string);

    work_hash_array.clear();
////////////////////////////////////////////

// Universal Hash
    vec_string_2d_t hash_by_divis_array2(size);
    create_universal_hash_arr(hash_by_divis_array2);

    //DEBUG
    hash_debug_output(hash_by_divis_array2, "Universal Hash");

    work_hash_array = hash_by_divis_array2;
    hash_by_divis_array2.clear();
    test_search_algorithm(hash_universal_search, "Universal Hash search", profile, search_string);

    work_hash_array.clear();
///////////////////////////////////////////

    print_timing_result(profile);

    return 0;
}

void hash_debug_output(vec_string_2d_t& hash_array, std::string hash_name)
{
    if (hash_array.size() < 30) {
        std::cout << "\n" << hash_name << " : \n";
        for (size_t i = 0; i < hash_array.size(); i++) {
            std::cout << i << " : ";
            for (auto hash_element : hash_array[i]) {
                std::cout << hash_element << " ";
            }
            std::cout << "\n";
        }
    }
}


void test_search_algorithm(algorithm_t algorithm, std::string name, profile_t& profile, std::string search_string)
{
    auto search_start_time = system_clock::now();
    algorithm(search_string);
    auto search_end_time = system_clock::now();
    duration<double> elapsed_seconds = search_end_time - search_start_time;

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



size_t str_to_size_t (std::string string, size_t max_letters)
{
    size_t str_num = 0;
    size_t degree = 0;

    for ( std::string::iterator it=string.end() - 1; it!=string.begin() - 1; --it) {
        size_t char_int = *it - 96;
        str_num += pow(max_letters,degree) * char_int;
        ++degree;
    }

    return str_num;
}

size_t hash_div(size_t str_num, size_t m) 
{
    size_t hash_num;

    hash_num = str_num % m;

    return hash_num;
}

void create_hash_div_arr(vec_string_2d_t& hash_by_divis_array)
{
    size_t str_num;
    for (size_t i = 0; i < hash_by_divis_array.size(); i++)
    {
        str_num = str_to_size_t(string_array[i],letter_size);
        hash_by_divis_array[hash_div(str_num,hash_by_divis_array.size())].push_back(string_array[i]);
    }
}

size_t ordinary_search(std::string search_string)
{
    std::cout << "[Debug] Ordinary search : ";
    for (auto string_in_hash_array : string_array) {
        if (search_string == string_in_hash_array) {
            std::cout << "\nFound!\n";
            return 1;
        }
    }
    std::cout << "\n";
    return 0;
}

size_t hash_div_search(std::string search_string)
{
    std::cout << "[Debug] HASH1 : ";
    size_t str_num = str_to_size_t(search_string,letter_size);
    size_t chain_index = hash_div(str_num,work_hash_array.size());

        for (auto chain_element : work_hash_array[chain_index]) {
            if (search_string == chain_element) {
                std::cout << "\nFound in hash!\n";
                return 1;
            }
        }
        std::cout << "\n";
    
    return 0;
}

void create_hash_multiplication_arr(vec_string_2d_t& hash_by_divis_array)
{
    size_t str_num;
    for (size_t i = 0; i < hash_by_divis_array.size(); i++)
    {
        str_num = str_to_size_t(string_array[i],letter_size);
        hash_by_divis_array[hash_multiplic(str_num,hash_by_divis_array.size())].push_back(string_array[i]);
    }
}

size_t hash_multiplic(size_t str_num, size_t m)
{
    float A = 0.618;
    size_t hash_num;

    hash_num = m * fmod((str_num * A),1);

    return hash_num;
}

size_t hash_multiplic_search(std::string search_string)
{
    std::cout << "[Debug] HASH2 : ";
    size_t str_num = str_to_size_t(search_string,letter_size);
    size_t chain_index = hash_multiplic(str_num,work_hash_array.size());

    for (auto chain_element : work_hash_array[chain_index]) {
        if (search_string == chain_element) {
            std::cout << "\nFound in hash2!\n";
            return 1;
        }
    }
    std::cout << "\n";

    return 0;
}


void create_universal_hash_arr(vec_string_2d_t& hash_by_divis_array)
{
    size_t str_num;
    for (size_t i = 0; i < hash_by_divis_array.size(); i++)
    {
        str_num = str_to_size_t(string_array[i],letter_size);
        hash_by_divis_array[hash_universal(str_num,hash_by_divis_array.size())].push_back(string_array[i]);
    }
}

size_t hash_universal(size_t str_num, size_t m)
{
    size_t hash_num;

    hash_num = fmod ( fmod((param_a * str_num + param_b ),max_str_val), m);

    return hash_num;
}

size_t hash_universal_search(std::string search_string)
{
    std::cout << "[Debug] HASH3 : ";
    size_t str_num = str_to_size_t(search_string,letter_size);
    size_t chain_index = hash_universal(str_num,work_hash_array.size());

    for (auto chain_element : work_hash_array[chain_index]) {
        if (search_string == chain_element) {
            std::cout << "\nFound in hash3!\n";
            return 1;
        }
    }
    std::cout << "\n";

    return 0;
}

