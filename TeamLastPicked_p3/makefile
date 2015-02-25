CC=gcc
CFLAGS=-Wall -Werror -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: p3.c buffer.o test
	${CC} ${CFLAGS} -pthread -o p3 p3.c buffer.o
	@echo Build successful. Please run ./p3

test: testBuffer.c buffer.o
	${CC} ${CFLAGS} -o testBuffer testBuffer.c buffer.o
	@echo Running Tests...
	./testBuffer
	@echo -e "\033[0;32m/=====================================\ \033[0m"
	@echo -e "\033[0;32m|========= All Tests Passed. =========|\033[0m"
	@echo -e "\033[0;32m\=====================================/\033[0m"
	@rm testBuffer

clean:
	rm -f *.o p3 testBuffer
