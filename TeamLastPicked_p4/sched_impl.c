#include "scheduler.h"
#include "sched_impl.h"

/* Fill in your scheduler implementation code below: */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue)
{
	/*...Code goes here...*/
}

static void destroy_thread_info(thread_info_t *info)
{
	/*...Code goes here...*/
}

/*...More functions go here...*/

static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
	if (queue == NULL)
	{
		queue = (sched_queue_t *) malloc(sizeof(sched_queue_t));
	}
	queue->q = (list_t *) malloc(sizeof(list_t));
	list_init(queue->q);

	queue->maxSize = queue_size;
	queue->numInQueue = 0;
}

static void destroy_sched_queue(sched_queue_t *queue)
{
	if (queue != NULL)
	{
		// Free the remaining elements in the queue
		list_foreach(queue->q, (void *) free);
		// Free the queue itself
		free(queue->q);
		queue = NULL;
	}

}

/*...More functions go here...*/

/* You need to statically initialize these structures: */
/* This structure will have function pointers only! */
/* The following are what we're supposed to implement:
	// Initialize a thread_info_t
	void (*init_thread_info)    (thread_info_t *info, sched_queue_t *queue);

	// Release the resources associated with a thread_info_t
	void (*destroy_thread_info) (thread_info_t *info);

	// Block until the thread can enter the scheduler queue.
	void (*enter_sched_queue)   (thread_info_t *info);

	// Remove the thread from the scheduler queue.
	void (*leave_sched_queue)   (thread_info_t *info);

	// While on the scheduler queue, block until thread is scheduled.
	void (*wait_for_cpu)        (thread_info_t *info);

	// Voluntarily relinquish the CPU when this thread's timeslice is over (cooperative multithreading).
	void (*release_cpu)         (thread_info_t *info);
 */

/* These are the functions that will be called when we are using a FIFO scheduling method */
sched_impl_t sched_fifo =
{
	{
		init_thread_info,
		destroy_thread_info
		/*, ...etc... */
	},
	{
		init_sched_queue,
		destroy_sched_queue
		/*, ...etc... */
	}
};
/* These are the functions that will be called when we are using a round robin scheduling method */
sched_rr =
{
	{
		init_thread_info,
		destroy_thread_info
		/*, ...etc... */
	},
	{
		init_sched_queue,
		destroy_sched_queue
		/*, ...etc... */
	}
};

/* A worker thread must be able to call the following operations.
 * See scheduler.c:worker_proc() for an example of usage. */
typedef struct worker_thread_ops {
	/* Initialize a thread_info_t */
	void (*init_thread_info)    (thread_info_t *info, sched_queue_t *queue);
	/* Release the resources associated with a thread_info_t */
	void (*destroy_thread_info) (thread_info_t *info);
	/* Block until the thread can enter the scheduler queue. */
	void (*enter_sched_queue)   (thread_info_t *info);
	/* Remove the thread from the scheduler queue. */
	void (*leave_sched_queue)   (thread_info_t *info);
	/* While on the scheduler queue, block until thread is scheduled. */
	void (*wait_for_cpu)        (thread_info_t *info);
	/* Voluntarily relinquish the CPU when this thread's timeslice is
	 * over (cooperative multithreading). */
	void (*release_cpu)         (thread_info_t *info);
} worker_thread_ops_t;
