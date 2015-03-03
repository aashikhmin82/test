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

typedef std::vector<std::string> vec_string_1d_t;
typedef std::vector< vec_string_1d_t > vec_string_2d_t;

//Hash functions
size_t hash_universal(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& un_max_str_val);

size_t hash_division(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& un_max_str_val);

size_t hash_multiplication(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& un_max_str_val);

size_t hash_linear(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val);

size_t hash_quadratic(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val);

size_t hash_double(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val);

//Convert string to size_t
size_t str_to_size_t (
        const std::string& string, 
        const size_t& max_letters);

//Output result create
typedef std::multimap<duration<double>, std::string> profile_t;
typedef size_t (*algorithm_t)(std::string);
void insert_to_timing_result(profile_t& profile, const std::string& algorithm_name, duration<double> elapsed_seconds);

//Print final result
void print_timing_result(const profile_t& profile);

//Debug
void hash_debug_output(const vec_string_2d_t& hash_array, const std::string& hash_name);
void hash_debug_output_1d(const vec_string_1d_t& hash_array, const std::string& hash_name);

typedef size_t (*hash_with_chain_func_t)(const size_t&, const size_t&, const double&, const double&, const size_t&);
typedef size_t (*hash_open_addr_func_t)(const size_t&, const vec_string_1d_t&, const std::string&, const double&, const double&, const size_t&);

//No hash
void ordinary_search_func(
        const vec_string_1d_t& string_array_c, 
        const std::string& search_string);

// Hash with chain
void create_hash_with_chain( 
        const hash_with_chain_func_t& hash_type, 
        const vec_string_1d_t& string_array, 
        vec_string_2d_t& hash_array, 
        const size_t& letter_size, 
        const double& param_a = 0, 
        const double& param_b = 0, 
        const size_t& max_str_val = 0 );

size_t search_in_hash_chain(
        const hash_with_chain_func_t& hash_type, 
        const vec_string_2d_t& hash_array, 
        const std::string& search_string, 
        const size_t& hash_letter_size, 
        const double& param_a = 0, 
        const double& param_b = 0, 
        const size_t& un_max_str_val = 0);

//Open addressing hash
void create_open_addr_hash(
        const hash_open_addr_func_t& hash_func, 
        vec_string_1d_t& open_addr_hash_array, 
        const vec_string_1d_t& string_array, 
        const size_t& letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val);

void search_in_open_addr_hash(
        const hash_open_addr_func_t& hash_func, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const size_t& letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val);

class Initial_Vars
{
    public:
        vec_string_1d_t string_array;
        std::string search_string;
        size_t letter_size;
        size_t max_str_val;
};

class Ordinary_Search
{
    private:
        vec_string_1d_t str_array_copy;

    public:
        Ordinary_Search(const vec_string_1d_t &str_array)
        {
            str_array_copy = str_array;
        }

        void debug_output()
        {
            std::cout << "[CLASS] Ordinary string : \n";
            for (const auto& arr_element : str_array_copy) {
                std::cout << arr_element << " ";
            }
            std::cout << "\n";
        }

        void ordinary_search(const std::string& search_string, const std::string& search_name, profile_t& profile)
        {
            std::cout << "[Debug] Class Ordinary search : ";
            auto search_start_time = system_clock::now();
            ordinary_search_func(str_array_copy, search_string);
            auto search_end_time = system_clock::now();
            duration<double> elapsed_seconds = search_end_time - search_start_time;

            insert_to_timing_result(profile, search_name, elapsed_seconds);
        }
};

void ordinary_search_func(const vec_string_1d_t& string_array_copy, const std::string& search_string)
{
    std::cout << "[CLASS Debug] Ordinary search : ";
    for (const auto& string_in_array : string_array_copy) {
        if (search_string == string_in_array) {
            std::cout << "\nFound!\n";
        }
    }
    std::cout << "\n";
}

class Chain_Hash
{
    protected:
        vec_string_2d_t hash_array;
        std::string description;
        hash_with_chain_func_t hash_type;
        size_t param_a = 0, 
               param_b = 0;

    public:
        void debug_output()
        {
            hash_debug_output(hash_array, description);
        }

        void create_hash_arr(const Initial_Vars &main_vars)
        {
            create_hash_with_chain(
                    hash_type, 
                    main_vars.string_array,
                    hash_array,
                    main_vars.letter_size,
                    param_a,
                    param_b,
                    main_vars.max_str_val);
        }

        void hash_search(const Initial_Vars& main_vars, const std::string& search_name, profile_t& profile)
        {
            auto search_start_time = system_clock::now();
            search_in_hash_chain(
                    hash_type, 
                    hash_array,
                    main_vars.search_string, 
                    main_vars.letter_size,
                    param_a,
                    param_b,
                    main_vars.max_str_val);
            auto search_end_time = system_clock::now();
            duration<double> elapsed_seconds = search_end_time - search_start_time;

            insert_to_timing_result(profile, search_name, elapsed_seconds);
        }
};

class Hash_by_Division : public Chain_Hash
{
    public:
        Hash_by_Division(Initial_Vars &main_vars)
        {
            description = "[CLASS NEW] Hash_by_Division";
            vec_string_2d_t division_hash_array_temp(main_vars.string_array.size());
            hash_array = division_hash_array_temp;
            division_hash_array_temp.clear();
            hash_type = hash_division;
        }
};

class Hash_by_Multiplication : public Chain_Hash
{
    public:
        Hash_by_Multiplication(Initial_Vars &main_vars)
        {
            description = "[CLASS NEW] Hash_by_Multiplication";
            vec_string_2d_t multiplication_hash_array_temp(main_vars.string_array.size());
            hash_array = multiplication_hash_array_temp;
            multiplication_hash_array_temp.clear();
            hash_type = hash_multiplication;
        }
};

class Universal_Hash : public Chain_Hash
{
    public:
        Universal_Hash(Initial_Vars &main_vars)
        {
            
            description = "[CLASS NEW] Universal_Hash";
            srand( static_cast<size_t>(time(nullptr)) );

            param_a = (rand() % (main_vars.max_str_val - 2)) + 1;
            param_b = rand() % (main_vars.max_str_val - 1);
            vec_string_2d_t universal_hash_array_temp(main_vars.string_array.size());
            hash_array = universal_hash_array_temp;
            universal_hash_array_temp.clear();
            hash_type = hash_universal;
        }
};

class Open_Address_Hash
{
    protected:
        vec_string_1d_t hash_array;
        std::string description;
        hash_open_addr_func_t hash_type;
        size_t param_a,
               param_b;

    public:
        void debug_output()
        {
            hash_debug_output_1d(hash_array, description);
        }

        void create_hash_arr(Initial_Vars &main_vars)
        {
            create_open_addr_hash(
                    hash_type, 
                    hash_array, 
                    main_vars.string_array, 
                    main_vars.letter_size, 
                    param_a, 
                    param_b, 
                    main_vars.max_str_val);
        }

        void hash_search(Initial_Vars &main_vars, std::string search_name, profile_t& profile)
        {
            auto search_start_time = system_clock::now();
            search_in_open_addr_hash(
                    hash_type, 
                    hash_array, 
                    main_vars.search_string, 
                    main_vars.letter_size, 
                    param_a, 
                    param_b, 
                    main_vars.max_str_val);
            auto search_end_time = system_clock::now();
            duration<double> elapsed_seconds = search_end_time - search_start_time;

            insert_to_timing_result(profile, search_name, elapsed_seconds);
        }
};

class Linear_Hash : public Open_Address_Hash
{
    private:
        size_t linear_hash_size_cl;

    public:
        Linear_Hash(Initial_Vars &main_vars)
        {
            description = "[CLASS NEW Debug] Linear enquire Hash";
            size_t size_log_cl = log2(main_vars.string_array.size());
            linear_hash_size_cl = pow(2, size_log_cl + 1);
            vec_string_1d_t linear_hash_tmp(linear_hash_size_cl,"0");
            hash_array = linear_hash_tmp;
            param_a = (rand() % (main_vars.max_str_val - 2)) + 1;
            param_b = rand() % (main_vars.max_str_val - 1);
            hash_type = hash_linear;
        }

};

class Quadratic_Hash : public Open_Address_Hash
{
    private:
        size_t quadratic_hash_size_cl;

    public:
        Quadratic_Hash(Initial_Vars &main_vars)
        {
            description = "[CLASS NEW Debug] Quadratic enquire Hash";
            size_t size_log_cl = log2(main_vars.string_array.size());
            quadratic_hash_size_cl = pow(2, size_log_cl + 1);
            vec_string_1d_t quadratic_hash_tmp(quadratic_hash_size_cl,"0");
            hash_array = quadratic_hash_tmp;
            param_a = (rand() % (main_vars.max_str_val - 2)) + 1;
            param_b = rand() % (main_vars.max_str_val - 1);
            hash_type = hash_quadratic;
        }

};

class Double_Hash : public Open_Address_Hash
{
    private:
        size_t double_hash_size_cl;

    public:
        Double_Hash(Initial_Vars &main_vars)
        {
            description = "[CLASS NEW Debug] Linear enquire Hash";
            size_t size_log_cl = log2(main_vars.string_array.size());
            double_hash_size_cl = pow(2, size_log_cl + 1);
            vec_string_1d_t double_hash_tmp(double_hash_size_cl,"0");
            hash_array = double_hash_tmp;
            param_a = (rand() % (main_vars.max_str_val - 2)) + 1;
            param_b = rand() % (main_vars.max_str_val - 1);
            hash_type = hash_double;
        }

};

int main()
{
    size_t letter_size = 26;
    char letter_heap[] = "abcdefghijklmnopqrstuvwxyz";
    size_t max_str_val = -1; //Dirty hack to set the max value of the site_t
//    size_t max_word_length = 5;
    size_t max_word_length = (log(max_str_val) / log(letter_size)) - 1;
    std::cout << "MAX WORD Length : " << max_word_length << "\n";
    vec_string_1d_t string_array;
    profile_t profile;

    size_t size;

//    srand( (unsigned)time(NULL) );
    srand( static_cast<size_t>(time(nullptr)) );

    std::cout << "Enter the array size : ";
    std::cin >> size;

    //Initialize array with values
    for (size_t i = 0; i < size; i++)
    {
        std::string str_rnd;
        for (size_t j = 0; j <= static_cast<size_t>(rand() % max_word_length) + 1; j++) 
        {
            str_rnd.push_back(letter_heap[rand() % letter_size]);
        }

        string_array.push_back(str_rnd);
        std::cout << str_rnd << " ";
    }
    std::cout << "\n";

    std::string search_string; 
    std::cout << "Enter the string to search : ";
    std::cin >> search_string;

//Create object with main valiables
    Initial_Vars obj_main_vars;
    obj_main_vars.string_array = string_array;
    obj_main_vars.search_string = search_string;
    obj_main_vars.letter_size = letter_size;
    obj_main_vars.max_str_val = max_str_val;

//Create vector without hash
   Ordinary_Search obj_Ordinary_array(string_array);
   obj_Ordinary_array.debug_output();
   obj_Ordinary_array.ordinary_search(search_string,  "[CLASS] Ordinary search", profile);

    //Hash array create
//Hash by division
   Hash_by_Division obj_Hash_by_Division(obj_main_vars);
   obj_Hash_by_Division.create_hash_arr(obj_main_vars);
   obj_Hash_by_Division.debug_output();

    //Search
    std::cout << "Search in Division Hash : ";
   obj_Hash_by_Division.hash_search(obj_main_vars,  "[CLASS] Division Hash search", profile);
   ////////////////////////////////////

// Multiplication
   Hash_by_Multiplication obj_Hash_by_Multiplication(obj_main_vars);
   obj_Hash_by_Multiplication.create_hash_arr(obj_main_vars);
   obj_Hash_by_Multiplication.debug_output();
    std::cout << "Search in Multiplication Hash : ";
   obj_Hash_by_Multiplication.hash_search(obj_main_vars, "[CLASS] Multiplication Hash search", profile);
   ////////////////////////////////////////////

   // Universal Hash
   Universal_Hash obj_Universal_hash(obj_main_vars);
   obj_Universal_hash.create_hash_arr(obj_main_vars);
   obj_Universal_hash.debug_output();
    std::cout << "Search in Universal Hash : ";
   obj_Universal_hash.hash_search(obj_main_vars, "[CLASS] Universal Hash search", profile);
   ///////////////////////////////////////////

   //Linear enquiry
   Linear_Hash obj_Linear_Hash(obj_main_vars);
   obj_Linear_Hash.create_hash_arr(obj_main_vars);
   obj_Linear_Hash.debug_output();
   std::cout << "Search in Linear Hash : \n";
   obj_Linear_Hash.hash_search(obj_main_vars, "[CLASS] Linear enquire Hash search", profile);
   //////////////////////////////////////////

   //Quadratic enquiry
   Quadratic_Hash obj_Quadratic_Hash(obj_main_vars);
   obj_Quadratic_Hash.create_hash_arr(obj_main_vars);
   obj_Quadratic_Hash.debug_output();
   std::cout << "Search in Quadratic Hash : \n";
   obj_Quadratic_Hash.hash_search(obj_main_vars, "[CLASS] Quadratic enquire Hash search", profile);
   //////////////////////////////////////////

   //Double enquiry
   Double_Hash obj_Double_Hash(obj_main_vars);
   obj_Double_Hash.create_hash_arr(obj_main_vars);
   obj_Double_Hash.debug_output();
   std::cout << "Search in Double Hash : \n";
   obj_Double_Hash.hash_search(obj_main_vars, "[CLASS] Double enquire Hash search", profile);
   //////////////////////////////////////////

    print_timing_result(profile);

    return 0;
}

void hash_debug_output(const vec_string_2d_t& hash_array, const std::string& hash_name)
{
    if (hash_array.size() < 30) {
        std::cout << "\n" << hash_name << " : \n";
        for (size_t i = 0; i < hash_array.size(); i++) {
            std::cout << i << " : ";
            for (const auto& hash_element : hash_array[i]) {
                std::cout << hash_element << " ";
            }
            std::cout << "\n";
        }
    }
}

void hash_debug_output_1d(const vec_string_1d_t& hash_array, const std::string& hash_name)
{
    size_t i = 0;
    if (hash_array.size() < 30) {
        std::cout << "\n" << hash_name << " : \n";
        for (const auto& hash_element : hash_array) {
            std::cout << i << " : ";
            std::cout << hash_element << "\n";
            ++i;
        }
        std::cout << "\n";
    }
}

void insert_to_timing_result(profile_t& profile, const std::string& algorithm_name, duration<double> elapsed_seconds)
{
            profile.insert(std::make_pair(elapsed_seconds, algorithm_name));
}

void print_timing_result(const profile_t& profile)
{
    std::cout << "\n";
    for (const auto& i : profile)
        std::cout << i.first.count() << "s : " << i.second << "\n";
}

size_t str_to_size_t (const std::string& string, const size_t& max_letters)
{
    size_t str_num = 0;
    size_t pow = 1;

    for ( auto it=string.rbegin(); it!=string.rend(); ++it) {
        size_t char_int = *it - 'a';
        str_num += pow * char_int;
        pow *= max_letters;
    }

    return str_num;
}

size_t hash_division(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& un_max_str_val) 
{
    size_t hash_num;

    hash_num = str_num % hash_size;

    return hash_num;
}

size_t hash_multiplication(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& un_max_str_val)
{
    float A = 0.618;
    size_t hash_num;

    hash_num = hash_size * fmod((str_num * A),1);

    return hash_num;
}

size_t hash_universal(
        const size_t& str_num, 
        const size_t& hash_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t hash_num;

    hash_num = fmod ( fmod((param_a * str_num + param_b ),max_str_val), hash_size);

    return hash_num;
}

size_t hash_linear(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t hash_num_temp, hash_num;

    hash_num_temp = fmod((param_a * str_num + param_b ),max_str_val);
    for (size_t i = 0; i < open_addr_hash_array.size(); ++i) {
        hash_num = fmod ( (hash_num_temp + i), open_addr_hash_array.size());
        if ((open_addr_hash_array[hash_num] == "0") or (open_addr_hash_array[hash_num] == search_string)) {
            return hash_num;
        } 
    }

    return hash_num;
}

size_t hash_quadratic(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t hash_num_temp, hash_num;
    double const_c1 = 0.5;
    double const_c2 = 0.5;

    hash_num_temp = fmod((param_a * str_num + param_b ),max_str_val);
    for (size_t i = 0; i < open_addr_hash_array.size(); ++i) {
        hash_num = fmod ( (hash_num_temp + const_c1 * i + const_c2 * i * i), open_addr_hash_array.size());
        if ((open_addr_hash_array[hash_num] == "0") or (open_addr_hash_array[hash_num] == search_string)) {
            return hash_num;
        } 
    }

    return hash_num;
}

size_t hash_double(
        const size_t& str_num, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t hash_num_temp, hash_num_temp1, hash_num;

    hash_num_temp = fmod(str_num, open_addr_hash_array.size());
    hash_num_temp1 = 1 + fmod(str_num, open_addr_hash_array.size() - 1);
    for (size_t i = 0; i < open_addr_hash_array.size(); ++i) {
        hash_num = fmod ( hash_num_temp + i * hash_num_temp1, open_addr_hash_array.size());
        if ((open_addr_hash_array[hash_num] == "0") or (open_addr_hash_array[hash_num] == search_string)) {
            return hash_num;
        } 
    }

    return hash_num;
}

void create_hash_with_chain(
        const hash_with_chain_func_t& hash_type, 
        const vec_string_1d_t& string_array, 
        vec_string_2d_t& hash_array, 
        const size_t& letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t str_num;
    for (const auto& current_string : string_array)
    {
        str_num = str_to_size_t(current_string, letter_size);
        hash_array[hash_type(str_num, hash_array.size(), param_a, param_b, max_str_val)].push_back(current_string);
    }
}

size_t search_in_hash_chain(
        const hash_with_chain_func_t& hash_type, 
        const vec_string_2d_t& hash_array, 
        const std::string& search_string, 
        const size_t& hash_letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t str_num = str_to_size_t(search_string, hash_letter_size);
    size_t chain_index = hash_type(str_num, hash_array.size(), param_a, param_b, max_str_val);

    for (const auto& chain_element : hash_array[chain_index]) {
        if (search_string == chain_element) {
            std::cout << "\nFound in Class hash!\n";
            return 1;
        }
    }
    std::cout << "NOT Found!\n";

    return 0;
}

void create_open_addr_hash(
        const hash_open_addr_func_t& hash_func, 
        vec_string_1d_t& open_addr_hash_array, 
        const vec_string_1d_t& string_array, 
        const size_t& letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
    size_t str_num;
    for (const auto& current_string : string_array)
    {
        str_num = str_to_size_t(current_string, letter_size);
        open_addr_hash_array[hash_func(str_num, open_addr_hash_array, "no_such_string", param_a, param_b, max_str_val)] = current_string;
    }
}

void search_in_open_addr_hash(
        const hash_open_addr_func_t& hash_func, 
        const vec_string_1d_t& open_addr_hash_array, 
        const std::string& search_string, 
        const size_t& letter_size, 
        const double& param_a, 
        const double& param_b, 
        const size_t& max_str_val)
{
//    std::cout << "[Debug Class] linear hash : ";
    size_t str_num = str_to_size_t(search_string, letter_size);
    if (open_addr_hash_array[hash_func(str_num, open_addr_hash_array, search_string, param_a, param_b, max_str_val)] == search_string) {
        std::cout << search_string << " : Found in hash!\n";
    } else {
        std::cout << "NOT found in hash.\n";
    }
}
