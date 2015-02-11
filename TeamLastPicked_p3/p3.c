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

#define FIFO 0
#define FILO 1

void printBuffer(int b[]);

int main(int argc, char ** argv)
{
	// 1. Get command line arguments
	int numProducers=0,numConsumers=0,bufferMode=-1;
	int i=0;
	int buffer[10];
	if (argc != 4)
	{
		// Wrong number of arguments
		fprintf(stderr,"Wrong number of arguments.\nUsage: %s [producers] [consumers] [buffer mode]\n\tproducers: number of producer threads\n\tconsumers: number of consumer threads\n\tbuffer mode: the mode for the buffer\n\t\t0: FIFO\n\t\t1: FILO\n\n",argv[0]);
		return 1;
	}
	sscanf(argv[1],"%d",&numProducers);
	sscanf(argv[2],"%d",&numConsumers);
	sscanf(argv[3],"%d",&bufferMode);
	
	if ((numProducers > 10) || (numProducers < 1))
	{
		// Wrong number of producers
		fprintf(stderr,"Wrong number of producers, should be between 1 and 10.\n");
		return 1;
	}
	if ((numConsumers > 10) || (numConsumers < 1))
	{
		// Wrong number of consumers
		fprintf(stderr,"Wrong number of consumers, should be between 1 and 10.\n");
		return 1;
	}
	if ((bufferMode != FIFO) && (bufferMode != FILO))
	{
		// Invalid buffer type
		fprintf(stderr,"Invalid buffer type given, must be 0 (for FIFO) or 1 (for FILO)\n");
		return 1;
	}
	
	printf("Running with %d producers, %d consumers, and a %s buffer.\n",numProducers,numConsumers,bufferMode == FIFO? "FIFO":"FILO");
	
	// 2. Initialize buffer entries with -1
	for (i=0;i<10;i++)
	{
		buffer[i] = -1;
	}
	
	printBuffer(buffer);
	
	// 3. Create producer thread(s)
	// 4. Create consumer thread(s)
	// 5. Sleep 300 seconds
	// 6. Exit
	return 0;
}

void printBuffer(int b[])
{
	int i=0;
	
	printf("Buffer contents: \n");
	for (i=0;i<10;i++)
	{
		printf("\tbuffer[%d]: %d\n",i,b[i]);
	}
}