#include "buffer.h"
#include <stdio.h>

int main(void)
{
	int b[10];
	int i;
	
	// Test initialize
	initializeBuffer(b);
	for (i=0;i<10;i++)
	{
		if (b[i] != -1)
		{
			fprintf(stderr,"b[%d]: %d (should be -1)\n",i,b[i]);
			return 1;
		}
	}	
	
	// Test enqueue
	
	// Test dequeue
	
	// Test push
	
	// Test pop
	
	// Test enqueue overflow
	
	// Test dequeue overflow
	
	printf("All buffer tests passed.\n");
	return 0;
}