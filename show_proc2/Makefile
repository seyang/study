CC = gcc
CPPFLAGS = -Wall -g -lncurses

OUT = my_nc

all: $(OUT)

test_ncurses: test_ncurses.o
	$(CC) $(CPPFLAGS) -o $@ $<

my_nc: my_nc.o
	$(CC) $(CPPFLAGS) -o $@ $<

clean:
	rm -rf *.o $(OUT)
