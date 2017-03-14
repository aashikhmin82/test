#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "debug.h"

using namespace std;

void print_map(const map<string, vector<string>> anagram_map) {
    for (auto& anagram_map_i : anagram_map) {
        cout << "Key : " << anagram_map_i.first << " -> ";
        for (const auto& anagr_i : anagram_map_i.second) {
            cout << anagr_i << " ";
        }
        cout << endl;
    }
}

void delete_nodes_contains_only_one_word(auto& anagram_map) {
    vector<string> words_to_delete;

    for (const auto& anagram_map_i : anagram_map) {
        if (anagram_map_i.second.size() < 2) {
            words_to_delete.push_back(anagram_map_i.first);
        }
    }

    for (const auto& word : words_to_delete) {
        anagram_map.erase(word);
    }
}

map<string, vector<string>> search_anagrams(const map<size_t, vector<string>>& same_letters_map) {
    map<string, vector<string>> anagram_map;

    for (const auto& same_letters_i : same_letters_map) {
        if (same_letters_i.second.size() > 1) {
            for (const auto& word : same_letters_i.second) {
                auto sorted_letters = word;
                sort(sorted_letters.begin(), sorted_letters.end());
                anagram_map[sorted_letters].push_back(word);
            }
        }
    }

    delete_nodes_contains_only_one_word(anagram_map);

    return anagram_map;
}

void get_file_content(const string input_filename, bool& get_file_thread_finished,
                      map<string, vector<string>>& anagrams_map, print_debug& debug) {
    ifstream input_fd(input_filename);

    string input_word;
    size_t i{0};
    map<size_t, vector<string>> same_letters_map;
    debug << "[print_file_content] Filename : " << input_filename << "\n";

    if (input_fd.is_open()) {
        while (input_fd >> input_word) {
            size_t word_sum { 0 };
            for (auto& word_letter : input_word) {
                word_sum = word_sum | (1 << (static_cast<size_t>(word_letter) - 97));
            }

            same_letters_map[word_sum].push_back(input_word);
            ++i;
        }
    }

    anagrams_map = search_anagrams(same_letters_map);

    get_file_thread_finished = true;
    debug << "[print_file_content] End\n";
}
