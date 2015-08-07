TARGET=x86_64

LD=gcc -Wall -Werror -O2 -pedantic -std=c99 -D_BSD_SOURCE
CC=$(LD) -c

AR= ar -rcs
RM=rm -rf
MAKE=make
CP=cp -rvf

all:clean test
	#./test

test:math.o imu.o
	$(CC) test.c -o test.o
	$(LD) -lm test.o math.o imu.o -o test

math.o:
	$(CC) -fno-strict-aliasing math.c -o math.o

imu.o:
	$(CC) -fno-strict-aliasing imu.c -o imu.o

clean:
	rm -rf *.o
	rm -rf test

.PHONY: clean all
