TARGET=X86_64

LD=gcc -Wall -Werror -O2 -pedantic -std=c99 -D_BSD_SOURCE -D$(TARGET) -DTRACE -DDEBUG
CC=$(LD) -c

AR= ar -rcs
RM=rm -rf
MAKE=make
CP=cp -rvf

all:clean test
	./test

test:math.o imu.o attidude.o data.o
	$(CC) test.c -o test.o
	$(LD) -lm -lpthread test.o math.o imu.o data.o attidude.o -o test

math.o:
	$(CC) -fno-strict-aliasing math.c -o math.o

imu.o:
	$(CC) -fno-strict-aliasing imu.c -o imu.o

attidude.o:
	$(CC) -fno-strict-aliasing attidude.c -o attidude.o

data.o:
	$(CC) -fno-strict-aliasing data.c -o data.o

clean:
	rm -rf *.o
	rm -rf test

.PHONY: clean all
