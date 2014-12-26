CC		:= g++
#CFLAGS	:= -Wall -g
CPPFLAGS := -I. -Wall -g 
LOADLIBES := -L.

OUT = put get

.PHONY: all clean
all: $(OUT)

put: put.o ShmQueue.o

get: get.o ShmQueue.o

ShmQueue.o: ShmQueue.cpp ShmQueue.h

clean:
	-rm -f *.o $(OUT)
