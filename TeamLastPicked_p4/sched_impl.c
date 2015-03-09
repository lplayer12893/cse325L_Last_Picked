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
