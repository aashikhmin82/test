CXXFLAGS = -std=c++14 -Wall -ggdb 

all: graph_exp

graph_exp: graph_exp.o graph_element.h
	$(CXX) -o $@ $^

graph_exp.o: graph_exp.cpp
