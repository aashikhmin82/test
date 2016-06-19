CXXFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: min_stp

min_stp: min_stp.o kruskal_alg.o
	$(CXX) -o $@ $^ $(LDFLAGS)

min_stp.o: min_stp.cpp graph_help.h graph_map.h

kruskal_alg.o: kruskal_alg.h kruskal_alg.cpp graph_help.h graph_map.h

test: graph_test
	./graph_test

graph_test: graph_test.o topological_sort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

graph_test.o: graph_test.cpp graph_element.h graph_help.h graph_map.h

clean:
	rm -f min_stp *.o
