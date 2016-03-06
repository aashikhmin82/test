#ifndef HUFFMANSG_H_
#define HUFFMANSG_H_

using namespace std;

//string compress(string const & input, bool print);
string compress(vector <uint16_t> rle_vec);

vector <uint16_t> decompress(string const & packed);

#endif /* HUFFMANSG_H_ */
