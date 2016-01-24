#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_map>

#include "huffman_tree16.h"

using namespace std;

huffman_tree::huffman_tree (multimap<size_t, uint16_t> probability_symbol) : symbol_frequency_map(probability_symbol)
{
    size_t multimap_count = 0;
    size_t multimap_size = symbol_frequency_map.size();
    while (multimap_count < multimap_size or huffman_tree_result.size() != 1)
    {
        size_t summ_count;
        shared_ptr <class_huffman_tree_element> first_map_object;
        shared_ptr <class_huffman_tree_element> second_map_object;

        if (multimap_count == multimap_size
                or (huffman_tree_result.begin()->first 
                    and huffman_tree_result.begin()->first <= symbol_frequency_map.begin()->first))
        {
            first_map_object = huffman_tree_result.begin()->second;

            if ((((++huffman_tree_result.begin())->first != 1) and (++huffman_tree_result.begin())->first <= symbol_frequency_map.begin()->first) or (symbol_frequency_map.size() == 0))
            {
                second_map_object = (++huffman_tree_result.begin())->second;
                huffman_tree_result.erase(++huffman_tree_result.begin());
            }
            else
            {
                shared_ptr <class_huffman_tree_element> map_object_tmp2(make_shared<class_huffman_tree_element>(symbol_frequency_map.begin()->first,symbol_frequency_map.begin()->second));
                second_map_object = map_object_tmp2;

                symbol_frequency_map.erase(symbol_frequency_map.begin());
                ++multimap_count;
            }
            huffman_tree_result.erase(huffman_tree_result.begin());
        }
        else
        {
            shared_ptr <class_huffman_tree_element> map_object_t1(make_shared<class_huffman_tree_element>(symbol_frequency_map.begin()->first,symbol_frequency_map.begin()->second));
            first_map_object = map_object_t1;
            ++multimap_count;

            if ((huffman_tree_result.size() > 0 
                        and huffman_tree_result.begin()->first != 1 
                        and huffman_tree_result.begin()->first <= (++symbol_frequency_map.begin())->first)
                    or symbol_frequency_map.size() == 1)
            {
                second_map_object = huffman_tree_result.begin()->second;
                huffman_tree_result.erase(huffman_tree_result.begin());
            }
            else
            {
                shared_ptr <class_huffman_tree_element> map_object_tmp2(make_shared<class_huffman_tree_element>((++symbol_frequency_map.begin())->first,(++symbol_frequency_map.begin())->second));
                second_map_object = map_object_tmp2;
                symbol_frequency_map.erase(symbol_frequency_map.begin());
                ++multimap_count;
            }

            symbol_frequency_map.erase(symbol_frequency_map.begin());
        }

        summ_count = first_map_object->sym_count() + second_map_object->sym_count();
        shared_ptr <class_huffman_tree_element> map_object_summ(make_shared<class_huffman_tree_element>(summ_count,0x01F0));
        map_object_summ->weak_left_down() = first_map_object;
        map_object_summ->weak_right_down() = second_map_object;
        first_map_object->right_up() = map_object_summ;
        second_map_object->left_up() = map_object_summ;
        huffman_tree_result.insert(pair<size_t, shared_ptr<class_huffman_tree_element>>(summ_count, map_object_summ));
    }

    vector<size_t> code_tmp(0);

    create_huffman_map(huffman_tree_result.begin()->second, code_tmp);
}

void huffman_tree::create_huffman_map(shared_ptr<class_huffman_tree_element> huffman_tree_element, vector<size_t> code)
{
    size_t code_unit = 0;
    if (huffman_tree_element->left_down())
    {
        vector<size_t> code_left = code;
        code_left.push_back(code_unit);
        if ((huffman_tree_element->left_down()->left_down())
                or (huffman_tree_element->left_down()->right_down()))
        {
            create_huffman_map(huffman_tree_element->left_down(),code_left);
        }
        else
        {
            cout << "[DEBUG] CODE : " << huffman_tree_element->left_down()->sym_letter() << "  (";
            for (auto code_int : code_left)
                cout << code_int;
            cout << ")" << endl;
        }

        if (huffman_tree_element->left_down()->sym_letter() != 0x01F0)
        {
            huffman_code_map.insert(pair<uint16_t, vector<size_t>>(huffman_tree_element->left_down()->sym_letter(), code_left));
        }
    }


    if (huffman_tree_element->right_down())
    {
        ++code_unit;
        vector<size_t> code_right = code;
        code_right.push_back(code_unit);
        if ((huffman_tree_element->right_down()->left_down())
                or (huffman_tree_element->right_down()->right_down()))
        {
            create_huffman_map(huffman_tree_element->right_down(),code_right);
        }
        else
        {
            cout << "[DEBUG] CODE : " << huffman_tree_element->right_down()->sym_letter() << "  (";
            for (auto code_int : code_right)
            {
                cout << code_int;
            }
            cout << ")" << endl;

            if (huffman_tree_element->right_down()->sym_letter() != 0x01F0)
                huffman_code_map.insert(pair<uint16_t, vector<size_t>>(huffman_tree_element->right_down()->sym_letter(), code_right));
        }
    }
}

void huffman_tree::create_archive_string(vector <uint16_t> vector_to_compress16)
{
    cout << "[DEBUG] CodeMap Size : " << huffman_code_map.size() << endl;
    for (auto code_map_element : huffman_code_map)
    {
        cout << "[DEBUG] code_map_element : " << code_map_element.first << "  ->  ";
    }

    //sentinel
    vector_to_compress16.push_back(0x0103);

    uint16_t compressed_value16 = 0;
    size_t bytes_count16 = 0;

    cout << "------------------------- Create New Vector ------------------------" << endl;
    for (size_t i16 = 0; i16 < vector_to_compress16.size(); ++i16)
    {
        for (auto code_int_tmp16 : huffman_code_map[vector_to_compress16[i16]])
        {
            compressed_value16 = compressed_value16 << 1;
            ++bytes_count16;
            if (code_int_tmp16 == 1)
                ++compressed_value16;

            cout << "[DEBUG] tmp value (" << vector_to_compress16[i16] << ") : " << bitset<16>(compressed_value16) << endl;

            if (bytes_count16 == 16)
            {
                compressed_string16.push_back(compressed_value16);
                cout << "[DEBUG] Save value : " << bitset<16>(compressed_value16) << endl;
                compressed_value16 = 0;
                bytes_count16 = 0;
            }
        }
    }

    if (bytes_count16 != 0)
    {
        compressed_value16 = compressed_value16 << (16 - bytes_count16);
        cout << "[DEBUG] Save value : " << bitset<16>(compressed_value16) << endl;
        compressed_string16.push_back(compressed_value16);
    }

    cout << "------------------------- End Create New Vector --------------------" << endl;
}

void huffman_tree::create_archive_header(shared_ptr<class_huffman_tree_element> huffman_tree_e, string filename)
{

    std::ofstream header_file;
    header_file.open(filename, ofstream::binary);
    vector <uint16_t> header_vector;
    uint16_t headers_bits = 0;
    size_t header_bits_count = 0;
    vector <uint16_t> header_vec;

    for (auto code_map_element1 : huffman_code_map)
    {
        size_t code_num1 = 0;
        cout << "[DG DECOMPR] code_map : " << code_map_element1.first;

        size_t symbol_bit_length = 9;
        uint16_t usymbol_tmp = code_map_element1.first;
        uint16_t usymbol_bit = 0;

        // Bit prefix Example : (1)111110
        size_t bit_count = 1;
        cout << endl;
        while (bit_count < (code_map_element1.second).size())
        {
            ++bit_count;
            headers_bits = headers_bits << 1;
            if (bit_count != (code_map_element1.second).size())
            {
                ++headers_bits;
            }

            ++header_bits_count;
            if (header_bits_count == 16)
            {
                header_vec.push_back(headers_bits);
                header_bits_count = 0;
                headers_bits = 0;
            }
        }

        // Char to bits
        cout << "[DEB] CHAR BYTE : ";
        while (symbol_bit_length > 0)
        {
            usymbol_bit = usymbol_tmp >> (symbol_bit_length - 1);
            size_t size_bit = (size_t)usymbol_bit;
            cout << size_bit;

            headers_bits = headers_bits << 1;
            if (size_bit == 1)
                ++headers_bits;

            --symbol_bit_length;
            usymbol_tmp = usymbol_tmp - (usymbol_bit << symbol_bit_length);

            ++header_bits_count;
            if (header_bits_count == 16)
            {
                header_vec.push_back(headers_bits);
                header_bits_count = 0;
                headers_bits = 0;
            }
        }
        cout << endl;
        //            headers_bits = headers_bits << 8;

        // Huffman code to bits
        for (auto bin_code1 : code_map_element1.second)
        {
            cout << bin_code1;
            code_num1 = code_num1 << 1;
            headers_bits = headers_bits << 1;
            if (bin_code1 == 1)
            {
                ++code_num1;
                ++headers_bits;
            }

            ++header_bits_count;
            if (header_bits_count == 16)
            {
                header_vec.push_back(headers_bits);
                header_bits_count = 0;
                headers_bits = 0;
            }
        }

        cout << endl;
        cout << " CodeToChar Map : " << bitset<16>(code_num1) << endl;
        cout << "[DEBUG New HEADER] Char and Code : " << bitset<16>(headers_bits) << endl;
    }

    //sentinel 1(16) - 111111111111111
    size_t header_sentinel = 0;
//    while (header_sentinel < 10)
    while (header_sentinel < 16)
    {
        headers_bits = headers_bits << 1;
        ++headers_bits;
        ++header_bits_count;
        ++header_sentinel;

        if (header_bits_count == 16)
        {
            header_vec.push_back(headers_bits);
            header_bits_count = 0;
            headers_bits = 0;
        }
    }

    if (header_bits_count != 0)
    {
        headers_bits = headers_bits << (16 - header_bits_count);
        header_vec.push_back(headers_bits);
    }
    //        }

    //DEBUG
cout << "===================  DEBUG ====================" << endl;
for (auto debug_headers_bits : header_vec)
{
    header_file.write((char *)&debug_headers_bits,2);
    cout << "[DEBUG HVector] : " << bitset<16>(debug_headers_bits) << endl;
}
cout << "===================  DEBUG END ================" << endl;
//DEBUG END

header_file.close();
}

void huffman_tree::save_compressed_string(string filename)
{
    cout << "Write to the file  : " << filename << endl;

    create_archive_header(huffman_tree_result.begin()->second, filename);

    std::ofstream arch_file16;
    arch_file16.open(filename, ios::app | ios::out | ios::binary);
    arch_file16.seekp(0,arch_file16.end);

    cout << "++++++++++++++++++++++++ Save New Archive ++++++++++++++++++++++++++++" << endl;
    for (auto compressed_value_unit16 : compressed_string16)
    {
        cout << "[DEBUG Save Compressed value: " << compressed_value_unit16 << "  (" << bitset <16>(compressed_value_unit16) << ")" << endl;
        arch_file16.write((char *)&compressed_value_unit16,2);
    }
    cout << "++++++++++++++++++++++++ New Archive Saved +++++++++++++++++++++++++++" << endl;

    arch_file16.close();
    }
