CC=gcc
CFLAGS=-Wall -Werror -std=c99 -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: p3.c
	${CC} ${CFLAGS} -o p3 p3.c

clean:
	rm -f *.o p3
