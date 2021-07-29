#include <heap.h>
#include <stdlib.h>
#include <stdint.h>

struct heap_node {
    void * p_data;
    int64_t index; 
};

struct heap {
    int ordering;
    int max_children;
    int64_t size;
    void (* destory)(void * p_data);
    int8_t (* compare)(void * p_key1, void * p_key2);
    hnode ** pp_array;
};

static hnode * heap_member(heap * p_heap, int index)
{
    return (hnode *)((p_heap->pp_array) + index);
}

static hnode * heap_parent(heap * p_heap, hnode * p_node)
{
    int index = ((p_node->index) / 2);
    return heap_member(p_heap, index);
}

static hnode * heap_tail(heap * p_heap)
{
    return heap_member(p_heap, p_heap->size);
}

static hnode * heap_left(heap * p_heap, hnode * p_node)
{
    int index = ((p_node->index) * (p_heap->max_children));
    return heap_member(p_heap, index);
}

static hnode * heap_right(heap * p_heap, hnode * p_node)
{
    int index = (((p_node->index) * (p_heap->max_children)) + 1);
    return heap_member(p_heap, index);
}

heap * heap_init(int ordering, int max_children, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2))
{

}
void * heap_destroy(heap * p_heap);
hnode * heap_insert(heap * p_heap, void * p_data);
hnode * heap_peak(heap * p_heap);
hnode * heap_pull(heap * p_heap);
