#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <random>

#include "compress_algorithm.h"

using namespace std;

void autotest_func(string test_string)
{
        //Test A (convert testing)
        vector <uint16_t> vec_to_compressA = convert_to_utf16(test_string);

        string uncompressed_stringA = bwt_convert_to_string(vec_to_compressA);
    
        cout << "Result A : " << uncompressed_stringA << endl;
        assert(uncompressed_stringA == test_string);
        cout << "TestA PASSED!" << endl;

        //Test B (BWT testing)
        vector <uint16_t> vec_to_compressB = convert_to_utf16(test_string);
        cout << "Conv: ";
        for (auto convert_symB : vec_to_compressA)
            cout << convert_symB << " ";
        cout << endl;
        vector <uint16_t> vec_bwtB = bwt_compress16(vec_to_compressB);
        cout << "BWT compressed: ";
        for (auto bwt_symB : vec_bwtB)
            cout << bwt_symB << " ";
        cout << endl;

        vector <uint16_t> uncompressed_bwt_vecB = bwt_uncompress16(vec_bwtB);
        cout << "BWT uncompres: ";
        for (auto bwt_symB : uncompressed_bwt_vecB)
            cout << bwt_symB << " ";
        cout << endl;
        string uncompressed_stringB = bwt_convert_to_string(vec_to_compressB);
    
        cout << "Result B : " << uncompressed_stringB << endl;
        assert(uncompressed_stringB == test_string);
        cout << "TestB PASSED!" << endl;

        //Test C (RLE testing)
        vector <uint16_t> vec_to_compress = convert_to_utf16(test_string);
        cout << "Converted : ";
        for (auto convert_sym : vec_to_compress)
            cout << convert_sym << " ";
        cout << endl;

        vector <uint16_t> vec_bwt = bwt_compress16(vec_to_compress);
        cout << "BWT compressed : ";
        for (auto bwt_sym : vec_bwt)
            cout << bwt_sym << " ";
        cout << endl;

        vector <uint16_t> vec_rle = rle_compress16(vec_bwt);
        cout << "RLE compressed : ";
        for (auto compressed_rle_sym : vec_rle)
            cout << compressed_rle_sym << " ";
        cout << endl;

        vector <uint16_t> uncompressed_rle_vec = rle_uncompress16(vec_rle);
        cout << "RLE uncompress : ";
        for (auto uncompr_rle_sym : uncompressed_rle_vec)
            cout << uncompr_rle_sym << " ";
        cout << endl;

        vector <uint16_t> uncompressed_bwt_vec = bwt_uncompress16(uncompressed_rle_vec);
        cout << "BWT uncompressed : ";
        for (auto ubwt_sym : uncompressed_bwt_vec)
            cout << ubwt_sym << " ";
        cout << endl;

        string uncompressed_string = bwt_convert_to_string(uncompressed_bwt_vec);
        cout << "\nResult : " << uncompressed_string << endl;

        assert(uncompressed_string == test_string);
        cout << "Test PASSED!" << endl;
}

default_random_engine re;

string random_string(size_t max_size)
{
    uniform_int_distribution<size_t> random_size(1, max_size);
//    uniform_int_distribution<char> random_letter('a', 'z', ' ', '\n');
    uniform_int_distribution<char> random_letter('\n','z');

    string s(random_size(re), '?');

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

int main(int argc, char* arg_vec[])
{
    vector <string> test_strings_vec = {"bananaaazzz", "bana ana", "bndndndnadsfsd\nkdlfd", "dkpevezggcfqsustrqbpfisdtnopisdem", "mmmiiiaa..agtp. c heos 0 aaade a", "bananaabcd", "ba\nb"};
    for (auto test_string : test_strings_vec)
    {
        cout << test_string << endl;
        autotest_func(test_string);
    }

    for (size_t i = 0; i < 1000; ++i)
    {
        string random_str = random_string(100);
        cout << i << ") random_str : " << random_str << endl;
        autotest_func(random_str);
        cout << "Test " << i << " PASSED" << endl;
    }

    return 0;
}
