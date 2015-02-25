/*
 * filename: buffer.c
 * description: This is an implementation of a buffer using integer arrays.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #3
 * assigned: February 11th, 2015
 * due: February 25th, 2015
 */

#include "buffer.h"

/**
 * This function prints the state of the buffer.
 * 
 * @param b buffer to print
 */
void printBuffer(int b[])
{
	int i = 0;
	
	printf("[");
	for (i = 0; i < 10; i++)
	{
		printf("%d",b[i]);
		if (i != 9)
			printf(", ");
	}
	printf("]\n");
}

/**
 * This initializes the buffer to safe values (-1)
 * 
 * @param b buffer to initialize
 */
void initializeBuffer(int b[])
{
	int i;
	for (i = 0; i < 10; i++)
		b[i] = -1;
}

/**
 * This function pushes an item into the FILO buffer.
 * 
 * @param b buffer to use
 * @param item item to push
 */
void push(int b[], int item)
{
	int i;
	
	// First, check for overflow
	if (b[9] != -1)
		printf("WARNING: Buffer overflow!\n");
	
	// Next, shift all items to the right.
	for (i = 9; i > 0; i--)
	{
		b[i] = b[i - 1];
	}
	
	// Finally, add our item
	b[0] = item;
}

/**
 * This function pops an item off of the FILO buffer
 * 
 * @param b buffer to use
 * @return the item popped off
 */
int pop(int b[])
{
	int i;
	
	// First, save our return value
	int ret = b[0];
	
	// Next, shift all our items left.
	for (i = 0; i < 9; i++)
	{
		b[i] = b[i + 1];
	}
	
	// Next, make the last item -1
	b[9] = -1;
	
	// Lastly, return our value
	return ret;
}

/**
 * This function enqueues an item into the FIFO buffer
 * 
 * @param b buffer to use
 * @param item item to enqueue
 */
void enqueue(int b[], int item)
{
	int i;
	
	// First, walk the array looking for the first free spot
	for (i = 0; i < 9; i++)
	{
		if (b[i] == -1)
			break;
	}
	// Check for overflow
	if ((i == 9) && (b[i] != -1))
		printf("WARNING: Buffer overflow!");
	
	// Add our item
	b[i] = item;
}

/**
 * This function dequeues an item from the FIFO buffer
 * @param b buffer to use
 * @return item dequeued
 */
int dequeue(int b[])
{
	// Since dequeue pulls from the same side as pop, we can just call it.
	return pop(b);
}

/**
 * This returns how many items are in the buffer.
 * 
 * @param b buffer to use
 * @return number of items in buffer.
 */
int countItems(int b[])
{
	int i;
	for (i = 0; i < 10; i++)
	{
		if (b[i] == -1)
			break;
	}
	return i;
}
