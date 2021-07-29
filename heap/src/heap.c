#include <heap.h>
#include <stdlib.h>
#include <stdint.h>

struct heap_node {
    void * p_data;
};

struct heap {
    int ordering;
    int max_children;
    void (* destory)(void * p_data);
    int8_t (* compare)(void * p_key1, void * p_key2);
    hnode ** pp_members;
};

heap * heap_init(int ordering, int max_children, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2));
void * heap_destroy(heap * p_heap);
hnode * heap_insert(heap * p_heap, void * p_data);
hnode * heap_peak(heap * p_heap);
hnode * heap_pull(heap * p_heap);
