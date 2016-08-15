#include "boost/program_options.hpp"
#include <cassert>
#include <fstream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <vector>

#include "rsa_key.h"
#include "debug.h"

using namespace rsakey;
using namespace std;
namespace po = boost::program_options;

bool process_command_line(int argc, char ** argv, bool& debug_flag)
{
    po::options_description desc("Options");
    string file_name;
    desc.add_options()
        ("help,h", "Show help")
        ("debug,d", "Enable debug")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return false;
    }

    if (vm.count("debug"))
    {
        cout << "-d found: DEBUG is ON" << endl;
        debug_flag = true;
    }

    return true;
}


template<typename keys_list_t, typename debug_t>
bool test(const keys_list_t& open_key, const keys_list_t& private_key, debug_t& debug)
{
    ifstream open_key_output(".rsaa/open_key_file"); 
    ifstream private_key_output(".rsaa/private_key_file");

    size_t key;
    vector <size_t> open_keys;
    while (open_key_output >> key)
    {
        open_keys.push_back(key);
        debug << "Open Key : " << key << "\n";
    }
    debug << "\n";

    vector <size_t> private_keys;
    while (private_key_output >> key)
    {
        private_keys.push_back(key);
        debug << "Private Key : " << key << "\n";
    }
    debug << "\n";

    vector <size_t> test_input = {65, 43, 22, 62, 89, 2, 43, 55};
    for (auto test_letter : test_input)
    {
        rsa_key rsa;
        debug << "I : " << test_letter << "\n";
        size_t encrypted_value = rsa.encrypt(test_letter, open_key[0], open_key[1]);
        size_t decrypted_value = rsa.decrypt(encrypted_value, private_key[0], private_key[1]);
        if (decrypted_value != test_letter)
            return false;
    }

    return true;
}

template<typename keys_list_t, typename debug_t>
void create_keys(keys_list_t& open_key, keys_list_t& private_key, debug_t& debug)
{
    size_t test_value {52};
    auto test_decrypted_value = test_value + 1;

    while(test_value != test_decrypted_value)
    {
        rsa_key rsa;
        vector <size_t> keys = rsa.create_key();

        auto encryped_value = rsa.encrypt(test_value, keys[0], keys[2]);
        debug << "Cryped Value : " << encryped_value << "\n";

        test_decrypted_value = rsa.decrypt(encryped_value, keys[1], keys[2]);
        debug << "DeCryped Value : " << test_decrypted_value << "\n";

        if (test_value == test_decrypted_value)
        {
            open_key.push_back(keys[0]); open_key.push_back(keys[2]);
            private_key.push_back(keys[1]); private_key.push_back(keys[2]);

            debug << "Main Open Key : " << open_key[0] << " , " << open_key[1] << "\n";
            debug << "Main Private Key : " << private_key[0] << " , " << private_key[1] << "\n";
        }
    }

}

int main(int argc, char** argv)
{
    bool debug_flag {false};

    try
    {
        process_command_line(argc, argv, debug_flag);
    }
    catch(std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown error!" << "\n";
        return false;
    }

    print_debug debug(debug_flag);

    vector <size_t> open_key;
    vector <size_t> private_key;
    create_keys(open_key, private_key, debug);

    mkdir(".rsaa",0700);
    ofstream open_key_file, private_key_file;
    open_key_file.open(".rsaa/open_key_file");
    private_key_file.open(".rsaa/private_key_file");

    open_key_file << open_key[0] << " " << open_key[1] << endl;
    private_key_file << private_key[0] << " " << private_key[1] << endl;
    open_key_file.close(); private_key_file.close();

    cout << "Test (check encryption - decryption) : ";
    test(open_key, private_key, debug) ? cout << "Test Passed\n" : cout << "Test Failed\n";
}
