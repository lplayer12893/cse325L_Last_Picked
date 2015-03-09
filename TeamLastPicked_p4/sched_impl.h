#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__

#include <semaphore.h>
#include "list.h"
#include <stdlib.h>

sem_t mutex;

struct thread_info {

};

struct sched_queue {
	list_t * q;
	int maxSize;
	int numInQueue;

};

#endif /* __SCHED_IMPL__H__ */
