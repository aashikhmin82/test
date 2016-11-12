#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class fsm {

    private:
    vector<size_t> fsm_matrix;

    size_t search_longest_needle_substring(const unordered_set<string>& needle_prefixes, string needle_substring) {
        while (needle_substring.size() > 0) {
            needle_substring.erase(needle_substring.begin());

            if (needle_prefixes.find(needle_substring) != needle_prefixes.end()) {
                return needle_substring.size();
            }
        }

        return 0;
    }

    public:
    explicit fsm(const string& needle) : fsm_matrix(needle.size() * 127, 0) {
        unordered_set<string> needle_prefixes;

        size_t match_count { 1 };

        for (size_t i = 0; i < needle.size(); ++i) {
            needle_prefixes.insert(needle.substr(0,i));

            fsm_matrix[127 * i + static_cast<size_t>(needle[i])] = match_count;

            if (i > 0) {
                for (size_t j = 0; j < i; ++j) {
                        string needle_substring = needle.substr(0,i) + needle[j];
                        size_t match_count = search_longest_needle_substring(needle_prefixes, needle_substring);
                        fsm_matrix[127 * i + static_cast<size_t>(needle[j])] = match_count;
                }
            }

            ++match_count;
        }
    }

    const vector<size_t> get() const {
        return fsm_matrix;
    }
};
