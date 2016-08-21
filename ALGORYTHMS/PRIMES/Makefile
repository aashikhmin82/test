CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lboost_program_options -lntl

all: expand_number

expand_number: expand_number.o
	$(CXX) -o $@ $^ $(LDFLAGS)

expand_number.o: expand_number.cpp debug.h primes.h

clean:
	rm -f expand_number *.o
