#ifndef HUFFMAN_UNCOMPRESS_H_
#define HUFFMAN_UNCOMPRESS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bitset>
#include <cassert>

using namespace std;

class huffman_tree_uncompress
{
    private:
        size_t header_end_pos;
        string compressed_file;
        map<string, uint16_t> huffman_code_to_string_map16 = {};

    public:
        explicit huffman_tree_uncompress(string filename);
        vector <uint16_t> decompress_value16(vector <uint16_t> compressed_vector16);
        vector <uint16_t> read_compressed_string();
};

//string decompress2(string const & packed);

#endif //HUFFMAN_H_
