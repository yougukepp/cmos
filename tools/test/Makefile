CC=gcc
LD=gcc
INC=-I../../cmos/
CFLAGS=-c -Wall -D_X64_PC_

all:clean test
	@echo
	@echo
	#./test

test:tree.c main.c vfs.c
	$(CC) $(CFLAGS) $(INC) vfs.c -o vfs.o
	$(CC) $(CFLAGS) $(INC) tree.c -o tree.o
	$(CC) $(CFLAGS) $(INC) main.c -o main.o
	$(LD) $(LDFLAGS) main.o tree.o vfs.o -o test

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf test
	rm -rf *.stackdump
	rm -rf cscope.*
	rm -rf tags

