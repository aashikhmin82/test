#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "compress_algorithm.h"

using namespace std;

vector <uint16_t> convert_to_utf16(string str_el)
{
    vector <uint16_t> vec_to_compress;

    for (size_t i = 0; i < str_el.size(); ++i)
    {
        uint16_t sym16 = str_el[i];
        vec_to_compress.push_back(sym16);
    }

    return vec_to_compress;
}

vector <uint16_t> bwt_compress16(vector <uint16_t> vec_to_compress)
{
    vector <uint16_t> sorted_vec;

    vec_to_compress.push_back(0x0101);

    vector <vector <uint16_t>> vec_to_sort;
    vec_to_sort.push_back(vec_to_compress);
    vector <uint16_t> vec_to_compress_start = vec_to_compress;

    for (auto sym16 : vec_to_compress_start)
    {
        vec_to_compress.erase(vec_to_compress.begin());
        vec_to_compress.push_back(sym16);
        vec_to_sort.insert(vec_to_sort.begin(),vec_to_compress);
    }

    vec_to_sort.erase(vec_to_sort.end());
    sort(vec_to_sort.begin(), vec_to_sort.end());
    for (auto vec_string : vec_to_sort)
        sorted_vec.push_back(vec_string.back());

    return sorted_vec;
}

vector <uint16_t> rle_compress16(vector <uint16_t> compress_bwt_vec)
{
    uint16_t iter_count = 1;
    uint16_t non_iter_count16 = 0;
    vector <uint16_t> rle_part;
    uint16_t prev_char16 = 260;
    vector <uint16_t> compressed_rle_vec;

    for (uint16_t rle_letter16 : compress_bwt_vec)
    {
        if (prev_char16 == rle_letter16)
        {
            if (non_iter_count16 > 1)
            {
//                non_iter_count16 = non_iter_count16 - 1 + 256;
                non_iter_count16 = non_iter_count16 - 1;
                rle_part.insert(rle_part.begin(),non_iter_count16);
                rle_part.insert(rle_part.begin(),0);

                for (auto rle_part_sym : rle_part)
                    compressed_rle_vec.push_back(rle_part_sym);

//                non_iter_count16 = 0;
            }

            ++iter_count;
            rle_part.erase(rle_part.begin(),rle_part.end());
//            rle_part.push_back(0);
            rle_part.push_back(iter_count);
            rle_part.push_back(prev_char16);
            non_iter_count16 = 0;
        }
        else
        {
            if (iter_count > 1)
            {
                for (auto rle_part_sym : rle_part)
                    compressed_rle_vec.push_back(rle_part_sym);

                iter_count = 1;
                rle_part.erase(rle_part.begin(),rle_part.end());
            }

            if (non_iter_count16 > 0)
                rle_part.push_back(prev_char16);
            ++non_iter_count16;
        }
        prev_char16 = rle_letter16;
    }

    if (iter_count > 1)
    {
        non_iter_count16 = 0;
        for (auto rle_part_sym : rle_part)
            compressed_rle_vec.push_back(rle_part_sym);
    }

    if (non_iter_count16 > 0)
    {
//        non_iter_count16 = non_iter_count16 + 256;
        rle_part.push_back(prev_char16);
        rle_part.insert(rle_part.begin(),non_iter_count16);
        rle_part.insert(rle_part.begin(),0);

        for (auto rle_part_sym : rle_part)
            compressed_rle_vec.push_back(rle_part_sym);
    }

    return compressed_rle_vec;
}

vector <uint16_t> rle_uncompress16(vector <uint16_t> compressed_rle_vec)
{
    vector <uint16_t> compressed_bwt_vec;

    size_t prefix_flag = 0;
    uint16_t sym_count = 0;
//    uint16_t nonrep_count = 0;
    size_t nonrep_count_flag = 0;
    for (auto rle_sym : compressed_rle_vec)
    {
        if (prefix_flag == 0)
        {
            prefix_flag = 1;
            sym_count = rle_sym;

cout << "DEBUG: sym_count : " << sym_count << endl;
            if (sym_count == 0)
            {
                prefix_flag = 0;
                nonrep_count_flag = 1;
                cout << "DEBUG: rep_count_flag is ON" << endl;
            }

//            if (sym_count > 256)
 //               nonrep_count = sym_count;
        } 
        else
        {
//            if (sym_count < 256)
//            if (rep_count_flag == 1)
//            {
//            }
            if (nonrep_count_flag == 1)
            {
//                --nonrep_count;
                --sym_count;
                compressed_bwt_vec.push_back(rle_sym);
//                if (nonrep_count == 0)
                if (sym_count == 0)
                {
                    prefix_flag = 0;
                    nonrep_count_flag = 0;
                }
            }
            else
            {
                for (size_t rep_count = 0; rep_count < sym_count; ++rep_count)
                    compressed_bwt_vec.push_back(rle_sym);

                prefix_flag = 0;
            }
        }
    }

    return compressed_bwt_vec;
}

vector <uint16_t> bwt_uncompress16(vector <uint16_t> compressed_bwt_vec)
{
    vector <vector <uint16_t>> result_vec(compressed_bwt_vec.size()); 

    for (size_t i = 0; i < compressed_bwt_vec.size();  ++i)
    {
        size_t j = 0;
        for(auto bwt_sym : compressed_bwt_vec)
        {
            if (i == 0)
                result_vec[j].push_back(bwt_sym);
            else
                result_vec[j].insert(result_vec[j].begin(), bwt_sym);

            ++j;
        }

        sort(result_vec.begin(), result_vec.end());
    }

    size_t tail_flag = 1;
    size_t head_count = 0;
    vector <uint16_t> uncompressed_bwt_vec;
    for (auto bwt_sym : result_vec[0])
    {
        if (bwt_sym == 257)
        {
            tail_flag = 0;
        }
        else
        {
            if (tail_flag == 1)
            {
                uncompressed_bwt_vec.push_back(bwt_sym);
            }
            else
            {
                uncompressed_bwt_vec.insert(uncompressed_bwt_vec.begin() + head_count, bwt_sym);
                ++head_count;
            }
        }
    }

    return uncompressed_bwt_vec;
}

string bwt_convert_to_string(vector <uint16_t> uncompressed_bwt_vec)
{
    vector <uint16_t> vec_to_compress;
    string uncompressed_string = "";

    for (auto bwt_sym : uncompressed_bwt_vec)
    {
        char char_bwt_sym = (char)bwt_sym;
        uncompressed_string = uncompressed_string + char_bwt_sym;
    }

    return uncompressed_string;
}

