CXXFLAGS = -std=c++11 -Wall -ggdb

all: file_test

test: autotest_compression
	./autotest_compression

test1: autotest_algorithm
	./autotest_algorithm

file_test: file_test.o compress_algorithm.o
	$(CXX) -o $@ $^

aacompress: aacompress.o compress_algorithm.o huffman_tree16.o
	$(CXX) -o $@ $^

aaucompress: aaucompress.o compress_algorithm.o huffman_tree_uncompress.o
	$(CXX) -o $@ $^

autotest_algorithm: autotest_algorithm.o compress_algorithm.o
	$(CXX) -o $@ $^

autotest_compression: autotest_compression.o
	$(CXX) -o $@ $^

file_test.o: huffman_tree16.h huffman_tree16.cpp huffman_tree_element16.h file_test.cpp

aacompress.o: huffman_tree_element16.h aacompress.cpp

aaucompress.o: aaucompress.cpp

autotest_algorithm.o: autotest_algorithm.cpp

autotest_compression.o: autotest_compression.cpp

compress_algorithm.o: compress_algorithm.h compress_algorithm.cpp

huffman_tree16.o: huffman_tree16.h huffman_tree16.cpp

huffman_tree_uncompress.o: huffman_tree_uncompress.h huffman_tree_uncompress.cpp
