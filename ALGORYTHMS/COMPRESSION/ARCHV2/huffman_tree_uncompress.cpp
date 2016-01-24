#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bitset>
#include <cassert>

#include "huffman_tree_uncompress.h"

using namespace std;
huffman_tree_uncompress::huffman_tree_uncompress(string filename) : compressed_file(filename)
{
    ifstream header_file;
    header_file.open(compressed_file,ios::in | ios::binary );

    uint16_t header_word;
    size_t head_bit_count = 1;
    size_t sentinel_header_code = 1;
    size_t bit_value = 1;
    cout << "======================== Debug Read Header ================" << endl;
    while (sentinel_header_code != 16)
    {
        header_file.read((char *)&header_word, 2);
        cout << bitset<16>(header_word) << endl;

        while (head_bit_count <= 16 and sentinel_header_code != 16)
        {
            //PREFIX
            cout << "[D] Prefix : ";
            while (bit_value != 0 and sentinel_header_code != 16)
            {
                bit_value = header_word >> (16 - head_bit_count);
                cout << bit_value;
                header_word = header_word - (bit_value << (16 - head_bit_count));
                ++sentinel_header_code;
                ++head_bit_count;
                if (head_bit_count > 16)
                {
                    head_bit_count = 1;
                    header_file.read((char *)&header_word, 2);
                }
            }
            cout << endl;

            //SYMBOL VALUE
            if (sentinel_header_code != 16)
            {
                uint16_t code_letter = 0;
                cout << "[D] Char : ";
                for (size_t uint16_t_code_count = 0; uint16_t_code_count < 9; ++uint16_t_code_count)
                {
                    bit_value = header_word >> (16 - head_bit_count);
                    cout << bit_value;
                    header_word = header_word - (bit_value << (16 - head_bit_count));
                    ++head_bit_count;

                    code_letter = code_letter << 1;
                    if (bit_value == 1)
                        ++code_letter;

                    if (head_bit_count > 16)
                    {
                        head_bit_count = 1;
                        header_file.read((char *)&header_word, 2);
                    }
                }
                uint16_t head_letter = (uint16_t) code_letter;
                cout << "  (" << head_letter << ")" << endl;

                //HUFFMAN CODE
                cout << "[D] Huff code : ";
                string huf_code_string = "";
                for (size_t huf_code_count = 0; huf_code_count < sentinel_header_code; ++huf_code_count)
                {
                    bit_value = header_word >> (16 - head_bit_count);
                    cout << bit_value;
                    huf_code_string = huf_code_string + to_string(bit_value);
                    header_word = header_word - (bit_value << (16 - head_bit_count));
                    ++head_bit_count;
                    if (head_bit_count > 16)
                    {
                        head_bit_count = 1;
                        header_file.read((char *)&header_word, 2);
                    }
                }
                cout << "\n[D] Huff code string : " << huf_code_string << "  Code_letter : " << code_letter;
                cout << "\n";
                huf_code_string = "M" + huf_code_string;

                huffman_code_to_string_map16.insert(pair<string, uint16_t>(huf_code_string, code_letter));

                sentinel_header_code = 1;
                bit_value = 1;
            }
        }
    }
    cout << "======================== Debug Read Header End ============" << endl;

    header_end_pos = header_file.tellg();
    header_file.close();

    cout << "[DEBUG] New Header closed" << endl;

}

vector <uint16_t> huffman_tree_uncompress::decompress_value16(vector <uint16_t> compressed_vector16)
{
    vector <uint16_t> result_uncompressed_value16;
    string search_value16 = "M";
    uint16_t huffman_string_sentinel = 259;

    cout << "RESULT STRING : ";
    for (auto compressed_value_unit16 : compressed_vector16)
    {
        size_t bit_offset16 = 15;
        size_t bit_count16 = 1;
        size_t bit_size16 = 16;
        while (bit_count16 < 17)
        {
            --bit_size16;

            size_t compressed_value_unit_byte16 = compressed_value_unit16 >> bit_size16;
            compressed_value_unit16 = compressed_value_unit16 - (compressed_value_unit_byte16 << bit_size16);
            search_value16 = search_value16 + to_string(compressed_value_unit_byte16);
            cout << "SV : " << search_value16 << endl;

            
            //FIXME ???????                if (huffman_code_to_string_map16[search_value16] == 259)
            if (huffman_code_to_string_map16.find(search_value16)->second == huffman_string_sentinel)
                bit_count16 = 18;
            else if (huffman_code_to_string_map16.count(search_value16))
            {
                cout << huffman_code_to_string_map16[search_value16] << " ";
                result_uncompressed_value16.push_back(huffman_code_to_string_map16[search_value16]);
                search_value16 = "M";
            } 

            ++bit_count16;
            --bit_offset16;
        }
    }
    cout << endl;
    return result_uncompressed_value16;
}


vector <uint16_t> huffman_tree_uncompress::read_compressed_string()
{
    // Read New archive file        
    ifstream new_arch_file;
    new_arch_file.open(compressed_file,ios::in | ios::binary );
    new_arch_file.seekg(0,new_arch_file.end);
    size_t size = new_arch_file.tellg();
    new_arch_file.seekg(header_end_pos);

    uint16_t body_word = 0;
    vector <uint16_t> compressed_value_vector16;
    while (new_arch_file.tellg() < size)
    {
        new_arch_file.read((char *)&body_word, 2);
        compressed_value_vector16.push_back(body_word);
        cout << "[DEBUG Body ] " << bitset<16>(body_word) << endl;
    }

    vector <uint16_t> decompressed_string16 = decompress_value16(compressed_value_vector16);

    return decompressed_string16;
}
