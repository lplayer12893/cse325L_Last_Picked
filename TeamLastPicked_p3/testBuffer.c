#include "buffer.h"
#include <stdio.h>

int main(void)
{
	int b[10];
	int i;
	
	// Test initialize
	initializeBuffer(b);
	for (i = 0; i < 10; i++)
	{
		if (b[i] != -1)
		{
			fprintf(stderr, "b[%d]: %d (should be -1)\n", i, b[i]);
			printBuffer(b);
			return 1;
		}
	}
	
	// Test push
	for (i = 0; i < 10; i++)
		push(b, i);
	for (i = 0; i < 10; i++)
	{
		if (b[i] != 9 - i)
		{
			fprintf(stderr, "b[%d]: %d (should be %d)\n", i, b[i], 9 - i);
			printBuffer(b);
			return 1;
		}
	}
	// Test pop
	for (i = 0; i < 10; i++)
	{
		if (pop(b) != 9 - i)
		{
			fprintf(stderr, "b[%d]: %d (should be %d)\n", i, b[i], 9 - i);
			printBuffer(b);
			return 1;
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (b[i] != -1)
		{
			fprintf(stderr, "b[%d]: %d (should be -1)\n", i, b[i]);
			printBuffer(b);
			return 1;
		}
	}
	
	// Test enqueue
	for (i = 0; i < 10; i++)
		enqueue(b, i);
	for (i = 0; i < 10; i++)
	{
		if (b[i] != i)
		{
			fprintf(stderr, "b[%d]: %d (should be %d)\n", i, b[i], i);
			printBuffer(b);
			return 1;
		}
	}
	
	// Test dequeue
	for (i = 0; i < 10; i++)
	{
		if (dequeue(b) != i)
		{
			fprintf(stderr, "b[%d]: %d (should be %d)\n", i, b[i], i);
			printBuffer(b);
			return 1;
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (b[i] != -1)
		{
			fprintf(stderr, "b[%d]: %d (should be -1)\n", i, b[i]);
			printBuffer(b);
			return 1;
		}
	}
	
	if (countItems(b) != 0)
	{
		fprintf(stderr, "countItems failed on empty list! Gave %d, should be 0.\n", countItems(b));
		printBuffer(b);
		return 1;
	}
	for (i = 0; i < 10; i++)
	{
		enqueue(b, i);
		if (countItems(b) != i + 1)
		{
			fprintf(stderr, "countItems failed! Gave %d, Should be %d\n", countItems(b), i);
			printBuffer(b);
			return 1;
		}
	}
	
	printf("All buffer tests passed.\n");
	return 0;
}
