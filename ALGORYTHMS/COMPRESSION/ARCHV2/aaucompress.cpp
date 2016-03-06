#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "huffman_tree_uncompress.h"
#include "compress_algorithm.h"
#include "huffman_treeSG.h"

using namespace std;

int main(int argc, char* arg_vec[])
{

    if (argc < 3 )
    {
        cerr << "HELP: " << endl;
        cerr << "./programm arch_filename uncompressed_file" << endl;
        return 1;
    }

    string compressed_filename = arg_vec[1];
    string uncompressed_file = arg_vec[2];
    string str_el1 = "";

    //// SG Begin
    ifstream sgfile;
    sgfile.open(compressed_filename);

    stringstream str_stream;
    str_stream << sgfile.rdbuf();
    string sg_string = str_stream.str();

//    cout << "SG file : " << sg_string << endl;

//    string decompressed_string = decompress2(sg_string);
    vector <uint16_t> huffman_result_string = decompress(sg_string);
//    cout << "SG string : " << endl;
//    cout << decompressed_string;
//    return 0;
    /// End

 //   huffman_tree_uncompress huffman_uncompress(compressed_filename);
 //   vector <uint16_t> huffman_result_string = huffman_uncompress.read_compressed_string();
  
    cout << "============================== Uncopress RLE ===================" << endl;
    vector <uint16_t> uncompressed_rle_vec = rle_uncompress16(huffman_result_string);

    cout << "[DEBUG] Uncompressed RLE vector : " << endl;
    for (auto uncompr_rle_sym : uncompressed_rle_vec)
        cout << uncompr_rle_sym << " ";
    cout << endl;

    cout << "============================== Uncopress RLE End ===============" << endl;

    cout << "============================= Uncompress BWT Begin ===============" << endl;
    vector <uint16_t> uncompressed_bwt_vec = bwt_uncompress16(uncompressed_rle_vec);
    cout << "+++++++++++++++++++++++ DEBUG BWT +++++++++++++++++++++++++++++++++" << endl;
    for (auto ubwt_sym : uncompressed_bwt_vec)
        cout << ubwt_sym << " ";
    cout << endl;
    cout << "============================= Uncompress BWT End =================" << endl;
    string result_string = bwt_convert_to_string(uncompressed_bwt_vec);

//    cout << "TOTAL RESULT : " << result_string << endl;

    ofstream uncompressed_file_desc(uncompressed_file);
    uncompressed_file_desc << result_string;

    uncompressed_file_desc.close();

    return 0;
}
