CC=gcc
CFLAGS=-Wall -Werror -g -O0

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

all: disk.o myfilesys.o test 
	@echo Build successful.

test: testDisk.c disk.o myfilesys.o
	@echo Compiling Tests...
	${CC} ${CFLAGS} -o testDisk testDisk.c disk.o myfilesys.o
	@echo Running Tests...
	@./testDisk
	@echo -e "\033[0;32m/=====================================\ \033[0m"
	@echo -e "\033[0;32m|========= All Tests Passed. =========|\033[0m"
	@echo -e "\033[0;32m\=====================================/\033[0m"

clean:
	rm -f *.o test.fs testDisk
