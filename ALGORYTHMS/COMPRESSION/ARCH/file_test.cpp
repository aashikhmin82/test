#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

#include "huffman_tree.h"

using namespace std;

string bwt_compress(string test_str)
{
    vector < string > vec_to_sort;
    vec_to_sort.push_back(test_str);
    size_t sym_count = 0;
    while (sym_count < test_str.size() - 1)
    {
        string changed_str = vec_to_sort.back();
        char add_char = vec_to_sort.back().back();
        changed_str.erase(changed_str.rbegin().base() - 1);
        changed_str = add_char + changed_str;
        vec_to_sort.push_back(changed_str);
        ++sym_count;
    }

    sort(vec_to_sort.begin(), vec_to_sort.end());

    cout << "Result : " << endl;
    size_t r_count = 0;
    string result_string = "";
    for (auto r_str : vec_to_sort)
    {
        cout << r_count << " : " << r_str << endl;
        result_string = result_string + r_str.back();
        ++r_count;
    }

    return result_string;
}

string bwt_uncompress(string compressed_string)
{
    cout << "Compressed string : " << compressed_string << endl;
    vector <string> init_vector;
    for(auto compressed_letter : compressed_string)
    {
        string compressed_letter_string(1,compressed_letter);
        init_vector.push_back(compressed_letter_string);
    }

    vector <string> result_vector = init_vector;
    size_t sym_count = 0;
    while(sym_count < compressed_string.size() - 1)
    {
        vector <string> vec_to_sort = result_vector;
        sort(vec_to_sort.begin(), vec_to_sort.end());
        size_t strings_count = 0;
        while (strings_count < compressed_string.size())
        {
            result_vector[strings_count] = init_vector[strings_count] + vec_to_sort[strings_count];
            ++strings_count;
        }
        ++sym_count;
    }

    cout << "Result Vector : " << endl;
    for(auto result_strings : result_vector)
    {
        cout << result_strings << endl;
    }

    string uncompressed_string_tmp = result_vector.back();
    string uncompressed_string = "";
    size_t switch_flag = 0;
    string uncompressed_string_begin = "";
    for (auto uncompressed_letter : uncompressed_string_tmp)
    {
        if (uncompressed_letter == '0')
            switch_flag = 1;
        else
        {
            if (switch_flag == 0)
                uncompressed_string.push_back(uncompressed_letter);
            else
                uncompressed_string_begin.push_back(uncompressed_letter);
        }
    }

    uncompressed_string = uncompressed_string_begin + uncompressed_string;
    cout << "Uncompressed Word : " << uncompressed_string << endl;

    return uncompressed_string;
}

string rle_compress(string compress_string)
{
    cout << "Copress value : " << compress_string << endl;
    string rle_compress_tmp = "";
    int iter_count = 1;
    int non_iter_count = 1;
    string rle_substr = "";
    string rle_string_part, rle_string;
    char prev_char;
    for (char rle_letter : compress_string)
    {
        cout << "Char1 : " << rle_letter << " <-> " << prev_char << endl;
        if (prev_char == rle_letter)
        {
            if (non_iter_count < 0)
            {
                stringstream ss;
                ss << non_iter_count << rle_substr;
                rle_string_part = ss.str();
                cout << "[DEBUG] SubStr : " << rle_string_part << endl;
                non_iter_count = 1;
                rle_string = rle_string + rle_string_part;
                cout << "[DEBUG] NON Iter Count : " << rle_string << endl;
            }
            cout << "Char : " << rle_letter << " <-> " << prev_char << endl;
            ++iter_count;
            stringstream ss;
            ss << rle_substr << iter_count;
            rle_substr = ss.str();
        }
        else
        {
            if (iter_count > 1)
            {
                stringstream ss1;
                ss1 << iter_count << prev_char;
                rle_string_part = ss1.str();
                iter_count = 1;
                cout << "[DEBUG] Iter Count0 : " << rle_string << "\t STR Part: " << rle_string_part << endl;
                rle_string = rle_string + rle_string_part;
                cout << "[DEBUG] Iter Count0 : " << rle_string << endl;
            }

            if (non_iter_count == 0)
                rle_substr = prev_char;
            else if (non_iter_count < 0)
                rle_substr = rle_substr + prev_char;

            --non_iter_count;
        }
        prev_char = rle_letter;
        cout << "SUBSTR : " << rle_substr << endl;
    }

    if (iter_count > 1)
    {
        stringstream ss1;
        ss1 << iter_count << prev_char;
        rle_string = rle_string + ss1.str();
    }

    if (non_iter_count < 1)
    {
        if (non_iter_count == 0)
            rle_substr = prev_char;
        else if (non_iter_count < 0)
            rle_substr = rle_substr + prev_char;

        --non_iter_count;
        stringstream ss;
        ss << non_iter_count << rle_substr;
        cout << "[DEBUG SS] ss.str : " << ss.str() << endl;
        rle_string = rle_string + ss.str();
        cout << "[DEBUG RLE] : " << rle_string << endl;
    }

    cout << "RESULT RLE STRING : " << rle_string << endl;
    return rle_string;
}

string rle_uncompress(string rle_compressed_value)
{
    string rle_uncompressed_value;

    size_t count_i = 0;
    string string_to_int;
    int char_count = 0;
    for (auto compressed_char : rle_compressed_value)
    {
        cout << "[DD] " << count_i << " : " << compressed_char << endl;
        if (compressed_char == '-' and char_count == 0)
        {
            cout << "Minus" << endl;
            string_to_int = "-";
        }
        else if (compressed_char >= '0' and compressed_char <= '9' and char_count == 0)
        {
            cout << "The value is number" << endl;
            string_to_int = string_to_int + compressed_char;
        }
        else
        {
            if (string_to_int != "")
            {
                char_count = stoi(string_to_int);
                cout << "STR VALUE : " << string_to_int << "  => Int Value : " << char_count << endl;
                string_to_int = "";
            }

            if (char_count < 0)
            {
                rle_uncompressed_value = rle_uncompressed_value + compressed_char;
                ++char_count;
            }
            else
                while (char_count > 0)
                {
                    rle_uncompressed_value = rle_uncompressed_value + compressed_char;
                    --char_count;
                }
        }

        ++count_i;
    }

    cout << "Result Uncompressed Value : " << rle_uncompressed_value << endl;
    return rle_uncompressed_value;
}


int main(int argc, char* arg_vec[])
{
    map<char, size_t> symbol_count;
    string arch_filename = "archtestfile.brh";

    if (argc < 2 )
    {
        cerr << "HELP: " << endl;
        cerr << "./programm file1" << endl;
        return 1;
    }

    string file_name1 = arg_vec[1];
    string str_el1 = "";

    cout << "[DEBUG] File name 1: " << file_name1 << endl;

    ifstream config_fl1(file_name1);
    getline(config_fl1,str_el1);

    cout << "[DEBUG] String1 : " << str_el1 << endl;
    str_el1 = str_el1 + "0";
    
    string bwt_compressed_string = bwt_compress(str_el1);
    string rle_compressed_string = rle_compress(bwt_compressed_string);

    cout << "[DEBUG] RLE_COMPRESSED_STRING : " << rle_compressed_string << endl;

    for (size_t i = 0; i < rle_compressed_string.size(); ++i)
    {
        cout << "[" << rle_compressed_string[i] << "]" << " ";
        if (rle_compressed_string[i] == ' ')
            rle_compressed_string[i] = 'S';

        if (symbol_count[rle_compressed_string[i]])
            ++symbol_count[rle_compressed_string[i]];
        else
            symbol_count[rle_compressed_string[i]] = 1;
    }

    cout << endl;

    multimap<size_t, char> probability_symbol;
    cout << "[DEBUG] MAP : ";
    for (auto uniq_symbol : symbol_count)
    {
        cout << uniq_symbol.first << " ";
        cout << symbol_count[uniq_symbol.first] << endl;
        probability_symbol.insert(pair<size_t, char>(symbol_count[uniq_symbol.first],uniq_symbol.first));
    }
    //Service symbol. Should be fixed!!!
    probability_symbol.insert(pair<size_t, char>(1,'X'));

    huffman_tree huffman_codec(probability_symbol);
    huffman_codec.create_archive_string(rle_compressed_string);
    huffman_codec.save_compressed_string(arch_filename);


    string huffman_result_string = huffman_codec.read_compressed_string(arch_filename);
    string uncompressed_rle_value = rle_uncompress(huffman_result_string);
    string result_string = bwt_uncompress(uncompressed_rle_value);

    cout << "\n\nRESULT:\n";
    cout << result_string << endl;

    return 0;
}
