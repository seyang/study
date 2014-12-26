CC = gcc
CFLAGS = -g -lcurl -ljansson -Wl,-rpath=/usr/local/lib
CXX = g++
CXXFLAGS = -g -Wall -std=gnu++0x -lpthread -lrt
CPPFLAGS = -I. -Wno-unused-variable

OUT = futex

all: $(OUT)

futex: futex.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.o *.ii $(OUT)
