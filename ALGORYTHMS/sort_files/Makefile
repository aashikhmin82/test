CXXFLAGS = -std=c++14 -Wall -ggdb

all: sort_files

#sort_files: sort_files.o primes.o
sort_files: sort_files.o files.o qsort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sort_files.o: sort_files.cpp primes.h myhash.h files.h qsort.h debug.h

#primes.o: primes.cpp primes.h

qsort_test : qsort_test.o qsort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

qsort_test.o: qsort_test.cpp qsort.h

hash_test : hash_test.o
	$(CXX) -o $@ $^ $(LDFLAGS)

hash_test.o: hash_test.cpp primes.h myhash.h debug.h

files.o: files.cpp files.h
qsort.o: qsort.cpp qsort.h

clean:
	rm -f sort_files qsort_test hash_test *.o

test: qsort_test
	./qsort_test

htest: hash_test
	./hash_test
