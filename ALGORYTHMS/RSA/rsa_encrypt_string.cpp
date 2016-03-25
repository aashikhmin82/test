#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <sys/stat.h>

#include "rsa_key.h"

using namespace std;

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

    string test_string = "NEW rsa encrypted test string";
    cout << "String to encrypt : " << test_string << endl;

    vector <uint8_t> vec_to_encrypt = convert_to_utf8(test_string);
    vector <size_t> encrypted_string;
    cout << "Value to encrypt : ";
    for (auto val_to_encrypt8 : vec_to_encrypt)
    {
        rsa_key rsa;
        size_t val_to_encrypt = val_to_encrypt8;
        cout << val_to_encrypt << " ";
        size_t encrypted_value = rsa.encrypt(val_to_encrypt, open_keys[0], open_keys[1]);
        encrypted_string.push_back(encrypted_value);
    }
    cout << endl;

    ofstream encrypted_file;
    encrypted_file.open("encrypted_string.txt");
    cout << "Encrypted string : ";
    vector <size_t> decrypted_vec;
    for (auto encrypted_string_vl : encrypted_string)
    {
        rsa_key rsa;
        encrypted_file << encrypted_string_vl << " ";
        cout << encrypted_string_vl << " ";
        size_t decrypted_value = rsa.decrypt(encrypted_string_vl, private_keys[0], private_keys[1]);
        decrypted_vec.push_back(decrypted_value);
    }
    cout << endl;
    encrypted_file << endl;
    encrypted_file.close();

    cout << "Result String : ";
    for (auto decrypted_vec_val : decrypted_vec)
    {
        uint8_t decrypted_vec_val8 = decrypted_vec_val;
        cout << decrypted_vec_val8;
    }
    cout << endl;
}
