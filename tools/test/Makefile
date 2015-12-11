CC=gcc
LD=gcc
INC=-I../../cmos/
CFLAGS=-c -Wall -D_X64_PC_

all:clean test
	@echo
	@echo
	#@./test t
	#@echo
	#@echo
	#@./test l
	#@echo
	#@echo
	@./test rb

test:tree.c main.c path.c vfs.c
	$(CC) $(CFLAGS) $(INC) vfs.c -o vfs.o
	$(CC) $(CFLAGS) $(INC) path.c -o path.o
	$(CC) $(CFLAGS) $(INC) tree.c -o tree.o
	$(CC) $(CFLAGS) $(INC) list.c -o list.o
	$(CC) $(CFLAGS) $(INC) ring_queue.c -o ring_queue.o
	$(CC) $(CFLAGS) $(INC) main.c -o main.o
	$(LD) $(LDFLAGS) main.o tree.o list.o vfs.o path.o ring_queue.o -o test

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf test
	rm -rf *.stackdump
	rm -rf cscope.*
	rm -rf tags

