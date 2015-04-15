CC=gcc
CFLAGS=-Wall -Werror -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: disk.o test 
	@echo Build successful.

test: testDisk.c disk.o
	@echo Compiling Tests...
	${CC} ${CFLAGS} -o testDisk testDisk.c disk.o
	@echo Running Tests...
	@./testDisk
	@echo "\033[0;32m/=====================================\ \033[0m"
	@echo "\033[0;32m|========= All Tests Passed. =========|\033[0m"
	@echo "\033[0;32m\=====================================/\033[0m"
	@rm testDisk

clean:
	rm -f *.o disk
