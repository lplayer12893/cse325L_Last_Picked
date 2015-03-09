/*
 * filename: sched_impl.c
 * description: This is our implementation of the FIFO and RR schedulers
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #4
 * assigned: February 25th, 2015
 * due: March 11th, 2015
 */

#include "scheduler.h"
#include "sched_impl.h"

// ****************** THREAD OPERATIONS ****************************************

/**
 * This is a function that will initialize a thread_info_t.
 */
static void init_thread_info(thread_info_t *info, sched_queue_t *queue)
{
	// Depends on what we put in the thread_info_t structure.
	info->queue = queue;
	list_elem_t *n = (list_elem_t*) malloc(sizeof(list_elem_t));
	if (n == NULL)
	{
		perror("Couldn't malloc a thread info!");
		return;
	}
	list_elem_init(n,info);
	info->list_element = n;
}

/**
 * This is a function that will destroy a thread info.
 * Releases the resources associated with a thread_info_t
 */
static void destroy_thread_info(thread_info_t *info)
{
	// Depends on what we put in the thread_info_t structure.
	info->queue = NULL;
	free(info->list_element);
	info->list_element = NULL;
}

/**
 * This is a function that will put a thread into the scheduler queue.
 * This should block until the thread can enter the scheduler queue (if the queue is full)
 */
static void enter_sched_queue(thread_info_t *info)
{
	// Check if the queue has room
	// This will block until the queue has room, and wake up when it does.
	sem_wait(&(info->queue->queue_sem));

	// Add to the queue.
	list_insert_tail(info->queue->q,info->list_element);
}

/**
 * This will be called when the worker has completed. It should remove the thread from the scheduler queue.
 */
static void leave_sched_queue(thread_info_t *info)
{
	// Remove from the queue
	list_remove_elem(info->queue->q,info->list_element);

	// Update the number of elements in the queue
	// Note, this will wake up something that is waiting to be in the queue.
	sem_post(&(info->queue->queue_sem));
}

/**
 * This is a function that will block until the scheduler allows it to run.
 * While on the scheduler queue, block until thread is scheduled.
 */
static void fifo_wait_for_cpu(thread_info_t * info)
{
	// TODO: Write this function out.
	// Check if the CPU is available.
	// If it is available, return immediately.
	// If it isn't, then block until it is.
}

/**
 * This will be called when the thread is ready to release the CPU
 * Voluntarily relinquish the CPU when this thread's timeslice is over (cooperative multithreading).
 */
static void fifo_release_cpu(thread_info_t * info)
{
	// TODO: Write this function out.
	// Since we are FIFO, I think we just return immediately, and let the process continue.
}

/**
 * This will be called when the thread is ready to pause and wait its next turn.
 * Voluntarily relinquish the CPU when this thread's timeslice is over (cooperative multithreading).
 */
static void rr_release_cpu(thread_info_t * info)
{
	// TODO: Write this function out.
	// Put this in the queue, at the end.
}

/**
 * This should block until its our turn in the queue.
 * While on the scheduler queue, block until thread is scheduled.
 */
static void rr_wait_for_cpu(thread_info_t * info)
{
	// TODO: Write this function out.
	// Block until we are up in the queue.
}

// ******************** SCHEDULER OPERATIONS ***********************************

/**
 * This is a function that will initialize a scheduler queue.
 * Initialize a sched_queue_t
 */
static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
	// Initialize the queue
	if (queue == NULL)
	{
		queue = (sched_queue_t *) malloc(sizeof(sched_queue_t));
	}
	queue->q = (list_t *) malloc(sizeof(list_t));
	list_init(queue->q);

	queue->currentPosition = 0;

	// Initialize the semaphores
	sem_init(&(queue->cpu_sem),0,1);
	sem_init(&(queue->queue_sem),0,queue_size);

}

/**
 * This is a function that will destroy a scheduler queue.
 * Release the resources associated with a sched_queue_t
 */
static void destroy_sched_queue(sched_queue_t *queue)
{
	// Destroy the queue
	if (queue != NULL)
	{
		// Free the remaining elements in the queue
		list_foreach(queue->q, (void *) free);
		// Free the queue itself
		free(queue->q);
		queue = NULL;
	}
	// Destroy the semaphores
	sem_destroy(&(queue->cpu_sem));
	sem_destroy(&(queue->queue_sem));

}

/**
 * Allows (signals) a worker thread to execute.
 */
static void wake_up_worker(thread_info_t *info)
{
	// TODO: Write this function out.
}

/**
 * Blocks until the current worker thread relinquishes the CPU.
 */
static void wait_for_worker(sched_queue_t *queue)
{
	// TODO: Write this function out.
	// Wait until the CPU Semaphore is done
}

/**
 * Select the next worker thread to execute. Returns NULL if the scheduler queue is empty.
 */
thread_info_t * next_worker(sched_queue_t *queue)
{
	// TODO: Write this function out.
	return NULL;
}

/**
 * Block until at least one worker thread is in the scheduler queue.
 */
static void wait_for_queue(sched_queue_t *queue)
{
	// TODO: Write this function out.
}

// *************************** END OF FUNCTIONS ********************************

/* These are the functions that will be called when we are using a FIFO scheduling method */
sched_impl_t sched_fifo =
{
	{
		init_thread_info,
		destroy_thread_info,
		enter_sched_queue,
		leave_sched_queue,
		fifo_wait_for_cpu,
		fifo_release_cpu
	},
	{
		init_sched_queue,
		destroy_sched_queue,
		wake_up_worker,
		wait_for_worker,
		next_worker,
		wait_for_queue
	}
};

/* These are the functions that will be called when we are using a round robin scheduling method */
sched_impl_t sched_rr =
{
	{
		init_thread_info,
		destroy_thread_info,
		enter_sched_queue,
		leave_sched_queue,
		rr_wait_for_cpu,
		rr_release_cpu
	},
	{
		init_sched_queue,
		destroy_sched_queue,
		wake_up_worker,
		wait_for_worker,
		next_worker,
		wait_for_queue
	}
};
