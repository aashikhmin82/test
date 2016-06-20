CXXFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: min_stp

min_stp: min_stp.o kruskal_alg.o
	$(CXX) -o $@ $^ $(LDFLAGS)

min_stp.o: min_stp.cpp graph_help.h graph_map.h

kruskal_alg.o: kruskal_alg.h kruskal_alg.cpp graph_help.h graph_map.h

test: test_stp
	./test_stp

test_stp: test_stp.o kruskal_alg.o
	$(CXX) -o $@ $^ $(LDFLAGS)

test_stp.o: test_stp.cpp graph_help.h graph_map.h

clean:
	rm -f min_stp *.o
