CXXFLAGS = -std=c++14 -Wall -ggdb
LDFLAGS = -lpthread

all: sort

sort: sort.o multithread_qsort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sort_test: sort_test.o multithread_qsort.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sort.o: sort.cpp multithread_qsort.h

sort_test.o: sort_test.cpp multithread_qsort.h

multithread_qsort.o: multithread_qsort.cpp multithread_qsort.h

clean:
	rm sort sort_test *.o
