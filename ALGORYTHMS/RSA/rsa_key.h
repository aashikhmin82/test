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
    vector <size_t> count_key(const size_t p, const size_t q);
    size_t encrypt(const size_t value, const size_t e, const size_t n);
    size_t decrypt(const size_t value, const size_t d, const size_t n);

    private:
    bool is_prime(const size_t check_num);
    size_t random_prime(const size_t min_value, const size_t max_value);
    size_t gcd(const size_t a, const size_t b);
    bool mutial_prime(const size_t number1, const size_t number2);
    size_t count_d(const size_t fi, const size_t e);
};

#endif /* RSA_KEY_H_ */
