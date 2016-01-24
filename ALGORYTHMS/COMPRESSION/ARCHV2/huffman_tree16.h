#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_map>

#include "huffman_tree_element16.h"

using namespace std;

class huffman_tree
{
    private:
        multimap<size_t, uint16_t> symbol_frequency_map;
        unordered_map<uint16_t, vector<size_t>> huffman_code_map;
        vector <uint16_t> compressed_string16;
        multimap <size_t, shared_ptr<class_huffman_tree_element>> huffman_tree_result;

    public:
        explicit huffman_tree (multimap<size_t, uint16_t> probability_symbol);
        void create_huffman_map(shared_ptr<class_huffman_tree_element> huffman_tree_element, vector<size_t> code);
        void create_archive_string(vector <uint16_t> vector_to_compress16);
        void create_archive_header(shared_ptr<class_huffman_tree_element> huffman_tree_e, string filename);
        void save_compressed_string(string filename);
};

#endif /* HUFFMAN_H_ */
