#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "debug.h"

using namespace std;
using anagram_map_t = std::unordered_map<std::string, std::vector<std::string>>;

void print_map(const anagram_map_t& anagram_map) {
    for (const auto& anagram_signature_n_words_vec : anagram_map) {
        cout << "Key : " << anagram_signature_n_words_vec.first << " -> ";
        for (const auto& anagram_word : anagram_signature_n_words_vec.second) {
            cout << anagram_word << " ";
        }
        cout << endl;
    }
}

void delete_nodes_containing_only_one_word(auto& anagram_map) {
    vector<string> words_to_delete;

    for (const auto& anagram_signature_n_words_vec : anagram_map) {
        if (anagram_signature_n_words_vec.second.size() < 2) {
            words_to_delete.push_back(anagram_signature_n_words_vec.first);
        }
    }

    for (const auto& word : words_to_delete) {
        anagram_map.erase(word);
    }
}

anagram_map_t search_anagrams(const unordered_map<size_t, vector<string>>& same_letters_map) {
     anagram_map_t anagram_map;

    for (const auto& word_signature_n_words_vec : same_letters_map) {
        if (word_signature_n_words_vec.second.size() > 1) {
            for (const auto& word : word_signature_n_words_vec.second) {
                auto sorted_letters = word;
                sort(sorted_letters.begin(), sorted_letters.end());
                anagram_map[sorted_letters].push_back(word);
            }
        }
    }

    delete_nodes_containing_only_one_word(anagram_map);

    return anagram_map;
}

void get_file_content(const string& input_filename, bool& get_file_thread_finished,
                      anagram_map_t& anagrams_map, print_debug& debug) {
    ifstream input_fd(input_filename);

    string input_word;
    size_t i{0};
    unordered_map<size_t, vector<string>> same_letters_map;
    debug << "[print_file_content] Filename : " << input_filename << "\n";

    if (input_fd.is_open()) {
        while (input_fd >> input_word) {
            size_t word_sum { 0 };
            for (auto& word_letter : input_word) {
                word_sum = word_sum | (1 << (static_cast<size_t>(word_letter) - static_cast<size_t>('a')));
            }

            same_letters_map[word_sum].push_back(input_word);
            ++i;
        }
    }

    anagrams_map = search_anagrams(same_letters_map);

    get_file_thread_finished = true;
    debug << "[print_file_content] End\n";
}
