CC=gcc
CFLAGS=-Wall -ansi -Werror -pedantic -g

all: myshell.c infinite.c
	${CC} ${CFLAGS} -o ssalinas_shell myshell.c
	${CC} ${CFLAGS} -o infinite infinite.c
clean:
	rm -f ssalinas_shell infinite
