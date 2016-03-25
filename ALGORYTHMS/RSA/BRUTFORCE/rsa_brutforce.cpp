#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <math.h>
#include <sys/stat.h>

#include "../rsa_key.h"

using namespace std;

int main(int argc, char* arg_vec[])
{

    ifstream open_key_output("../.rsaa/open_key_file"); 
    size_t key, e_key;
    vector <size_t> open_keys;
    open_key_output >> e_key;
    open_key_output >> key;
    open_keys.push_back(key);

    size_t find_primes = 1;
    size_t first_prime = 1;
    size_t second_prime = 1;
    while (find_primes)
    {
        first_prime += 2;
        find_primes = key % first_prime;
    }

    second_prime = key / first_prime;
    cout << "First prime : " << first_prime << endl;
    cout << "Second prime : " << second_prime << endl;
    vector <size_t> keys;

    vector <size_t> open_key;
    vector <size_t> private_key;
    size_t counted_e = 1;
    while(counted_e != e_key)
    {
        rsa_key rsa;
        keys = rsa.count_key(first_prime, second_prime);

        counted_e = keys[0];
        if (counted_e == e_key)
        {
            open_key.push_back(keys[0]); open_key.push_back(keys[2]);
            private_key.push_back(keys[1]); private_key.push_back(keys[2]);
            cout << "[D] Main Open Key : " << open_key[0] << " , " << open_key[1] << endl;
            cout << "[D] Main Private Key : " << private_key[0] << " , " << private_key[1] << endl;
        }
    }

    rsa_key rsa;
    ifstream encrypted_string("../encrypted_string.txt"); 
    size_t encrypted_letter;
    cout << "Decrypted Value : ";
    while (encrypted_string >> encrypted_letter)
    {
        size_t decrypted_value = rsa.decrypt(encrypted_letter, private_key[0], private_key[1]);
        uint8_t decrypted_vec_val8 = decrypted_value;
        cout << decrypted_vec_val8;
    }
    cout << endl;
}
