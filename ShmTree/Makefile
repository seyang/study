CXX = g++
CXXFLAGS = -g -Wall -lrt
CPPFLAGS = -I.


OUT = ShmTree

all: $(OUT)

ShmTree: ShmTree.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.o $(OUT)
