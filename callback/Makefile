CC = gcc
CFLAGS = -g -lcurl -ljansson -Wl,-rpath=/usr/local/lib
CXX = g++
CXXFLAGS = -g -Wall -std=gnu++0x -lpthread -lrt
CPPFLAGS = -I. -Wno-unused-variable

EXEC_NAME = callback

OUT = $(EXEC_NAME)

all: $(OUT)

EXEC_NAME: $(EXEC_NAME).o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.o *.ii $(OUT)
