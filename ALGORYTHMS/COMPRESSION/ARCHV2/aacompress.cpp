#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iterator>

#include "huffman_tree16.h"
#include "compress_algorithm.h"

using namespace std;

int main(int argc, char* arg_vec[])
{
    map<char, size_t> symbol_count;
    map<uint16_t, size_t> symbol_count16;

    if (argc < 3 )
    {
        cerr << "HELP: " << endl;
        cerr << "./programm file_to_compress arch_filename" << endl;
        return 1;
    }

    string file_to_compress = arg_vec[1];
    string compressed_filename = arg_vec[2];
    string str_el1 = "";

    cout << "[DEBUG] File name 1: " << file_to_compress << endl;

    ifstream compress_file_stream(file_to_compress);

    compress_file_stream >> noskipws;
    copy(istream_iterator<char>(compress_file_stream), istream_iterator<char>(), back_inserter(str_el1));
    cout << "File : " << str_el1;


    cout << "[DEBUG] String1 : " << str_el1 << endl;
    vector <uint16_t> vec_to_compress = convert_to_utf16(str_el1);
    //DEBUG
    cout << "Converted : ";
    for (auto convert_sym : vec_to_compress)
        cout << convert_sym << " ";
    cout << endl;
    //DEBUG END

    vector <uint16_t> vec_bwt = bwt_compress16(vec_to_compress);
    //DEBUG
    cout << "BWT compressed : ";
    for (auto bwt_sym : vec_bwt)
        cout << bwt_sym << " ";
    cout << endl;
    //DEBUG END

    vector <uint16_t> vec_rle = rle_compress16(vec_bwt);
    //DEBUG
    cout << "RLE compressed : ";
    for (auto compressed_rle_sym : vec_rle)
        cout << compressed_rle_sym << " ";
    cout << endl;
    //DEBUG END

//Huffman probability count
    for (auto rle_sym : vec_rle)
    {
        if (symbol_count16[rle_sym])
            ++symbol_count16[rle_sym];
        else
            symbol_count16[rle_sym] = 1;
    }

    multimap<size_t, uint16_t> probability_symbol16;
    cout << "=========================== Huffman Probability ===================" << endl;
    for (auto& prob_syb : symbol_count16)
    {
        cout << prob_syb.first << " (" << prob_syb.second << ")  ";
        probability_symbol16.insert(pair<size_t, uint16_t>(prob_syb.second, prob_syb.first));
    }
    cout << endl;
    cout << "=========================== Huffman Probability End ===============" << endl;
    probability_symbol16.insert(pair<size_t, uint16_t>(1,0x0103));



    huffman_tree huffman_codec(probability_symbol16);
    huffman_codec.create_archive_string(vec_rle);
    huffman_codec.save_compressed_string(compressed_filename);

    return 0;
}
