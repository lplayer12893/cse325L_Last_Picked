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
			printBuffer(b);
			return 1;
		}
	}
	
	// Test push
	for (i=0;i<10;i++)
		push(b,i);
	for (i=0;i<10;i++)
	{
		if (b[i] != 9-i)
		{
			fprintf(stderr,"b[%d]: %d (should be %d)\n",i,b[i],9-i);
			printBuffer(b);
			return 1;
		}
	}
	// Test pop
	for (i=0;i<10;i++)
	{
		if (pop(b) != 9-i)
		{
			fprintf(stderr,"b[%d]: %d (should be %d)\n",i,b[i],9-i);
			printBuffer(b);
			return 1;
		}
	}
	for (i=0;i<10;i++)
	{
		if (b[i] != -1)
		{
			fprintf(stderr,"b[%d]: %d (should be -1)\n",i,b[i]);
			printBuffer(b);
			return 1;
		}
	}
	
	// Test enqueue
	
	// Test dequeue
	
	
	
	// Test enqueue overflow
	
	// Test dequeue overflow
	
	printf("All buffer tests passed.\n");
	return 0;
}