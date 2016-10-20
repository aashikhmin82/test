CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: search

search: search.o search_func.o
	$(CXX) -o $@ $^ $(LDFLAGS)

search.o: search.cpp debug.h fsm.h search_func.h

test_search: test_search.o search_func.o
	$(CXX) -o $@ $^ $(LDFLAGS)

test_search.o: test_search.cpp debug.h fsm.h search_func.h
search_func.o: search_func.cpp search_func.h

test: test_search
	./test_search

time_search: time_search.o search_func.o
	$(CXX) -o $@ $^ $(LDFLAGS)

time_search.o: time_search.cpp debug.h fsm.h search_func.h

clean:
	rm test_search search time_search *.o
