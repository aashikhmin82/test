CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: reverse_list

reverse_list: reverse_list.o
	$(CXX) -o $@ $^ $(LDFLAGS)

reverse_list.o: reverse_list.cpp randoms_list.h node.h

clean:
	rm -f reverse_list *.o
