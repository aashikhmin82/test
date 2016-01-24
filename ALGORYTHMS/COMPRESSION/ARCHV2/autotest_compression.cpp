#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <random>
#include <cstdlib>

using namespace std;

default_random_engine re;

string random_string(size_t max_size)
{
    uniform_int_distribution<size_t> random_size(1, max_size);
    uniform_int_distribution<char> random_letter('\n','z');

    string s(random_size(re), '?');

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

int main(int argc, char* arg_vec[])
{
    srand( static_cast<size_t>(time(nullptr)) );
    size_t test_count = 20;
    string filename_to_compress = "autotest_file.first";
    string filename_compressed = "autotest_file.cz";
    for (size_t j = 0; j < test_count; ++j)
    {
        size_t random_strings_count = rand() % 10;
        ofstream text_file(filename_to_compress);
        for (size_t k = 0; k < random_strings_count; ++k)
        {
            string random_str = random_string(50);

            text_file << random_str;
        }

        text_file.close();

//FIXME implement parameters 
        system("./aacompress autotest_file.first autotest_file.cz");
        system("./aaucompress autotest_file.cz autotest_file.ucz");
        size_t exit_code = system("diff autotest_file.first autotest_file.ucz");

        assert (exit_code == 0);
    }

    cout << "\nPASSED" << endl;

    return 0;
}
