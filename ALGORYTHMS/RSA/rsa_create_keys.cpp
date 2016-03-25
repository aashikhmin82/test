#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <sys/stat.h>

#include "rsa_key.h"

using namespace std;

void test(vector <size_t> open_key, vector <size_t> private_key)
{
    ifstream open_key_output(".rsaa/open_key_file"); 
    ifstream private_key_output(".rsaa/private_key_file");

    size_t key;
    vector <size_t> open_keys;
    while (open_key_output >> key)
    {
        open_keys.push_back(key);
        cout << "Open Key : " << key << endl;
    }
    cout << endl;

    vector <size_t> private_keys;
    while (private_key_output >> key)
    {
        private_keys.push_back(key);
        cout << "Private Key : " << key << endl;
    }
    cout << endl;

    vector <size_t> test_input = {65, 43, 22, 62, 89, 2, 43, 55};
    for (auto test_letter : test_input)
    {
        rsa_key rsa;
        cout << "I : " << test_letter << endl;
        size_t encrypted_value = rsa.encrypt(test_letter, open_key[0], open_key[1]);
        size_t decrypted_value = rsa.decrypt(encrypted_value, private_key[0], private_key[1]);
        assert(decrypted_value == test_letter);
    }
}

vector <uint8_t> convert_to_utf8(string str_el)
{
    vector <uint8_t> vec_to_encrypt;

    for (size_t i = 0; i < str_el.size(); ++i)
    {
        uint8_t sym8 = str_el[i];
        vec_to_encrypt.push_back(sym8);
    }

    return vec_to_encrypt;
}

int main(int argc, char* arg_vec[])
{
    vector <size_t> open_key;
    vector <size_t> private_key;
    size_t value = 52;

    //Fixme workaround
    size_t decrypted_value = value + 1;
    while(value != decrypted_value)
    {
        rsa_key rsa;
        vector <size_t> keys = rsa.create_key();

        size_t encryped_value = rsa.encrypt(value, keys[0], keys[2]);
        cout << "Cryped Value : " << encryped_value << endl;

        decrypted_value = rsa.decrypt(encryped_value, keys[1], keys[2]);
        cout << "DeCryped Value : " << decrypted_value << endl;

        if (value == decrypted_value)
        {
            open_key.push_back(keys[0]); open_key.push_back(keys[2]);
            private_key.push_back(keys[1]); private_key.push_back(keys[2]);
            cout << "[D] Main Open Key : " << open_key[0] << " , " << open_key[1] << endl;
            cout << "[D] Main Private Key : " << private_key[0] << " , " << private_key[1] << endl;
        }
    }

    mkdir(".rsaa",0777);
    ofstream open_key_file, private_key_file;
    open_key_file.open(".rsaa/open_key_file");
    private_key_file.open(".rsaa/private_key_file");

    open_key_file << open_key[0] << " " << open_key[1] << endl;
    private_key_file << private_key[0] << " " << private_key[1] << endl;
    open_key_file.close(); private_key_file.close();

    test(open_key, private_key);
}
