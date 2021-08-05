#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdint.h>
typedef struct queue queue
queue * queue_init(void (* destory)(void * p_data));
void * queue_destroy(queue * p_queue);
int8_t queue_enqueue(queue * p_queue, void * p_data);
int8_t queue_dequeue
#endif
