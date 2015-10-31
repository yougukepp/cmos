CC=gcc
LD=gcc
INC=-I../../cmos/
CFLAGS=-c

all:test

test:tree.c main.c
	$(CC) $(CFLAGS) $(INC) tree.c -o tree.o
	$(CC) $(CFLAGS) $(INC) main.c -o main.o
	$(LD) $(LDFLAGS) main.o tree.o -o test

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf test
