#include <fstream>
#include <iostream>
#include <thread>

#include "debug.h"
#include "read_and_search.h"

using namespace std;

int main() {
    const bool debug_enable { true };
    print_debug debug(debug_enable);

    string in_filename { "dictionary.txt" };
    debug << "Filename " << in_filename << "\n";

    bool get_file_thread_finished { false };
    anagram_map_t anagrams_map;

    thread find_anagrams(get_file_content, in_filename,
                             ref(get_file_thread_finished), ref(anagrams_map), ref(debug));

    find_anagrams.join();

    print_map(anagrams_map);
}
