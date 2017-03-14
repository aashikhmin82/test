CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lpthread

all: search_anagram

search_anagram: search_anagram.o read_and_search.o
	$(CXX) -o $@ $^ $(LDFLAGS)

search_anagram.o: search_anagram.cpp debug.h read_and_search.h

search_anagram_tests: search_anagram_tests.o read_and_search.o
	$(CXX) -o $@ $^ $(LDFLAGS)

search_anagram_tests.o: search_anagram_tests.cpp debug.h read_and_search.h

clean:
	rm search_anagram search_anagram_tests *.o
