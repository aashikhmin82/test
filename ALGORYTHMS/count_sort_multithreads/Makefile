CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lpthread

all: sort_numbers

sort_numbers: sort_numbers.o read_and_sort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sort_numbers.o: sort_numbers.cpp debug.h read_and_sort.h constants.h

sort_numbers_test: sort_numbers_test.o read_and_sort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sort_numbers_test.o: sort_numbers_test.cpp debug.h read_and_sort.h constants.h

read_and_sort.o: read_and_sort.cpp read_and_sort.h constants.h

clean:
	rm sort_numbers sort_numbers_test *.o
