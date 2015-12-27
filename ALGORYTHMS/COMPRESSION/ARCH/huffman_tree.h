#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <bitset>

#include "huffman_tree_element.h"

using namespace std;

class huffman_tree
{
    private:
        multimap<size_t, char> symbol_frequency_map;
        multimap<size_t, shared_ptr<class_huffman_tree_element>> huffman_tree_map;
        map<char, vector<size_t>> huffman_code_map;
        map<string, vector<size_t>> huffman_code_map_string;
        vector <size_t> compressed_string;
        map<size_t, char> huffman_code_to_char_map;
        map<string, string> huffman_code_to_string_map;

    public:
        explicit huffman_tree (multimap<size_t, char> probability_symbol) : symbol_frequency_map(probability_symbol)
        {
            size_t multimap_count = 0;
            size_t multimap_size = symbol_frequency_map.size();
            multimap <size_t, shared_ptr<class_huffman_tree_element>> huffman_tree_result;
            while (multimap_count < multimap_size or huffman_tree_result.size() != 1)
            {
                cout << "\n\n[DEBUG] MM_count : " << multimap_count << "  MM_size : " << multimap_size << endl;
                cout << "[DEBUG] Huf_Tree_Result.size() : " << huffman_tree_result.size() << endl; 
                size_t summ_count;
                shared_ptr <class_huffman_tree_element> first_map_object;
                shared_ptr <class_huffman_tree_element> second_map_object;

                if (multimap_count == multimap_size
                            or ((*huffman_tree_result.begin()).first 
                                and (*huffman_tree_result.begin()).first <= (*symbol_frequency_map.begin()).first))
                {
                    cout << "[DEBUG] 2L Tree Iterator1 : First -> " << (*huffman_tree_result.begin()).first;
                    cout << "\tSecond -> " << (*huffman_tree_result.begin()).second << endl;
                    cout << "[DEBUG] Freq Sym Map : " << (*symbol_frequency_map.begin()).first << endl;
                    cout << "[DEBUG] Sym Freq Map Size() : " << symbol_frequency_map.size() << endl;
                    cout << "[DEBUG] MM Size : " << multimap_size << "  MM Count : " << multimap_count << endl;
                    first_map_object = (*huffman_tree_result.begin()).second;

                    if ((((*++huffman_tree_result.begin()).first != 1) and (*++huffman_tree_result.begin()).first <= (*symbol_frequency_map.begin()).first) or (symbol_frequency_map.size() == 0))
                    {
                        cout << "[DEBUG_1] Huff: " << (*++huffman_tree_result.begin()).first << endl;
                        second_map_object = (*++huffman_tree_result.begin()).second;
                        huffman_tree_result.erase(++huffman_tree_result.begin());
                        cout << "[DEBUG_1] huff2 : " << second_map_object->sym_count() << endl;
                    }
                    else
                    {
                        shared_ptr <class_huffman_tree_element> map_object_tmp2(make_shared<class_huffman_tree_element>((*symbol_frequency_map.begin()).first,(*symbol_frequency_map.begin()).second));
                        second_map_object = map_object_tmp2;
                        cout << "[DEBUG] F1" << endl;

                        //DEBUG
                        cout << "DEBUG ON" << endl;
                        for (auto it = symbol_frequency_map.begin(); it != symbol_frequency_map.end(); ++it)
                        {
                            cout << (*it).first << " " << (*it).second << endl;
                        }
                        cout << "DEBUG OFF" << endl;
                        //DEBUG

                        symbol_frequency_map.erase(symbol_frequency_map.begin());
                        cout << "[DEBUG] F2" << endl;
                        ++multimap_count;
                        cout << "[DEBUG_2] huff2 : " << second_map_object->sym_count() << endl;
                    }
                    huffman_tree_result.erase(huffman_tree_result.begin());
                }
                else
                {
                    shared_ptr <class_huffman_tree_element> map_object_t1(make_shared<class_huffman_tree_element>((*symbol_frequency_map.begin()).first,(*symbol_frequency_map.begin()).second));
                    cout << "[DEBUG] 1L Iterator1 : First -> " << (*symbol_frequency_map.begin()).first;
                    cout << "\tSecond -> " << (*symbol_frequency_map.begin()).second << "\n" << endl;
                    first_map_object = map_object_t1;
                    ++multimap_count;

                    cout << "[DB0] " << huffman_tree_result.size() << endl;
                    if (((huffman_tree_result.size() > 0) 
                                and ((*huffman_tree_result.begin()).first != 1) 
                                and ((*huffman_tree_result.begin()).first <= (*++symbol_frequency_map.begin()).first))
                            or (symbol_frequency_map.size() == 1))
                    {
                        cout << "[DEBUG_1a] Huff: " << (*huffman_tree_result.begin()).first << endl;
                        second_map_object = (*huffman_tree_result.begin()).second;
                        huffman_tree_result.erase(huffman_tree_result.begin());
                        cout << "[DEBUG_1a] huff2 : " << second_map_object->sym_count() << endl;
                    }
                    else
                    {
                        cout << "[DEBUG_2a] huff2 FLAG1" << endl;
                        cout << "[DEB_2a] SiZe : " << symbol_frequency_map.size() << endl;
                        shared_ptr <class_huffman_tree_element> map_object_tmp2(make_shared<class_huffman_tree_element>((*++symbol_frequency_map.begin()).first,(*++symbol_frequency_map.begin()).second));
                        second_map_object = map_object_tmp2;
                        cout << "[DEBUG_2A] SFM : " << (*++symbol_frequency_map.begin()).first << endl;
                        symbol_frequency_map.erase(symbol_frequency_map.begin());
                        ++multimap_count;
                        cout << "[DEBUG_2a] huff2 : " << second_map_object->sym_count() << endl;
                    }

                    cout << "[DEBUG] Sym_count1() " << first_map_object->sym_count() << endl;
                    cout << "[DEB] SFM.Begin() : " << (*symbol_frequency_map.begin()).first << endl;
                    symbol_frequency_map.erase(symbol_frequency_map.begin());
                }

                cout << "[DEBUG_TMP] SYM_COUNT: " << first_map_object->sym_count() << endl;
                cout << "[DEBUG_TMP1] SYM_COUNT: " << second_map_object->sym_count() << endl;
                summ_count = first_map_object->sym_count() + second_map_object->sym_count();
                cout << "[DEBUG_SUMM] SUMM : " << summ_count << endl;
                shared_ptr <class_huffman_tree_element> map_object_summ(make_shared<class_huffman_tree_element>(summ_count,'Z'));
                map_object_summ->weak_left_down() = first_map_object;
                map_object_summ->weak_right_down() = second_map_object;
                first_map_object->right_up() = map_object_summ;
                second_map_object->left_up() = map_object_summ;
                huffman_tree_result.insert(pair<size_t, shared_ptr<class_huffman_tree_element>>(summ_count, map_object_summ));
            }

            cout << "[DEBUG] Tree Debug Begin : " << endl;
            cout << (*huffman_tree_result.begin()).first << endl;
            cout << (*huffman_tree_result.begin()).second->sym_letter() << endl;
            vector<size_t> code_tmp(0);

            create_huffman_map((*huffman_tree_result.begin()).second, code_tmp);
        }

    void create_huffman_map(shared_ptr<class_huffman_tree_element> huffman_tree_element, vector<size_t> code)
    {
        size_t code_unit = 0;
        cout << "[DEBUG_F1] PRINT MAP FUNC Tree Debug Begin : " << endl;
        cout << huffman_tree_element->sym_letter() << endl;
        if (huffman_tree_element->left_down())
        {
            cout << "[DEBUG_F] left_down : " << huffman_tree_element->left_down()->sym_letter() << " -> " << code_unit <<endl;
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
                {
                    cout << code_int;
                }
                cout << ")" << endl;
            }
            cout << "[DEBUG LETTER] : " << huffman_tree_element->left_down()->sym_letter() << endl;
            //MUST BE FIXED!!!
            if (huffman_tree_element->left_down()->sym_letter() != 'Z')
            {
                huffman_code_map.insert(pair<char, vector<size_t>>(huffman_tree_element->left_down()->sym_letter(), code_left));
                cout << "[DEBUG LETTER] Insert : " << huffman_tree_element->right_down()->sym_letter() << endl;
          }
        }

        
        if (huffman_tree_element->right_down())
        {
            ++code_unit;
            cout << "[DEBUG_F] right_down : " << huffman_tree_element->right_down()->sym_letter() << " -> " << code_unit << endl;
            vector<size_t> code_right = code;
            code_right.push_back(code_unit);
            if ((huffman_tree_element->right_down()->left_down())
                or (huffman_tree_element->right_down()->right_down()))
            {
                create_huffman_map(huffman_tree_element->right_down(),code_right);
            }
            else
            {
                cout << "[DEBUG] BinCode : " << bitset<8>(huffman_tree_element->right_down()->sym_letter()) << endl;
                cout << "[DEBUG] CODE : " << huffman_tree_element->right_down()->sym_letter() << "  (";
                for (auto code_int : code_right)
                {
                    cout << code_int;
                }
                cout << ")" << endl;
                cout << "[DEBUG LETTER1] : " << huffman_tree_element->right_down()->sym_letter() << endl;
                // Must be fixed!!!!
                if (huffman_tree_element->right_down()->sym_letter() != 'Z')
                    huffman_code_map.insert(pair<char, vector<size_t>>(huffman_tree_element->right_down()->sym_letter(), code_right));
                cout << "[DEBUG1] CodeMap Size : " << huffman_code_map.size() << endl;
            }
        }
    }

    void create_archive_string(string string_to_compress)
    {
        cout << "[DEBUG] CodeMap Size : " << huffman_code_map.size() << endl;
        for (auto code_map_element : huffman_code_map)
        {
            cout << "[DEBUG] code_map_element : " << code_map_element.first << "  ->  ";
        }

        size_t compressed_value = 0;
        size_t bytes_count = 0;

        //Add unused symbol to the compressed string FIXME
        string_to_compress = string_to_compress + 'X';

        for (size_t i = 0; i < string_to_compress.size(); ++i)
        {
            cout << "[DEBUG] Symbol : " << string_to_compress[i] << endl;
            cout << "\t" << endl;
            for (auto code_int_tmp : huffman_code_map[string_to_compress[i]])
            {
                compressed_value = compressed_value << 1;
                ++bytes_count;
                if (code_int_tmp == 1)
                    ++compressed_value;
                cout << code_int_tmp;

                if (bytes_count == 32)
                {
                    cout << "[DEBUG] Push Back : " << compressed_value << endl;
                    cout << "[DEBUG] SAVE VALUE, NEW VALUE OPEN." << endl;
                    compressed_string.push_back(compressed_value);
                    compressed_value = 0;
                    bytes_count = 0;
                }
            }
            cout << endl;
            cout << endl;
            cout << "[DEBUG] Bitset Value : " << bitset<32>(compressed_value) << endl;
        }
        cout << "[DEBUG] Bytes_Count : " << bytes_count << endl;
        cout << "[DEBUG] Compr Val : " << bitset<32>(compressed_value) << endl;
        compressed_value = compressed_value << (32 - bytes_count);
        cout << "[DEBUG] Compr ValNew : " << bitset<32>(compressed_value) << endl;
        compressed_string.push_back(compressed_value);

        cout << "[DEBUG] Result string : ";
        for (auto compressed_value_unit : compressed_string)
        {
            cout << compressed_value_unit << endl;
        }
        cout << endl;
    }

    string decompress_value(vector <size_t> compressed_vector)
    {
        cout << "[DEBUG] Decompress : " << endl;
        string result_uncompressed_value = "";
        string search_value = "";
        for (auto compressed_value_unit : compressed_vector)
        {
            cout << "[DEBUG] Compressed val : " << compressed_value_unit << endl;
            size_t bit_offset = 31;
            size_t bit_count = 1;
            size_t bit_size = 32;
            while (bit_count < 33)
            {
                --bit_size;
    
                size_t compressed_value_unit_byte = compressed_value_unit >> bit_size;
                cout << "[DEBUG] Byte1 : " << compressed_value_unit_byte << "   BIT SIZE : " << bit_size << endl;
                cout << "  COMP VAL UNIT : " << bitset<32>(compressed_value_unit) << endl;
                compressed_value_unit = compressed_value_unit - (compressed_value_unit_byte << bit_size);
                cout << "  COMP VAL UNI1 : " << bitset<32>(compressed_value_unit) << endl;
                search_value = search_value + to_string(compressed_value_unit_byte);

                if (huffman_code_to_string_map[search_value] == "X")
                {
                        bit_count = 34;
                }
                else if (huffman_code_to_string_map[search_value] != "")
                {
                    cout << "[DEBUG] FOUND!!!!! : " << search_value << "  [[[[[ " << huffman_code_to_string_map[search_value] << " ]]]]]" << endl;
                    string res111 = huffman_code_to_string_map[search_value];
                    result_uncompressed_value = result_uncompressed_value + huffman_code_to_string_map[search_value];
                    cout << "[DEBUG] ======== " << result_uncompressed_value << endl;
                    search_value = "";
                } 

                ++bit_count;
                --bit_offset;
            }
        }
        cout << endl;

        return result_uncompressed_value;
    }

    void save_compressed_string(string filename)
    {
        cout << "Write to the file  : " << filename << endl;

        ofstream arch_file(filename);

        for (auto code_map_element : huffman_code_map)
        {
            size_t code_num = 0;
            cout << "[DEBUG DECOMPRESS] code_map_element : " << code_map_element.first << "  ->  ";
            for (auto bin_code : code_map_element.second)
            {
                cout << bin_code;
                code_num = code_num << 1;
                if (bin_code == 1)
                    ++code_num;
            }
            cout << "[DEBUG] CodeToChar Map : " << code_num;
            cout << " --> " << code_map_element.first << endl;
            cout << endl;

            arch_file << code_map_element.first << " ";
            for (auto bin_code : code_map_element.second)
                arch_file << bin_code;
            arch_file << " ";
        }
        arch_file << endl;


        for (auto compressed_value_unit : compressed_string)
        {
            arch_file << compressed_value_unit << endl;
        }
        arch_file.close();
    }

    string read_compressed_string(string filename)
    {
        cout << "Read from file : " << filename << endl;
        string compressed_value_string;
        string arch_map_string, arch_map_string1;
        ifstream arch_file(filename);

        getline(arch_file,arch_map_string);
        cout << "[DEBUG] Map String from Arch File : " << arch_map_string << endl;
        istringstream iss(arch_map_string);
        string token1;
        string token2;
        while (getline(iss, token1, ' '))
        {
            getline(iss, token2, ' ');
            huffman_code_to_string_map.insert(pair<string, string>(token2, token1));
        }

        vector <size_t> compressed_value_vector;
        while (getline(arch_file,compressed_value_string))
        {
            cout << "String to decompress : " << compressed_value_string << "    INT value : " << stoul(compressed_value_string) << endl;
            size_t compressed_value_sizet = stoul (compressed_value_string);
            compressed_value_vector.push_back(compressed_value_sizet);
        }

        string decompressed_string = decompress_value(compressed_value_vector);
        return decompressed_string;
    }
};
