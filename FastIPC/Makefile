CXX = g++
CXXFLAGS = -g -Wall -lrt
CPPFLAGS = -I.


OUT = get put

all: $(OUT)

ipc.o: ipc.cpp ipc.h

get: get.o ipc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

put: put.o ipc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.o $(OUT)
