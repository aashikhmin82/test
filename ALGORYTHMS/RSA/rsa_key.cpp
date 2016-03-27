#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <random>
#include <vector>
#include <cassert>
#include <NTL/ZZ.h>

#include "rsa_key.h"

using namespace std;
using namespace NTL;

bool rsa_key::is_prime(const size_t check_num)
{
    if(check_num <= 1)
        return false;

    size_t root = sqrt((double)check_num);

    for(size_t x = 2; x <= root; x++)
    {
        if(check_num % x == 0)
            return false;
    }

    return true;
}

size_t rsa_key::random_prime(const size_t min_value, const size_t max_value)
{
    default_random_engine generator((std::random_device())());
    uniform_int_distribution<size_t> distribution(min_value,max_value);
    size_t random_prime_number = distribution(generator);

    size_t flag = 1;
    while (flag)
    {
        if (is_prime(random_prime_number))
            --flag;
        else
            random_prime_number = distribution(generator);
    }

    return random_prime_number;
}

size_t rsa_key::gcd(size_t a, size_t b)
{
    while(a > 0 && b > 0)
        if(a > b)
            a %= b;
        else
            b %= a;

    return a + b;
}

bool rsa_key::mutial_prime(const size_t number1, const size_t number2)
{
    if (gcd(number1, number2) == 1)
        return true;
    
    return false;
}

size_t rsa_key::count_d(const size_t fi, const size_t e)
{
    size_t d = 2;
    while (d < fi)
    {
        if ((d * e)%fi == 1)
            return d;
        ++d;
    }

    return 1;
}

vector <size_t> rsa_key::count_key(const size_t p, const size_t q)
{
    size_t n, fi, e;
    size_t d = 1;
    while (d == 1)
    {
        n = p * q;
        fi = (p - 1) * (q - 1);
        e = random_prime(3, fi - 1);
        while (!mutial_prime(fi, e))
        {
            e = random_prime(3, fi - 1);
        }

        d = count_d(fi,e);
    }

    vector <size_t> keys;
    keys.push_back(e); keys.push_back(d); keys.push_back(n);
    assert( (d * e) % fi == 1);

    return (keys);
}

vector <size_t> rsa_key::create_key()
{
    vector <size_t> keys;
    size_t p = random_prime(300, 1000);
    size_t q = random_prime(300, 1000);
    keys = count_key(p,q);
    return (keys);
}

size_t rsa_key::encrypt(const size_t value, const size_t e, const size_t n)
{
    ZZ big_value, big_e, big_n, zz_encrypted_value, sum_value, aa_long_big;
    big_value = value;
    big_e = e;
    big_n = n;
    zz_encrypted_value = PowerMod(big_value, big_e, big_n); // value ^ e % n

    size_t encrypted_value;
    conv(encrypted_value, zz_encrypted_value);
    return encrypted_value;
}

size_t rsa_key::decrypt(const size_t value, const size_t d, const size_t n)
{
    ZZ big_value, big_d, big_n, zz_decrypted_value;
    big_value = value; big_d = d; big_n = n;
    zz_decrypted_value = PowerMod(big_value, big_d, big_n); // value ^ d % n

    size_t decrypted_value;
    conv(decrypted_value, zz_decrypted_value);

    return decrypted_value;
}

