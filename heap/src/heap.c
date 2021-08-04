#include <heap.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * @param INITIAL_MEMBERS the initial size of the pp_array
 */
enum {INITIAL_MEMBERS = 5};
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
 * @param node_space number of nodes allocated for the heap
 * @param size the number of children in the heap
 * @param destroy the tear down function for the data in a heap node
 * @param compare the user defined compare function for the heap
 * @param pp_array an array of heap nodes in the heap
 */
struct heap {
    int ordering;
    uint64_t node_space;
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
    return p_heap->pp_array[index];
}

/*
 * @brief gets the root of a heap
 * @param p_heap the heap to get the root from
 * @return pointer to the root node in the heap
 */
static hnode * heap_root(heap * p_heap)
{
    // cant get the root from a null or empty heap
    if ((NULL == p_heap) || (0 == p_heap->size)){
        return NULL;
    }
    return heap_member(p_heap, 0);
}

/*
 * @brief get the last element in a heap
 * @param p_heap the heap to get the tail from
 * @return the tail node of the heap
 */
static hnode * heap_tail(heap * p_heap)
{
    // an empty heap does not have a tail
    if ((NULL == p_heap) || (0 == p_heap->size)){
        return NULL;
    }
    // return the tail
    return heap_member(p_heap, p_heap->size - 1);
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
 * @brief orders a min after inserting a new node heap
 * @param p_heap the heap to rebalance as a min heap
 */
static void heap_min_up(heap * p_heap)
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
 * @brief orders a max after inserting a new node heap
 * @param p_heap the heap to rebalance as a max heap
 */
static void heap_max_up(heap * p_heap)
{
    // while the tail node is less than its parent node
    // swap the values
    hnode * p_new_node = heap_tail(p_heap);
    while((NULL != heap_parent(p_heap, p_new_node)) \
        && (1 == p_heap->compare(p_new_node->p_data, (heap_parent(p_heap, p_new_node))->p_data))){
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
 * @brief rebalances a heap after inserting a new node
 * @param p_heap the heap to rebalance
 */
static void heap_bubble_up(heap * p_heap)
{
    // check if the last node in the heap is greater than or less than the parent
    // this is based on the ordering of the heap
    // min heap defined
    if (MIN == p_heap->ordering){
        heap_min_up(p_heap); 
    }
    else {
        heap_max_up(p_heap);
    }
}

/*
 * @brief rebalances a min heap after removing a new node 
 * @param p_heap the heap to rebalance
 */
static void heap_min_down(heap * p_heap)
{
    hnode * p_new_root = heap_root(p_heap);
    bool b_done = false;
    // continue to swap the root with its children until neither of its children is
    // less than the current node
    while (!b_done){
        // get the lesser value of the two children
        hnode * p_lesser_temp = NULL;
        hnode * p_left = heap_left(p_heap, p_new_root);
        hnode * p_right = heap_right(p_heap, p_new_root);
        if (-1 == p_heap->compare(p_left, p_right)){
            p_lesser_temp = p_left;
        }
        else {
            p_lesser_temp = p_right;
        }
        // swap the values if the new_root is less than the lesser child 
        if ((NULL != p_lesser_temp) && (-1 == p_heap->compare(p_new_root, p_lesser_temp))){
            // switch the node indexes
            int temp_index = p_lesser_temp->index; 
            p_lesser_temp->index = p_new_root->index;
            p_new_root->index = temp_index;
            // switch the nodes to their new indexes in the array
            p_heap->pp_array[p_new_root->index] = p_new_root;
            p_heap->pp_array[p_lesser_temp->index] = p_lesser_temp;
        }
        else {
            // if the new root children are not lesser than the new root then exit the loop
            b_done = true;
        }
    }
}

/*
 * @brief rebalances a max heap after removing a new node 
 * @param p_heap the heap to rebalance
 */
static void heap_max_down(heap * p_heap)
{
    hnode * p_new_root = heap_root(p_heap);
    bool b_done = false;
    // continue to swap the root with its children until neither of its children is
    // greater than the current node
    while (!b_done){
        // get the greater value of the two children
        hnode * p_greater_temp = NULL;
        hnode * p_left = heap_left(p_heap, p_new_root);
        hnode * p_right = heap_right(p_heap, p_new_root);
        if (1 == p_heap->compare(p_left, p_right)){
            p_greater_temp = p_left;
        }
        else {
            p_greater_temp = p_right;
        }
        // swap the values if the new_root is less than the lesser child 
        if ((NULL != p_greater_temp) && (1 == p_heap->compare(p_new_root, p_greater_temp))){
            // switch the node indexes
            int temp_index = p_greater_temp->index; 
            p_greater_temp->index = p_new_root->index;
            p_new_root->index = temp_index;
            // switch the nodes to their new indexes in the array
            p_heap->pp_array[p_new_root->index] = p_new_root;
            p_heap->pp_array[p_greater_temp->index] = p_greater_temp;
        }
        else {
            // if the new root children are not greater than the new root then exit
            b_done = true;
        }
    }

}

/*
 * @brief rebalances a heap after removing the root
 * @param p_heap the heap to rebalance
 */
static void heap_bubble_down(heap * p_heap){
    if (MIN == p_heap->ordering){
        heap_min_down(p_heap);
    }
    else {
       heap_max_down(p_heap); 
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
    p_heap->node_space = INITIAL_MEMBERS;
    p_heap->size = 0;
    p_heap->destroy = destroy;
    p_heap->compare = compare;
    p_heap->pp_array = calloc(p_heap->node_space, sizeof(*(p_heap->pp_array)));
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
        for (int64_t index = p_heap->size; index >= 0; index--){
            if (NULL != p_heap->destroy){
                p_heap->destroy((heap_member(p_heap, index))->p_data);    
            }
            free(p_heap->pp_array[index]);
        }
    free(p_heap->pp_array);
    free(p_heap);
}

/*
 * @brief gets the size of a heap
 * @param p_heap the heap to get the size of
 * @return the size of the heap or -1 on error
 */
int64_t heap_size(heap * p_heap)
{
    if (NULL == p_heap){
        return -1;
    }
    return p_heap->size;
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
    if (NULL == p_node){
        perror("heap insert ");
        return NULL;
    }
    p_node->p_data = p_data;
    p_node->index = p_heap->size;
    // assign the new node at the end of the heap
    p_heap->pp_array[p_node->index] = p_node;    
    p_heap->size++;
    // reallocate the array if needed
    // TODO
    // if the heap size is not zero then we have to rebalance the heap
    if (1 != p_heap->size){
        heap_bubble_up(p_heap);    
    }
    // return the node
    return p_node;
}

/*
 * @brief gets the data at the root of the heap but does not remove it
 * @param p_heap the heap to get the root from
 * @return pointer to the heap that is the root
 */
hnode * heap_peek(heap * p_heap)
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
hnode * heap_pull(heap * p_heap)
{
    // cant get the root of a NULL or empty heap
    if ((NULL == p_heap) || (0 == p_heap->size)){
        return NULL;
    }
    // store the old root value to hold the heap root 
    hnode * p_old_root = heap_member(p_heap, 0);
    // insert the last value as the new root
    hnode * p_new_root = heap_tail(p_heap); 
    p_heap->pp_array[0] = p_new_root;
    p_new_root->index = 0;
    // we are removing the old root so we have to decrease the size
    p_heap->size--;
    heap_bubble_down(p_heap);
    // while the new root is greater than or less than its children based on ordering
    // swap with the greater or lesser child
    // return the old root
    return p_old_root;
}

/*
 * @brief get the data in a heap node
 * @brief p_node the node to get the data from
 * @return void pointer to the nodes data
 */
void * heap_data(hnode * p_node)
{
    // cant get the data in a null node
    if (NULL == p_node){
        return NULL;
    }
    return p_node->p_data;
}
