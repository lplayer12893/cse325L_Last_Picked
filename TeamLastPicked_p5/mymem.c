#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>

/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
	// doubly-linked list
	struct memoryList *prev;
	struct memoryList *next;

	int size; // How many bytes in this block?
	char alloc; // 1 if this block is allocated, 0 if this block is free.
	void *ptr; // location of block in memory pool.
};

strategies myStrategy = NotSet; // Current strategy

size_t mySize;
void *myMemory = NULL;

static struct memoryList *head = NULL;
static struct memoryList *last = NULL;

/* initmem must be called prior to mymalloc and myfree.

 initmem may be called more than once in a given execution;
 when this occurs, all memory you previously malloc'ed  *must* be freed,
 including any existing bookkeeping data.

 strategy must be one of the following:
 - "best" (best-fit)
 - "worst" (worst-fit)
 - "first" (first-fit)
 - "next" (next-fit)
 sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
 */

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL)
		free(myMemory); /* in case this is not the first time initmem is called */

	if (head != NULL)
	{
		// Clear out our old management structure
		struct memoryList * cur = head;
		while (cur->next != NULL)
		{
			cur = cur->next;
			free(cur->prev);
		}
		free(cur);
		head = NULL;
		last = NULL;
	}

	// Initialize our new memory block
	myMemory = malloc(sz);
	if (myMemory == NULL)
	{
		perror("CRITICAL: Couldn't malloc myMemory");
		return;
	}

	// Initialize our new management structure
	head = (struct memoryList *) malloc(sizeof(struct memoryList));
	if (head == NULL)
	{
		perror("CRITICAL: Couldn't malloc our first memoryList");
		return;
	}
	last = head;

	head->next = NULL;
	head->prev = NULL;
	head->ptr = myMemory;
	head->size = sz;
	head->alloc = 0;
}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested)
{
	assert((int ) myStrategy > 0);
	if (head == NULL)
	{
		printf("ERROR: mymalloc called, but initMem was not called first.\n");
	}
	switch (myStrategy)
	{
		case NotSet:
			printf("ERROR: mymalloc called, but strategy not set. Returning NULL\n");
			return NULL;
		case First:
			struct memoryList * cur = head;
			while (cur != NULL)
			{
				if ((cur->alloc == 0) && cur->size >= requested)
				{
					// Found a block suitable.
					if (cur->size < requested)
					{
						// There will be memory left over, so alloc a new block
						struct memoryList * leftover = (struct memoryList *) malloc(sizeof(struct memoryList));
						if (leftover == NULL)
						{
							perror("Couldn't malloc structure for leftover");
							return NULL;
						}

						leftover->alloc = 0; // Leftover memory is free
						leftover->ptr = cur->ptr + requested; // ptr is the start of the leftover block
						leftover->size = cur->size - requested; // size is the leftover size

						// Insert it in the list
						leftover->next = cur->next;
						leftover->prev = cur;
						if (cur->next == NULL)
							last = leftover;
						else
							cur->next->prev = leftover;
						cur->next = leftover;
					}

					cur->alloc = 1;
					cur->size = requested;
				}
			}
			return NULL;
		case Best:
			// Best Fit, smallest suitable block
			return NULL;
		case Worst:
			// Worst fit, largest suitable block
			return NULL;
		case Next:
			// Next fit, start from the last allocated block. wraps around.
			return NULL;
	}
	return NULL;
}

/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void *block)
{
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->ptr == block) //if block is found
			{
				cur->alloc = '0'; //NOTE: sets alloc to free whether or not it was allocated already
				return;
			}

			cur = cur->next;
		}
		printf("Called myfree with %p, but it was not found!\n", block);
		return;
	}
	else
	{
		printf("Called myfree, but our internal list HEAD is NULL!\n");
		return;
	}
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when we refer to "memory" here, we mean the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	int count = 0;
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->alloc == '0') //if free
				count++;

			cur = cur->next;
		}
	}
	else
	{
		printf("Called mem_holes, but head is NULL!\n");
	}
	return count;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	int count = 0;
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->alloc == '1') //if free
				count += cur->size;

			cur = cur->next;
		}
	}
	else
	{
		printf("Called mem_allocated, but head is NULL!\n");
	}
	return count;
}

/* Number of non-allocated bytes */
int mem_free()
{
	int count = 0;
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->alloc == '0') //if free
				count += cur->size;

			cur = cur->next;
		}
	}
	else
	{
		printf("Called mem_free, but head is NULL!\n");
	}
	return count;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	int count = 0;
	int max = 0;
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->alloc == '0') //if free
			{
				while (cur != NULL && cur->alloc == '0')
				{
					count += cur->size;
					cur = cur->next;
					if (count > max)
						max = count;
				}
			}
			else
				cur = cur->next;
		}
	}
	else
	{
		printf("Called mem_largest_free, but head is NULL!\n");
	}
	return max;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	int count = 0;

	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->alloc == '0' && cur->size <= size) //if free and smaller or equal to block size
				count += cur->size;

			cur = cur->next;
		}
	}
	else
	{
		printf("Called mem_small_free, but head is NULL!\n");
	}
	return count;
}

char mem_is_alloc(void *ptr)
{
	struct memoryList *cur = NULL;
	if (head != NULL)
	{
		cur = head;
		while (cur != NULL)
		{
			if (cur->ptr == ptr) //if byte is found
				return cur->alloc;

			cur = cur->next;
		}
		printf("Called mem_is_alloc with %p, but it was not found!\n", ptr);
	}
	else
	{
		printf("Called mem_is_alloc, but head is NULL!\n");
	}
	return '1'; //If not found, mark it as allocated (because we can't allocate it)
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */

//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}

// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char *strategy)
{
	if (!strcmp(strategy, "best"))
	{
		return Best;
	}
	else if (!strcmp(strategy, "worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy, "first"))
	{
		return First;
	}
	else if (!strcmp(strategy, "next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}

/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n", mem_allocated(), mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n", mem_free(), mem_holes(), mem_largest_free());
	printf("Average hole size is %f.\n\n", ((float) mem_free()) / mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv)
{
	strategies strat;
	void *a, *b, *c, *d, *e;
	if (argc > 1)
		strat = strategyFromString(argv[1]);
	else
		strat = First;

	/* A simple example.
	 Each algorithm should produce a different layout. */

	initmem(strat, 500);

	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);

	print_memory();
	print_memory_status();

}
