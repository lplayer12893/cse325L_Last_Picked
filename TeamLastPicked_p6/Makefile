CC=gcc
CFLAGS=-Wall -Werror -g

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: rbtree.o disk.o test 
	@echo Build successful.

test: testDisk.c disk.o rbtree.o
	@echo Compiling Tests...
	${CC} ${CFLAGS} -o testDisk testDisk.c disk.o
	@echo Running Tests...
	@./testDisk
	@echo -e "\033[0;32m/=====================================\ \033[0m"
	@echo -e "\033[0;32m|========= All Tests Passed. =========|\033[0m"
	@echo -e "\033[0;32m\=====================================/\033[0m"
	@rm testDisk

clean:
	rm -f *.o disk
