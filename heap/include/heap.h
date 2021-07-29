#ifndef _HEAP_H
#define _HEAP_H
#include <stdint.h>
typedef struct heap_node hnode;
typedef struct heap heap;
heap * heap_init(int ordering, int max_children, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2));
void * heap_destroy(heap * p_heap);
hnode * heap_insert(heap * p_heap, void * p_data);
hnode * heap_peak(heap * p_heap);
hnode * heap_pull(heap * p_heap);
#define MAX 1
#define MIN 0
#endif
