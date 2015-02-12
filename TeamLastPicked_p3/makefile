CC=gcc
CFLAGS=-Wall -Werror -std=c99 -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: p3.c buffer.o test
	${CC} ${CFLAGS} -pthread -o p3 p3.c buffer.o
	@echo Build successful. Please run ./p3

test: testBuffer.c buffer.o
	${CC} ${CFLAGS} -o testBuffer testBuffer.c buffer.o
	@echo Running Tests...
	./testBuffer
	@echo All Tests Passed.
	@rm testBuffer

clean:
	rm -f *.o p3 testBuffer
