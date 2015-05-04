#include "myfilesys.h"
#include <stdio.h>
#include <stdlib.h>

void error(char * str);

int main(int argc, char ** argv)
{
	// This is where we add our own tests. Make will automatically run these. 
	// To error out, just return any non-zero integer.
	if (make_fs("test.fs") == -1)
		error("Cannot make file system!");
	if (mount_fs("test.fs") == -1)
		error("Cannot mount file system!");
	if (unmount_fs("test.fs") == -1)
		error("Cannot unmount file system!");

	return 0;
}

/**
 * Prints an error and stops the makefile.
 * @param str string describing the error.
 */
void error(char * str)
{
	fprintf(stderr,"\033[22;41;30mERROR:\033[0m %s\n",str);
	exit(1);
}
