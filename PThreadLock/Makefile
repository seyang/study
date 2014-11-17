CC = gcc
CFLAGS = -g -lcurl -ljansson -Wl,-rpath=/usr/local/lib
CXX = g++
CXXFLAGS = -g -Wall -std=gnu++0x -lpthread -lrt
CPPFLAGS = -I.

OUT = testpro1 testpro2

all: $(OUT)

testpro1: testpro1.o PThreadLock.o
	$(CXX) $(CXXFLAGS) -o $@ $^

testpro2: testpro2.o PThreadLock.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.o *.ii $(OUT)
