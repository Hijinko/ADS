#include <queue.h>
#include <list.h>
#include <stdint.h>
int8_t queue_enqueue(queue * p_queue, void * p_data);
int8_t queue_dequeue(queue * p_queue, void * p_data);
void * queue_peek(queue * p_queue);
