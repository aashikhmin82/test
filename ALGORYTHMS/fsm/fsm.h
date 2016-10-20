#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class fsm {

    private:
    vector<vector<size_t>> fsm_matrix;

    size_t check_substring_match(const unordered_set<string>& search_substrings, string substring, char unmatched_char) {
        substring.pop_back();
        substring += unmatched_char;
        while (substring.size() > 0) {
                substring.erase(substring.begin());

            if (search_substrings.find(substring) != search_substrings.end())
                return substring.size();
        }

        return 0;
    }

    public:
    explicit fsm(const string& search_string) : fsm_matrix(search_string.size(), vector<size_t>(127,0)) {
        unordered_set<string> search_substrings;

        string substring { "" };
        size_t i { 0 }, match_count { 1 };

        for (const auto& ch : search_string) {
            string prev_substring = substring;
            substring += ch;
            search_substrings.insert(substring);

            size_t ch_number = static_cast<size_t>(ch);
            fsm_matrix[i][ch_number] = match_count;

            if (i > 0) {
                for (const char& prev_substr_ch : prev_substring) {
                    if (prev_substr_ch != ch) {
                        size_t match_count = check_substring_match(search_substrings, substring, prev_substr_ch);
                        size_t prev_ch_number = static_cast<size_t>(prev_substr_ch);
                        fsm_matrix[i][prev_ch_number] = match_count;
                    }
                }
            }

            ++i;
            ++match_count;
        }
    }

    const vector<vector<size_t>> get() const {
        return fsm_matrix;
    }
};
