#pragma once

namespace rsakey {
    class rsa_key
    {
        public:
            std::vector <size_t> create_key();
            std::vector <size_t> count_key(const size_t p, const size_t q);
            size_t encrypt(const size_t value, const size_t e, const size_t n);
            size_t decrypt(const size_t value, const size_t d, const size_t n);
    };
}
