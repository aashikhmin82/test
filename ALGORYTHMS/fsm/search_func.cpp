#include <regex>
#include <map>
#include <chrono>
#include <thread>

#include "fsm.h"

using namespace std;

bool fsm_check_match(const string& heystack, const string& needle) {

    if (needle == "" )
        return true;

    const size_t search_str_len = needle.size();
    if (search_str_len > heystack.size())
        return false;

    fsm fsm_search { needle };
    auto fsm_matrix = fsm_search.get();

    size_t matched_letters_count { 0 }, uniq_symbols_amount { 256 };

    for (const char& heystack_char : heystack) {
        if (heystack_char != needle[matched_letters_count]) {
            if(matched_letters_count) {
                matched_letters_count = fsm_matrix[matched_letters_count * uniq_symbols_amount + static_cast<size_t>(static_cast<unsigned char>(heystack_char))];
            }
        } else {
            ++matched_letters_count;

            if (matched_letters_count == search_str_len)
                return true;
        }
    }

    if (matched_letters_count == search_str_len)
        return true;
    else
        return false;
}

bool match_with_quadratic_complexity(const string& heystack, const string& needle) {
    if (heystack.size() < needle.size())
        return false;

    for (size_t i = 0; i <= heystack.size() - needle.size(); ++i) {

        size_t j = 0;
        while (j < needle.size()) {
            if (needle[j] != heystack[i+j])
                break;

            ++j;
        }

        if (j == needle.size()) {
            return true;
        }
    }

    return false;
}

bool regex_check_match(const string& heystack, const string& needle) {
    if (heystack.size() < needle.size())
        return false;

    string regex_string = ".*" + needle + ".*";
    regex search_regex { regex_string };
    if (regex_match(heystack, search_regex))
        return true;

    return false;
}

bool boyer_moore_match(const string& heystack, const string& needle) {

    if (needle == "" )
        return true;

    map <char, size_t> needle_letters;
    for (size_t i = 0; i < needle.size() - 1; ++i)
        needle_letters[needle[i]] = needle.size() - (i + 1);

    if (heystack.size() < needle.size())
        return false;

    size_t k { needle.size() - 1 }, j { 0 };
    while (k < heystack.size() and j != needle.size()) {
        if (heystack[k - j] == needle[needle.size() - j - 1]) {
            ++j;
        } else {
            if (needle_letters.find(heystack[k - j]) != needle_letters.end()) {
                if (needle_letters[heystack[k - j]] < j) {
                    k += needle.size() - j;
                } else {
                    k += needle_letters[heystack[k - j]] - j;
                }
            } else {
                k += needle.size() - j;
            }

            j = 0;
        }
    }

    if (j == needle.size())
        return true;
    else
        return false;
}
