#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdint.h>
#include <list.h>
typedef list queue;
int8_t queue_enqueue(queue * p_queue, void * p_data);
int8_t queue_dequeue(queue * p_queue);
void * queue_peek(queue * p_queue);
#define queue_init list_init
#define queue_destroy list_destroy
#define queue_size list_size
#endif
