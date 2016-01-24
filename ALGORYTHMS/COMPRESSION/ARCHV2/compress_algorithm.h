#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector <uint16_t> convert_to_utf16(string str_el);
vector <uint16_t> bwt_compress16(vector <uint16_t> vec_to_compress);
vector <uint16_t> rle_compress16(vector <uint16_t> compress_bwt_vec);
vector <uint16_t> rle_uncompress16(vector <uint16_t> compressed_rle_vec);
vector <uint16_t> bwt_uncompress16(vector <uint16_t> compressed_bwt_vec);
string bwt_convert_to_string(vector <uint16_t> uncompressed_bwt_vec);
