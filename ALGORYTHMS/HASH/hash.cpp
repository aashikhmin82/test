#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cmath>
#include <chrono>

size_t letter_size = 26;
char letter_heap[] = "abcdefghijklmnopqrstuvwxyz";
std::vector<std::string> string_array;
std::vector< std::vector<std::string> > work_hash_array;
//std::vector< std::vector<std::string> > hash_by_divis_array;

size_t hash_div(size_t str_num, size_t m);
size_t hash_multiplic(size_t str_num, size_t m);
size_t str_to_size_t (std::string string, size_t max_letters);

//Create hash 
void create_hash_div_arr(std::vector< std::vector<std::string> >& hash_by_divis_array);
void create_hash_multiplication_arr(std::vector< std::vector<std::string> >& hash_by_divis_array);

//Search
size_t ordinary_search(std::string search_string);
size_t hash_div_search(std::string search_string);
size_t hash_multiplic_search(std::string search_string);

//Output result create
typedef std::multimap<std::chrono::duration<double>, std::string> profile_t;
typedef size_t (*algorithm_t)(std::string);
void test_search_algorithm(algorithm_t algorithm, std::string name, profile_t& profile, std::string search_string);
void insert_to_timing_result(profile_t& profile, std::string sortT, std::chrono::duration<double> elapsed_seconds);

//Print final result
void print_timing_result(const profile_t& profile);

//Debug
void hash_debug_output(std::vector< std::vector<std::string> >& hash_array, std::string hash_name);

int main()
{
    size_t size;

    srand( (unsigned)time(NULL) );

    std::cout << "Enter the array size : ";
    std::cin >> size;

    //Initialize array with values
    for (size_t i = 0; i < size; i++)
    {
        std::string str_rnd;
        for (size_t j = 0; j <= (rand() % 5) + 1; j++) 
        {
            str_rnd.push_back(letter_heap[rand() % letter_size]);
        }

        string_array.push_back(str_rnd);
    }

    std::vector< std::vector<std::string> > hash_by_divis_array(size);

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
    std::vector< std::vector<std::string> > hash_by_divis_array1(size);
    create_hash_multiplication_arr(hash_by_divis_array1);

    //DEBUG
    hash_debug_output(hash_by_divis_array1, "Hash by multiplication");

    work_hash_array = hash_by_divis_array1;
    hash_by_divis_array1.clear();
    test_search_algorithm(hash_multiplic_search, "Muliplication Hash search", profile, search_string);

    work_hash_array.clear();
////////////////////////////////////////////

    print_timing_result(profile);

    return 0;
}

void hash_debug_output(std::vector< std::vector<std::string> >& hash_array, std::string hash_name)
{
    if (hash_array.size() < 30) {
//        std::cout << "Hash by multiplication : \n";
        std::cout << hash_name << " : \n";
        for (size_t i = 0; i < hash_array.size(); i++) {
            std::cout << i << " : ";
            for (size_t j = 0; j < hash_array[i].size(); ++j) {
                std::cout << hash_array[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
}


void test_search_algorithm(algorithm_t algorithm, std::string name, profile_t& profile, std::string search_string)
{
    std::chrono::time_point<std::chrono::system_clock> search_start_time, search_end_time;

    search_start_time = std::chrono::system_clock::now();
    algorithm(search_string);
    search_end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = search_end_time - search_start_time;

    insert_to_timing_result(profile, name, elapsed_seconds);
}

void insert_to_timing_result(profile_t& profile, std::string sortT, std::chrono::duration<double> elapsed_seconds)
{
            profile.insert(std::make_pair(elapsed_seconds, sortT));
}

void print_timing_result(const profile_t& profile)
{
    std::cout << "\n";
    for (profile_t::const_iterator i = profile.begin(); i != profile.end(); ++i)
        std::cout << i->first.count() << "s : " << i->second << "\n";
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

void create_hash_div_arr(std::vector< std::vector<std::string> >& hash_by_divis_array)
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
    for (size_t i = 0; i < string_array.size(); ++i) {
        if (search_string == string_array[i]) {
            std::cout << "\n";
            std::cout << "Found!\n";
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

        for (size_t j = 0; j < work_hash_array[chain_index].size(); ++j) {
            if (search_string == work_hash_array[chain_index][j]) {
                std::cout << "\n";
                std::cout << "Found in hash!\n";
                return 1;
            }
        }
        std::cout << "\n";
    
    return 0;
}

void create_hash_multiplication_arr(std::vector< std::vector<std::string> >& hash_by_divis_array)
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

    for (size_t j = 0; j < work_hash_array[chain_index].size(); ++j) {
        if (search_string == work_hash_array[chain_index][j]) {
            std::cout << "\n";
            std::cout << "Found in hash2!\n";
            return 1;
        }
    }
    std::cout << "\n";

    return 0;
}

