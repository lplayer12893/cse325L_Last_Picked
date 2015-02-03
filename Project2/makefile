CC=gcc
CFLAGS=-Wall -Werror -std=c99 -g

all: shell.c infinite.c
	${CC} ${CFLAGS} -o shell shell.c
	${CC} ${CFLAGS} -o infinite infinite.c
clean:
	rm -f shell infinite
