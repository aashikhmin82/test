#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

using letter_t = uint16_t;
using text_t = vector<letter_t>;

letter_t char2letter(char ch)
{
            return static_cast<unsigned char>(ch);
}

char letter2char(letter_t l)
{
            return static_cast<unsigned char>(l);
}

letter_t const max_char = char2letter(numeric_limits<unsigned char>::max());
//letter_t const sentinel = max_char + 1; // Just for a demonstration purpose.
letter_t const max_letter = numeric_limits<letter_t>::max() / 2;

size_t const bits_per_char = numeric_limits<unsigned char>::digits;

static_assert(sizeof(letter_t) > sizeof(char), "choose a wider type for letter_t so it can hold more than just a char");
static_assert(bits_per_char == 8, "bits_per_char != 8, pack_bits()/unpack_bits() have to be rewritten to be platform independent");

bool is_char(letter_t l)
{
    return l <= max_char;
}

text_t char2letter(string const & text)
{
    text_t result;
    for (auto ch : text)
        result.push_back(char2letter(ch));
    return result;
}

string letter2char(text_t const & text)
{
    string result;
    for (auto l : text)
        if (is_char(l))
            result += letter2char(l);
    return result;
}

using bits_t = string;
using code_t = map<letter_t, bits_t>;
using text_codes = vector<uint16_t>;

using bit_t = char;
bit_t int2bit(uint16_t x)
{
    return '0' + (x & 1);
}

uint8_t bit2int(bit_t x)
{
    return (x - '0') & 1;
}

bits_t uint2unary(unsigned int n)
{
    bits_t result(n, int2bit(1));
    result += int2bit(0);
    return result;
}

unsigned int unary2uint(bits_t const & bits, size_t & position)
{
    unsigned int result = 0;
    for (; position < bits.size() and bits[position] == int2bit(1); ++position)
        ++result;

    if (position < bits.size())
        ++position;
    else
    {
        cerr << "Error: no terminating zero bit in a unary code.\n";
        abort();
    }

    return result;
}

uint16_t digits(uint16_t n)
{
    uint16_t result = 0;
    cout << "Digits N0 : " << n << endl;
    for (; n > 0; n >>= 1)
    {
        cout << "Digits NI : " << n << endl;
        ++result;
    }
    cout << "Digits Result : " << result << endl;
    return result;
}

bits_t uint2gamma(uint16_t n)
{
    assert(n > 0);

    uint16_t size = digits(n);

    bits_t result(size - 1, int2bit(0));

    for (uint16_t i = size; i-- > 0; )
        result += int2bit((n >> i) & 1);

    return result;
}

unsigned int gamma2uint(bits_t const & bits, size_t & position)
{
    unsigned int size = 1;
    for (; position < bits.size() and bits[position] == int2bit(0); ++position)
        ++size;

    if (bits.size() - position < size)
    {
        cerr << "Error: gamma code is truncated.\n";
        abort();
    }

    unsigned int result = 0;
    for (unsigned int i = 0; i < size; ++i, ++position)
        result = (result << 1) | bit2int(bits[position]);

    return result;
}


bits_t pack_code(code_t const & code)
{
    bits_t result = uint2gamma(code.size() + 1);
    cout << "Uint2gamma Result : " << result << "  code.size : " << code.size() <<  endl;

    size_t previous_letter = -1;

    for (auto const & c : code)
    {
        cout << "C.First : " << c.first << "   Priv : " << previous_letter << "  Dt : " << (c.first - previous_letter) << endl;
        assert(previous_letter == -1 or previous_letter < c.first);
        result += uint2gamma(c.first - previous_letter);
        cout << "Uint2Gamma Delta : " << uint2gamma(c.first - previous_letter) << endl;
        cout << "Result +letter : " << result << endl;
        previous_letter = c.first;

        result += uint2gamma(c.second.size());
        cout << "Result +code.size() : " << result << endl;

        result += c.second;
        cout << "C.second : " << c.second << endl;
        cout << "Result (step end) +code : " << result << endl;
    }

    return result;
}

class decode_t
{
    public:
        decode_t()
        {
        }

        explicit decode_t(code_t const & code)
        {
            for (auto const & c : code)
                push(c.second, c.first);
        }

        void push(bits_t const & bits, letter_t letter)
        {
            if (letter > max_letter)
            {
                cerr << "Error: letter value " << letter << " is greater than maximum " << max_letter << ".\n";
                abort();
            }

            if (bits.size() == 0)
            {
                cerr << "Error: code for letter=" << letter << " is empty.\n";
                abort();
            }

            node_index_t position = 0;
            node_index_t node = walk(bits, position);
            if (position >= bits.size())
            {
                cerr << "Error: code " << bits << " for letter=" << letter
                    << " is a prefix of another code.\n";
                abort();
            }

            trie.reserve(trie.size() + bits.size() - 1 - position);
            for (; position < bits.size() - 1; ++position)
            {
                node_index_t next = trie.size();
                trie.push_back(node_t { });
                trie[node].set_child(bits[position], next);
                node = next;
            }

            trie[node].set_letter(bits[position], letter);
        }

        letter_t decode(bits_t const & bits, size_t & position) const
        {
            size_t p = position;
            auto const & node = trie[walk(bits, p)];
            if (not node.is_leaf(bits[p]))
                return letter_t { };
            position = p + 1;
            return node.get_letter(bits[p]);
        }

        void dump(ostream & out, string const & label) const
        {
            static size_t const max_label = 80;

            out << "digraph {\n  label=\"";
            for (auto const & c : label.substr(0, max_label))
                out << escape(c);
            out << "\"\n  fontsize=32\n";

            for (node_index_t i = 0; i < trie.size(); ++i)
            {
                out << "  node_" << i << " [label = \"\", shape = point]\n";
                auto const & node = trie[i];
                for (int j = 0; j <= 1; ++j)
                {
                    if (node.is_leaf(j))
                    {
                        letter_t l = node.get_letter(j);
                        out << "  node_" << i << '_' << j << " [label = \"";
                        if (is_char(l))
                            out << escape(letter2char(l));
                        else
                            out << "letter=" << l;
                        out << "\", shape = box]\n"
                        << "  node_" << i << " -> node_" << i << '_' << j;
                    }
                    else if (node.is_nil(j))
                        out << "  node_" << i << '_' << j
                            << " [label = \"nil\", shape = circle]\n"
                            << "  node_" << i << " -> node_" << i << '_' << j;
                    else
                        out << "  node_" << i << " -> node_" << node.get_child(j);
                    out << " [label = \"" << j << "\"]\n";
                }
            }

            out << "}\n";
        }

    private:
        using node_index_t = size_t;

        static string escape(char c)
        {
            if (not isprint(c))
            {
                ostringstream result;
                result << "\\\\x" << hex << setfill('0') << setw(2)
                    << static_cast<int>(static_cast<unsigned char>(c));
                return result.str();
            }
            else if (c == '"' or c == '\\')
                return string { "\\" } + c;
            else
                return string { 1, c };
        }

        class node_t
        {
            public:
                bool is_leaf(bit_t bit) const
                {
                    return child[bit2int(bit)] > max_node;
                }
                bool is_nil(bit_t bit) const
                {
                    return child[bit2int(bit)] == nil;
                }

                bool is_child(bit_t bit) const
                {
                    node_index_t node = child[bit2int(bit)];
                    return node <= max_node and node != nil;
                }

                letter_t get_letter(bit_t bit) const
                {
                    assert(is_leaf(bit));
                    return letters_top - child[bit2int(bit)];
                }

                void set_letter(bit_t bit, letter_t letter)
                {
                    assert(letters_top - letter > max_node);
                    assert(is_nil(bit));
                    child[bit2int(bit)] = letters_top - letter;
                }

                node_index_t get_child(bit_t bit) const
                {
                    assert(is_child(bit));
                    return child[bit2int(bit)];
                }

                void set_child(bit_t bit, node_index_t node)
                {
                    assert(is_nil(bit));
                    child[bit2int(bit)] = node;
                }

            private:
                static_assert(sizeof(node_index_t) >= sizeof(letter_t), "letter_t variables can not fit in node_index_t, use a larger type for trie node index");

                static node_index_t const letters_top = numeric_limits<node_index_t>::max();
                static node_index_t const max_node = numeric_limits<node_index_t>::max() / 2;

                static_assert(letters_top - max_letter > max_node, "letter and node index ranges can not be combined into node_index_t, reduce these ranges or do not store them in one variable.");

                static node_index_t const nil = 0;

                node_index_t child[2] { nil, nil };
        };

        node_index_t walk(bits_t const & bits, node_index_t & position) const
        {
            node_index_t node = 0;
            for (; position < bits.size() and trie[node].is_child(bits[position]); ++position)
            {
                node_index_t next = trie[node].get_child(bits[position]);
                assert(node < next and next < trie.size());
                node = next;
            }
            return node;
        }

        vector<node_t> trie { node_t { } };
};

bits_t encode(text_codes const & rle_vec, code_t const & code)
{
    bits_t result;
    for (auto l : rle_vec)
    {
        auto c = code.find(l);
        if (c != code.cend())
            result += c->second;
        else
        {
//            cerr << "Can not encode letter " << l << " ("
//                << (is_char(l) ? string(1, letter2char(l)) : "not a char") << ")\n";
            abort();
        }
    }
    return result;
}

text_t decode(bits_t const & text, decode_t const & code)
{
    text_t result;

    for (size_t i = 0; i < text.size(); )
    {
        size_t position = i;
        letter_t l = code.decode(text, position);

        if (i < position)
            result.push_back(l);
        else
        {
            cerr << "Failed to decode bit sequence from position " << i << ": " << text.substr(i, 100) << "...\n";
            abort();
        }

        i = position;
    }

    return result;
}

class huffman_t
{
    public:
        size_t count;
        code_t code;

        huffman_t()
        {
        }

        huffman_t(size_t count, letter_t letter) :
            count { count },
            code { { letter, "" } }
        {
            if (letter > max_letter)
            {
                cerr << "Error: letter value " << letter << " is greater than maximum " << max_letter << ".\n";
                abort();
            }
        }

        huffman_t(huffman_t && a, huffman_t && b) :
            count { a.count + b.count }
        {
            a.prepend(int2bit(0));
            b.prepend(int2bit(1));

            swap(code, a.code);
            code.insert(b.code.cbegin(), b.code.cend());
        }

        void prepend(bit_t bit)
        {
            for (auto & c : code)
            {
                code[c.first] = bits_t(1, bit) + c.second;
            }
            cout << endl;
        }

        bool operator>(huffman_t const & other) const
        {
            return count > other.count;
        }
};

bits_t pad(bits_t const & bits, size_t boundary)
{
//    assert(boundary != 0);

    size_t pad_size = boundary - (bits.size() % boundary);
//    assert(pad_size > 0);

    bits_t result = uint2unary(pad_size - 1) + bits;

//    assert(result.size() % boundary == 0);
//    assert(result.size() - bits.size() <= boundary);

    return result;
}

decode_t unpack_code(bits_t const & bits, size_t & position)
{
    decode_t result;

    letter_t letter = -1;

    unsigned int entries = gamma2uint(bits, position) - 1;
    unsigned int i;

    for (i = 0; position < bits.size() and i < entries; ++i)
    {
        letter += gamma2uint(bits, position);

        unsigned int code_size = gamma2uint(bits, position);
        if (bits.size() - position < code_size)
        {
            cerr << "Error: code table is truncated.\n";
            abort();
        }
        bits_t code = bits.substr(position, code_size);
        position += code_size;

        cout << "[D] Code : " << code << "   Letter : " << letter << endl;

        result.push(code, letter);
    }

    if (i != entries)
    {
        cerr << "Error: failed to read code table. Found only " << i << " of " << entries << " entries.\n";
        abort();
    }

    return result;
}

bits_t unpad(bits_t const & bits)
{
    size_t position = 0;
    unary2uint(bits, position);
    return bits.substr(position);
}

string pack_bits(bits_t bits)
{
    bits = pad(bits, bits_per_char);

    string result(bits.size() / bits_per_char, '\0');

    for (size_t i = 0, j = 0; i < result.size(); ++i)
    {
        unsigned char ch = 0;
        for (size_t k = 0; k < bits_per_char; ++j, ++k)
            ch = (ch << 1) | bit2int(bits[j]);
        result[i] = ch;
    }

    return result;
}

bits_t unpack_bits(string const & in)
{
    string result(in.size() * bits_per_char, '\0');

    for (size_t i = 0, j = 0; i < in.size(); ++i)
    {
        unsigned char ch = in[i];
        for (size_t k = bits_per_char; k-- > 0; ++j)
            result[j] += int2bit((ch >> k) & 1);
    }

    return unpad(result);
}

code_t build_code(text_t const & text)
{
    vector<uint16_t> count;

    cout << "Build Code : ";
    for (auto l : text)
    {
        if (l >= count.size())
            count.resize(l + 1);
        ++count[l];
    }

    cout << "Huffman add : ";
    priority_queue<huffman_t, vector<huffman_t>, greater<huffman_t>> huffman;
    for (size_t i = 0; i < count.size(); ++i)
        if (count[i] > 0)
        {
            cout << count[i] << " -> " << i << "   ";
            huffman.push(huffman_t(count[i], i));
        }
    cout << endl;
    while (huffman.size() > 1)
    {
        //        print_queue(huffman);

        huffman_t a = huffman.top();
        huffman.pop();

        huffman_t b = huffman.top();
        huffman.pop();
        huffman.push(huffman_t(move(a), move(b)));
    }

    if (huffman.empty())
        return code_t { };

    code_t result = huffman.top().code;
    //    huffman.pop();

    cout << "Build Code Result1 : ";

//    if (result.size() == 1 and result.cbegin()->second.empty())
//        result.begin()->second = int2bit(0);

    cout << "Build Code Result : ";
    for (auto res_el : result)
    {
        cout << res_el.first << " -> " << res_el.second << "   ";
    }
    cout << endl;
    return result;
}

string compress(text_codes rle_vec)
{
    cout << "Compress1 RLE : ";
    for (auto rle_sym : rle_vec)
        cout << rle_sym << " ";
    cout << endl;
//    build_code(rle_vec);
    code_t code = build_code(rle_vec);

    bits_t packed_code = pack_code(code);
    cout << "\nPacked Code : " << packed_code << endl;

    bits_t encoded_text = encode(rle_vec, code);
    cout << "\nEncoded Text : " << encoded_text << endl;

    return pack_bits(packed_code + encoded_text);
}

//string decompress(string const & packed)
text_t decompress(string const & packed)
{
    cout << "Decompress func....: " << endl;
    bits_t unpacked = unpack_bits(packed);
    size_t position = 0;
    decode_t unpacked_code = unpack_code(unpacked, position);
    unpacked = unpacked.substr(position);

    text_t result = decode(unpacked, unpacked_code);
//    for (auto res_el : output_vec)
//        cout << "[D] Vec_El : " << res_el << endl;

//    string result = letter2char(decode(unpacked, unpacked_code));

    /*
    if (print)
        unpacked_code.dump(cerr, result);

    */
    //string result = unpacked;
    return result;
}

