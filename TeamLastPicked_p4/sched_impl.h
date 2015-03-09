/*
 * filename: sched_impl.h
 * description: This is our implementation of the FIFO and RR schedulers. This is the header file.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #4
 * assigned: February 25th, 2015
 * due: March 11th, 2015
 */

#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__

#include <semaphore.h>
#include "list.h"
#include <stdlib.h>

struct thread_info
{

};

struct sched_queue
{
		// The actual queue.
		list_t * q;

		// Semaphores for our execution. One for the CPU (running state), and one for the queue
		sem_t queue;
		sem_t cpu;

		// Used for the round robin scheduler method. Marks the current process that is running.
		int currentPosition;

};

struct sched_queue * masterQueue;

#endif /* __SCHED_IMPL__H__ */
