CXXFLAGS = -std=c++11 -Wall -ggdb 

all: rsa_brutforce

rsa_brutforce: rsa_brutforce.o ../rsa_key.o
	$(CXX) -o $@ $^ -lntl

rsa_brutforce.o: rsa_brutforce.cpp

../rsa_key.o: ../rsa_key.h ../rsa_key.cpp
