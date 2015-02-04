CC=gcc
CFLAGS=-Wall -Werror -std=c99 -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: shell.c infinite.c getStrings.o
	${CC} ${CFLAGS} -o shell shell.c getStrings.o
	${CC} ${CFLAGS} -o infinite infinite.c
clean:
	rm -f *.o shell infinite
