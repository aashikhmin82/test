CXXFLAGS = -std=c++11 -Wall -ggdb

all: tree
#all: bin_tree

tree: tree.o bin_tree.o dot.o
	$(CXX) -o $@ $^

test_tree: test_tree.o bin_tree.o dot.o
	$(CXX) -o $@ $^

#bin_tree: bin_tree.o dot.o
#	$(CXX) -o $@ $^

tree.o: node.h tree.cpp

test_tree.o: node.h test_tree.cpp

bin_tree.o: node.h bin_tree.h bin_tree.cpp

dot.o: node.h dot.h dot.cpp

%.dot: clean bin_tree FORCE
	echo "10 dump add_value 777 add_value 888 add_value 666 search_value 777 delete_value 777 search_value 777 delete_value 777 add_value 777 add_value 777 add_value 777 exit" | tr ' ' '\n' | ./bin_tree || true

%.pdf: %.dot
	dot -Tpdf $< > $@
	evince $@

# У меня под Mint'ом dot создаёт битые PDF-ки, в которых показывается только последняя страница.
# Если у тебя те же проблемы, используй формат PostScript.
# Его умеет показывать evince.
%.ps: %.dot
	dot -Tps $< > $@
	evince $@

clean:
	rm -f bin_tree *.o *.dot *.pdf *.ps

FORCE:
