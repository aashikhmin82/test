CXXFLAGS = -std=c++11 -Wall -ggdb

all: file_test

file_test: file_test.o
	$(CXX) -o $@ $^

file_test.o: huffman_tree.h huffman_tree_element.h file_test.cpp
