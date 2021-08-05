#include <queue.h>
#include <list.h>
#include <stdint.h>
#include <stdio.h>

/*
 * @brief adds an element to the queue
 * @param p_queue the queue to add the element to
 * @param p_data the data to add to the queue
 * @return 0 if element added successfully else -1
 */
int8_t queue_enqueue(queue * p_queue, void * p_data)
{
    // cant add data to a NULL queue or from NULL data
    if ((NULL == p_queue) || (NULL == p_data)){
        return -1;
    }
    return list_ins_next(p_queue, list_tail(p_queue), p_data);
}

int8_t queue_dequeue(queue * p_queue, void * p_data);
void * queue_peek(queue * p_queue);
