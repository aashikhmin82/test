CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: create_primes_list

create_primes_list: create_primes_list.o
	$(CXX) -o $@ $^ $(LDFLAGS)

create_primes_list.o: create_primes_list.cpp debug.h primes_v1.h primes_v2.h primes_v3.h

clean:
	rm -f create_primes_list *.o
