CXXFLAGS = -std=c++11 -Wall

all: bin_tree

tree.pdf: bin_tree FORCE
	rm -f tree.{dot,pdf}
	echo "10 dump add_value 777 add_value 888 dump exit" | tr ' ' '\n' | ./bin_tree

FORCE:
