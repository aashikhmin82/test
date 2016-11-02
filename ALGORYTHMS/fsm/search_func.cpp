#include <regex>
#include <unordered_map>

#include "fsm.h"

using namespace std;

bool fsm_check_match(const string& test_string, const string& search_string) {

    if (search_string == "" )
        return true;

    const size_t search_str_len = search_string.size();
    if (search_str_len > test_string.size())
        return false;

    fsm fsm_search { search_string };
    auto fsm_matrix = fsm_search.get();

    size_t matched_letters_count { 0 };

    for (const char& test_ch : test_string) {
        if (test_ch != search_string[matched_letters_count]) {
            if(matched_letters_count) {
                matched_letters_count = fsm_matrix[matched_letters_count * 127 + static_cast<size_t>(test_ch)];
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

bool match_with_quadratic_complexity(const string& test_string, const string& search_string) {
    if (test_string.size() < search_string.size())
        return false;

    for (size_t i = 0; i <= test_string.size() - search_string.size(); ++i) {

        size_t j = 0;
        while (j < search_string.size()) {
            if (search_string[j] != test_string[i+j])
                break;

            ++j;
        }

        if (j == search_string.size()) {
            return true;
        }
    }

    return false;
}

bool regex_check_match(const string& test_string, const string& search_string) {
    if (test_string.size() < search_string.size())
        return false;

    string regex_string = ".*" + search_string + ".*";
    regex search_regex { regex_string };
    if (regex_match(test_string, search_regex))
        return true;

    return false;
}
