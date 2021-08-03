#include <heap.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * @brief the node that is stored in a heap
 * @param p_data pointer to the data in the node
 * @param index index of the node
 */
struct heap_node {
    void * p_data;
    int64_t index; 
};

/*
 * @brief a heap structure 
 * @param ordering either min or max ordering
 * @param max_children maximum amount of nodes the heap can have
 * @param size the number of children in the heap
 * @param destroy the tear down function for the data in a heap node
 * @param compare the user defined compare function for the heap
 * @param pp_array an array of heap nodes in the heap
 */
struct heap {
    int ordering;
    int max_children;
    int64_t size;
    void (* destory)(void * p_data);
    int8_t (* compare)(void * p_key1, void * p_key2);
    hnode ** pp_array;
};

/*
 * @brief get the node at the specified index in a heap
 * @param p_heap the heap to get the node from
 * @param index the index of the node to get
 * @return the heap node in the heap
 */
static hnode * heap_member(heap * p_heap, int index)
{
    return (hnode *)((p_heap->pp_array) + index);
}

/*
 * @brief get the parent node of a specified heap node
 * @param p_heap the heap to search in
 * @param p_node the node to get the parent from
 * @return the parent node of the passed in heap node
 */
static hnode * heap_parent(heap * p_heap, hnode * p_node)
{
    // check if p_node is the root node
    if (p_node->index == 0){
        return NULL;
    }

    int index = ((p_node->index) / 2);
    return heap_member(p_heap, index);
}

/*
 * @brief get the last element in a heap
 * @param p_heap the heap to get the tail from
 * @return the tail node of the heap
 */
static hnode * heap_tail(heap * p_heap)
{
    // an empty heap does not have a tail
    if (0 == p_heap->size){
        return NULL;
    }

    return heap_member(p_heap, p_heap->size);
}

/*
 * @brief get the left child of a heap in a base 0 index
 * @param p_heap the heap to search in
 * @param p_node the node to get the left child from
 * @return the left child node of the passed in node
 */
static hnode * heap_left(heap * p_heap, hnode * p_node)
{
    int index = ((p_node->index) * (p_heap->max_children));
    return heap_member(p_heap, index);
}

/*
 * @brief get the right child of a heap in a base 0 index
 * @param p_heap the heap to search in
 * @param p_node the node to get the right child from
 * @return the right child node of the passed in node
 */
static hnode * heap_right(heap * p_heap, hnode * p_node)
{
    int index = (((p_node->index) * (p_heap->max_children)) + 1);
    return heap_member(p_heap, index);
}

/*
 * @brief allocate and initialize a heap
 * @param ordering integer identifying if the heap should be min or max heap 0 or 1 respectively   
 * @param max_children the initial max number of elements in the heap
 * @param destroy user defined function to tear down the nodes data
 * @param compare user defined function to compare the data in the nodes should return -1 0 or 1
 * @return a newly initialized heap or NULL on error
 */
heap * heap_init(int ordering, int max_children, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2))
{

}

/*
 * @brief tear down a heap
 * @param p_heap the heap to tear down
 */
void * heap_destroy(heap * p_heap);

/*
 * @brief adds a new node to a heap
 * @param p_heap the heap to insert the node into
 * @param p_data the data of the new node
 * @return pointer to the new heap node inside the heap or NULL on error
 */
hnode * heap_insert(heap * p_heap, void * p_data);

/*
 * @brief gets the data at the root of the heap but does not remove it
 * @param p_heap the heap to get the root from
 * @return pointer to the heap that is the root
 */
hnode * heap_peak(heap * p_heap);

/*
 * @brief gets the data at the root of the heap and removes it from the heap
 * @param p_heap the heap to get the root from
 * @return pointer to the heap that is the root
 */
hnode * heap_pull(heap * p_heap);
