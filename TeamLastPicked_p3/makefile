CC=gcc
CFLAGS=-Wall -Werror -std=c99 -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: p3.c buffer.o
	${CC} ${CFLAGS} -o p3 p3.c buffer.o

test: testBuffer.c buffer.o
	${CC} ${CFLAGS} -o testBuffer testBuffer.c buffer.o
clean:
	rm -f *.o p3 testBuffer
