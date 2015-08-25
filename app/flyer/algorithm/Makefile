TARGET=X86_64

OBJS = pc.o algo_math.o port.o attidude.o gyro.o accel.o mag.o algo.o test.o

LD=gcc -Wall -Werror -O2 -pedantic -std=c99 -D_BSD_SOURCE -D$(TARGET) -DALGO_TRACE -DALGO_DEBUG
CC=$(LD) -c -fno-strict-aliasing
RM=rm -rf

all:clean test
	./test

test: $(OBJS)
	$(LD) -lm -lpthread $(OBJS) -o test

$(OBJS): %.o: %.c
	$(CC) -c $< -o $@

clean:
	${RM} *.o
	${RM} test

.PHONY: clean all

