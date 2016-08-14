CXXFLAGS = -std=c++11 -Wall -ggdb
LDFLAGS = -lboost_program_options -lntl

all: rsa_create_keys

rsa_encrypt_string: rsa_encrypt_string.o rsa_key.o
	$(CXX) -o $@ $^ $(LDFLAGS)

rsa_create_keys: rsa_create_keys.o rsa_key.o
	$(CXX) -o $@ $^ $(LDFLAGS)

rsa_create_keys.o: rsa_create_keys.cpp debug.h

rsa_key.o: rsa_key.h rsa_key.cpp

clean:
	rm -f rsa_create_keys *.o
