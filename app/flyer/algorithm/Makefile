TARGET=x86_64

LD=gcc -Wall -Werror -O2 -pedantic -std=c99
CC=$(LD) -c

AR= ar -rcs
RM=rm -rf
MAKE=make
CP=cp -rvf

all:test

test:math.o
	$(CC) test.c -o test.o
	$(LD) test.o math.o -o test

math.o:
	$(CC) -fno-strict-aliasing math.c -o math.o

clean:
	rm -rf *.o
	rm -rf test

.PHONY: clean all
