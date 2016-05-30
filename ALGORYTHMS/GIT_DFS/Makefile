CXXFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: graph_exp

graph_exp: graph_exp.o topological_sort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

graph_exp.o: graph_exp.cpp graph_element.h graph_help.h graph_map.h

topological_sort.o: topological_sort.h topological_sort.cpp graph_help.h graph_map.h

test: graph_test
	./graph_test

graph_test: graph_test.o topological_sort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

graph_test.o: graph_test.cpp graph_element.h graph_help.h graph_map.h

clean:
	rm -f graph_exp *.o
