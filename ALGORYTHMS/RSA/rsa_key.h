#ifndef RSA_KEY_H_
#define RSA_KEY_H_

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class rsa_key
{
    public:
    vector <size_t> create_key();
    vector <size_t> count_key(size_t p, size_t q);
    size_t encrypt(size_t value, size_t e, size_t n);
    size_t decrypt(size_t value, size_t d, size_t n);

    private:
    bool is_prime(size_t check_num);
    size_t random_prime(size_t min_value, size_t max_value);
    size_t gcd(size_t a, size_t b);
    bool mutial_prime(size_t number1, size_t number2);
    size_t count_d(size_t fi, size_t e);
};

#endif /* RSA_KEY_H_ */
