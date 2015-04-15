CC=gcc
CFLAGS=-Wall -Werror -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: disk.o
	${CC} ${CFLAGS} disk.o -o disk

clean:
	rm -f *.o disk
