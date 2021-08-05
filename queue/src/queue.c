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
    if (NULL != list_ins_next(p_queue, list_tail(p_queue), p_data)){
        return 0;
    }
    return -1;
}

/*
 * @brief dequeues an element from the queue
 * @param p_queue the queue to dequeue from
 * @return 0 if the element dequeued successfully else -1
 */
int8_t queue_dequeue(queue * p_queue)
{
    // cant dequeue from a NULL or empty queue and cant dequeue NULL data
    if ((NULL == p_queue) || (0 == queue_size(p_queue))){
        return -1; 
    } 
    return list_rm_next(p_queue, NULL);
}

void * queue_peek(queue * p_queue);
