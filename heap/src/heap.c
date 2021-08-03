#include <heap.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * @param INITIAL_MEMBERS the initial size of the pp_array
 */
enum {INITIAL_MEMBERS = 10};
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
 * @param size the number of children in the heap
 * @param destroy the tear down function for the data in a heap node
 * @param compare the user defined compare function for the heap
 * @param pp_array an array of heap nodes in the heap
 */
struct heap {
    int ordering;
    int64_t size;
    void (* destroy)(void * p_data);
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
    // return the tail
    return heap_member(p_heap, p_heap->size);
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

    int index = ((p_node->index - 1) / 2);
    return heap_member(p_heap, index);
}

/*
 * @brief get the left child of a heap in a base 0 index
 * @param p_heap the heap to search in
 * @param p_node the node to get the left child from
 * @return the left child node of the passed in node
 */
static hnode * heap_left(heap * p_heap, hnode * p_node)
{
    int index = ((2 * p_node->index) + 1);
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
    int index = ((2 * p_node->index) + 2);
    return heap_member(p_heap, index);
}

/*
 * @brief orders a min heap
 * @param p_heap the heap to rebalance as a min heap
 */
static void heap_min_heap(heap * p_heap)
{
    // while the tail node is less than its parent node
    // swap the values
    hnode * p_new_node = heap_tail(p_heap);
    while((NULL != heap_parent(p_heap, p_new_node)) \
        && (-1 == p_heap->compare(p_new_node->p_data, (heap_parent(p_heap, p_new_node))->p_data))){
        // switch the node indexes
        hnode * p_temp_node = heap_parent(p_heap, p_new_node);
        int temp_index = p_temp_node->index;
        p_temp_node->index = p_new_node->index;
        p_new_node->index = temp_index;
        // switch the nodes to their new indexes in the array
        p_heap->pp_array[p_new_node->index] = p_new_node;
        p_heap->pp_array[p_temp_node->index] = p_temp_node;
    }
}

/*
 * @brief orders a max heap
 * @param p_heap the heap to rebalance as a max heap
 */
static void heap_max_heap(heap * p_heap)
{
    return; 
}

/*
 * @brief rebalances a heap
 * @param p_heap the heap to rebalance
 */
static void heap_heapify(heap * p_heap)
{
    // check if the last node in the heap is greater than or less than the parent
    // this is based on the ordering of the heap
    // min heap defined
    if (MIN == p_heap->ordering){
        heap_min_heap(p_heap); 
    }
    else {
        heap_max_heap(p_heap);
    }
}

/*
 * @brief allocate and initialize a heap
 * @param ordering integer identifying if the heap should be min or max heap 0 or 1 respectively   
 * @param destroy user defined function to tear down the nodes data
 * @param compare user defined function to compare the data in the nodes should return -1 0 or 1
 * @return a newly initialized heap or NULL on error
 */
heap * heap_init(int ordering, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2))
{
    // create the heap
    heap * p_heap = calloc(1, sizeof(*p_heap));
    if (NULL == p_heap){
        return NULL;
    }
    // initialize the values
    p_heap->ordering = ordering;
    p_heap->size = 0;
    p_heap->destroy = destroy;
    p_heap->compare = compare;
    p_heap->pp_array = calloc(INITIAL_MEMBERS, sizeof(hnode));
    if (NULL == p_heap->pp_array){
        return NULL;
    }
    return p_heap;
}

/*
 * @brief tear down a heap
 * @param p_heap the heap to tear down
 */
void heap_destroy(heap * p_heap)
{
    // cant destroy a NULL heap
    if (NULL == p_heap){
        return;
    }
    // perform user defined destroy
    if (NULL != p_heap->destroy){
        for (int64_t index = p_heap->size; index > 0; index--){
            p_heap->destroy((heap_member(p_heap, index))->p_data);    
        }
    }
    free(p_heap->pp_array);
    free(p_heap);
}

/*
 * @brief adds a new node to a heap
 * @param p_heap the heap to insert the node into
 * @param p_data the data of the new node
 * @return pointer to the new heap node inside the heap or NULL on error
 */
hnode * heap_insert(heap * p_heap, void * p_data)
{
    // cant insert into a null heap or from null data
    if ((NULL == p_heap) || (NULL == p_data)){
        return NULL;
    }
    // create the new node
    hnode * p_node = calloc(1, sizeof(*p_node));
    p_node->p_data = p_data;
    p_node->index = p_heap->size;
    if (NULL == p_node){
        return NULL;
    }
    // assign the new node at the end of the heap
    p_heap->pp_array[p_node->index] = p_node;    
    p_heap->size++;
    // if the heap size is not zero then we have to rebalance the heap
    if (0 != p_heap->size){
        heap_heapify(p_heap);    
    }
    // return the node
    return p_node;
}

/*
 * @brief gets the data at the root of the heap but does not remove it
 * @param p_heap the heap to get the root from
 * @return pointer to the heap that is the root
 */
hnode * heap_peak(heap * p_heap)
{
    // cant peek at a null or empty heap
    if ((NULL == p_heap) || (0 == p_heap->size)){
        return NULL;
    }
    return heap_member(p_heap, 0);
}

/*
 * @brief gets the data at the root of the heap and removes it from the heap
 * @param p_heap the heap to get the root from
 * @return pointer to the heap that is the root
 */
hnode * heap_pull(heap * p_heap);