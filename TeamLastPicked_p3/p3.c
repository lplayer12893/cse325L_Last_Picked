/*
 * filename: p3.c
 * description: This is an implementation of producer/consumer model of shared
 *  buffering, with multi-threading.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #3
 * assigned: February 11th, 2015
 * due: February 25th, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "buffer.h"

#define FIFO 0
#define FILO 1
#define PRODUCER 0
#define CONSUMER 1

sem_t mutex;

typedef struct _thread_info
{
	int num;
	int type;
	int delay;
	int bufferType;
	int * buffer;
} thread_info;

void * thread_run(void * thread_info);

int main(int argc, char ** argv)
{
	int numProducers = 0, numConsumers = 0, bufferMode = -1;
	int i;
	int buffer[10];
	pthread_t threads[20];
	sem_init(&mutex, 0, 1);

	// 1. Get command line arguments
	if (argc != 4)
	{
		// Wrong number of arguments
		fprintf(stderr, "Wrong number of arguments.\nUsage: %s [producers] [consumers] [buffer mode]\n\tproducers: number of producer threads\n\tconsumers: number of consumer threads\n\tbuffer mode: the mode for the buffer\n\t\t0: FIFO\n\t\t1: FILO\n\n", argv[0]);
		return 1;
	}

	// Parse arguments given
	sscanf(argv[1], "%d", &numProducers);
	sscanf(argv[2], "%d", &numConsumers);
	sscanf(argv[3], "%d", &bufferMode);

	// Check for invalid parameters
	if ((numProducers > 10) || (numProducers < 1))
	{
		// Wrong number of producers
		fprintf(stderr, "Wrong number of producers, should be between 1 and 10.\n");
		return 1;
	}
	if ((numConsumers > 10) || (numConsumers < 1))
	{
		// Wrong number of consumers
		fprintf(stderr, "Wrong number of consumers, should be between 1 and 10.\n");
		return 1;
	}
	if ((bufferMode != FIFO) && (bufferMode != FILO))
	{
		// Invalid buffer type
		fprintf(stderr, "Invalid buffer type given, must be 0 (for FIFO) or 1 (for FILO)\n");
		return 1;
	}

	// Print out the information, for a sanity check
	printf("Running with %d producers, %d consumers, and a %s buffer.\n", numProducers, numConsumers, bufferMode == FIFO ? "FIFO" : "FILO");

	// 2. Initialize buffer entries with -1
	initializeBuffer(buffer);
	// printBuffer(buffer);

	// Initialize RNG
	srand(time(NULL));
	// 3. Create producer thread(s)
	for (i = 0; i < numProducers; i++)
	{
		thread_info * t = malloc(sizeof(thread_info));
		if (t == NULL)
		{
			perror("Couldn't malloc");
			return 1;
		}
		t->num = i;
		t->type = PRODUCER;
		t->delay = rand() % 1001;
		t->bufferType = bufferMode;
		t->buffer = buffer;
		pthread_create(&threads[i], NULL, thread_run, (void *) t);
	}

	// 4. Create consumer thread(s)
	for (i = 0; i < numConsumers; i++)
	{
		thread_info * t = malloc(sizeof(thread_info));
		if (t == NULL)
		{
			perror("Couldn't malloc");
			return 1;
		}
		t->num = i;
		t->type = CONSUMER;
		t->delay = rand() % 1001;
		t->bufferType = bufferMode;
		t->buffer = buffer;
		pthread_create(&threads[numProducers + i], NULL, thread_run, (void *) t);
	}
	// 5. Sleep 300 seconds
	sleep(300);

	// 6. Exit
	sem_destroy(&mutex);
	return 0;
}

void * thread_run(void * arg)
{
	// Cast our thread info from the argument we are passed.
	thread_info * info = (thread_info *) (arg);
	int print;
	int randInt;
	
	// Used for nanosleep
	struct timespec t;

	t.tv_sec = 0;
	t.tv_nsec = info->delay * 1000000; // Conversion from nanoseconds to milliseconds.

	// Print all thread status.
	if (info->type == PRODUCER)
		printf("Producer ");
	else if (info->type == CONSUMER)
		printf("Consumer ");
	else
		printf("UNKNOWN ");
	printf("thread #%d checking in, with delay of %dms.\n", info->num, info->delay);

	while(1)
	{
		// First, sleep our random amount of time
		nanosleep(&t,NULL);
		
		// Next, wait for the semaphore.
		sem_wait(&mutex);
		
		// Set our print variable. We don't want to print the buffer if we didn't do anything to it.
		print = 0;

		if (info->type == PRODUCER)
		{
			// Producer thread
			// Make a random value to add
			randInt = rand();
			
			if(countItems(info->buffer) != 10)
			{
				// Buffer is not full, so add to it
				if(info->bufferType == FIFO)
				{
					// FIFO = enqueue
					enqueue(info->buffer,randInt);
				}
				else if(info->bufferType == FILO)
				{
					// FILO = push
					push(info->buffer,randInt);
				}
				// Print what we did.
				printf("Item %d added by Producer %d:",randInt, info->num);
				// Set print flag
				print = 1;
			}
		}
		else if (info->type == CONSUMER)
		{
			// Consumer thread
			if(countItems(info->buffer) != 0)
			{
				// Buffer is not empty
				if(info->bufferType == FIFO)
				{
					// FIFO = dequeue
					randInt = dequeue(info->buffer);
				}
				else if(info->bufferType == FILO)
				{
					// FILO = pop
					randInt = pop(info->buffer);
				}
				// Printi what we did
				printf("Item %d taken by Consumer %d:",randInt, info->num);
				// Set print flag
				print = 1;
			}
			
		}
		if (print == 1)
		{
			// We did something to the buffer, so print its state
			printf(" buffer = ");
			printBuffer(info->buffer);
		}
		// Release the lock on the semaphore
		sem_post(&mutex);
	}

	free(arg);
	return NULL;
}
