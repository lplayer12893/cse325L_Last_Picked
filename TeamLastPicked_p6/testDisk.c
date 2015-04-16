#include "disk.h"
#include <stdio.h>
#include <stdlib.h>

void error(char * str);

int main(int argc, char ** argv)
{
	// This is where we add our own tests. Make will automatically run these. 
	// To error out, just return any non-zero integer.

	// First, let's test our red black tree
	
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