CXXFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: graph_exp

graph_exp: graph_exp.o
	$(CXX) -o $@ $^ $(LDFLAGS)

graph_exp.o: graph_exp.cpp graph_element.h graph_help.h

clean:
	rm -f graph_exp *.o
