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

int main(int argc, char ** argv)
{
	// 1. Get command line arguments
	int numProducers=0,numConsumers=0,bufferMode=-1;
	int buffer[10];
	if (argc != 4)
	{
		// Wrong number of arguments
		fprintf(stderr,"Wrong number of arguments.\nUsage: %s [producers] [consumers] [buffer mode]\n\tproducers: number of producer threads\n\tconsumers: number of consumer threads\n\tbuffer mode: the mode for the buffer\n\t\t0: FIFO\n\t\t1: FILO\n\n",argv[0]);
		return 1;
	}
	
	// 2. Initialize buffer entries with -1
	// 3. Create producer thread(s)
	// 4. Create consumer thread(s)
	// 5. Sleep 300 seconds
	// 6. Exit
	return 0;
}
