CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lboost_program_options

all: queue

queue: queue.o
	$(CXX) -o $@ $^ $(LDFLAGS)

queue.o: queue.cpp stack.h myqueue.h

clean:
	rm -f queue *.o
