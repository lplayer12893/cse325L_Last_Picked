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

sem_t mutex;

struct thread_info
{

};

struct sched_queue
{
		list_t * q;
		int maxSize;
		int numInQueue;

};

#endif /* __SCHED_IMPL__H__ */
