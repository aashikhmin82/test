CXXFLAGS = -std=c++11 -Wall -ggdb 

all: rsa_create_keys

rsa_encrypt_string: rsa_encrypt_string.o rsa_key.o
	$(CXX) -o $@ $^ -lntl

rsa_create_keys: rsa_create_keys.o rsa_key.o
	$(CXX) -o $@ $^ -lntl

rsa_create_keys.o: rsa_create_keys.cpp

rsa_key.o: rsa_key.h rsa_key.cpp
