#include "scheduler.h"
#include "sched_impl.c"
#include "list.c"

static void destroy_sched_queue(sched_queue_t *queue)
{
    if(queue != NULL)
    {
        list_foreach(queue->q,free);
    }
}

void disp((void *) elem)
{
    printf("Element %i of list\n",(int)elem);
}

static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
    int i = 0;
    if(queue == NULL)
    {
        queue = (sched_queue)malloc(sizeof(queue));
    }
    queue->q = malloc(sizeof(list_t));
	list_init(queue->q);

	for(; i<queue_size; i++)
    {
        list_insert_head(queue->q, list_elem_init((list_elem_t)malloc(sizeof(list_elem_t)),&i));
    }

    list_foreach(queue->q,disp);
}

int main(int argc, char ** argv)
{
    sched_queue_t q;
    init_sched_queue(&q,1);

}
