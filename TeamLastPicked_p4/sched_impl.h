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
#include <stdio.h>

struct thread_info
{
	// We need to keep track of where the queue is
	sched_queue_t *queue;
	// We also need to keep track of the element container for the linked list
	list_elem_t *list_element;
};

struct sched_queue
{
		// The actual queue.
		list_t * q;

		// Semaphores for our execution. One for the CPU (running state), and one for the queue
		sem_t queue_sem;
		sem_t cpu_sem;

		// Used for max size, RR method.
		int queueSize;

		// Used for the round robin scheduler method. Marks the current process that is running.
		int currentPosition;

};

#endif /* __SCHED_IMPL__H__ */
