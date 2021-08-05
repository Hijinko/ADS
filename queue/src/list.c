#include <list.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct list_elem {
    void * p_data;
    struct list_elem * p_prev;
    struct list_elem * p_next;
};

struct list {
    uint16_t size;
    void (* destroy)(void * data);
    int8_t (* compare)(void * key1, void * key2);
    list_elem * p_head;
    list_elem * p_tail;
};

/*
 * @brief initializes a list
 * @return pointer to a newly malloced list
 */
list * list_init(void (* destroy)(void * data), int8_t (* compare)(void * key1, void * key2)){
    list * p_list_t = calloc(1, sizeof(* p_list_t));
    p_list_t->size = 0;
    p_list_t->destroy = destroy;
    p_list_t->compare = compare;
    p_list_t->p_head = NULL;
    p_list_t->p_tail = NULL;
    return p_list_t;
}

/*
 * @brief frees a list and its elements
 * @param p_list_t pointer to list to free 
 */
void list_destroy(list * p_list_t)
{
    while(p_list_t->size > 0){
        list_rm_next(p_list_t, NULL);
    }
    free(p_list_t);
}

/*
 * @brief inserts a new list element after the passed element
 * @param p_list_t list to insert element into
 * @param p_elem_t element in list to insert after
 * @param p_data data for new element
 * @return 0 if successful else -1
 */
list_elem * list_ins_next(list * p_list_t, list_elem * p_elem_t, void * p_data)
{
    // handle invalid list
    if (NULL == p_list_t){
        return NULL;
    }
    // create a new element
    list_elem * p_new_t = calloc(1, sizeof(*p_new_t));
    p_new_t->p_data = p_data;
    p_new_t->p_prev = NULL;
    p_new_t->p_next = NULL;
    // handle empty list
    if (0 == p_list_t->size){
        p_list_t->p_head = p_new_t;
        p_list_t->p_tail = p_new_t;
    }
    else if (p_elem_t == NULL){
        // handle insert at head
        p_new_t->p_next = p_list_t->p_head;
        p_list_t->p_head = p_new_t;
        if (NULL != p_new_t->p_next){
            p_new_t->p_next->p_prev = p_new_t;
        }
    }
    else {
    // handle insert anywhere else
        p_new_t->p_next = p_elem_t->p_next;
        p_elem_t->p_next = p_new_t;
        p_new_t->p_prev = p_elem_t;
        if (NULL != p_new_t->p_next){
            p_new_t->p_next->p_prev = p_new_t;
        }
        // handle a new tail
        if (p_list_t->p_tail == p_elem_t){
            p_list_t->p_tail = p_new_t;
        }
    }
    // update size
    p_list_t->size++;
    // return element
    return p_new_t;
}

/*
 * @brief removes the node after the element specified in list
 * @param p_list_t pointer to list to remove element from
 * @param p_elem_t pointer to element before the element to remove
 * @return 0 on success else -1
 */
int8_t list_rm_next(list * p_list_t, list_elem * p_elem_t)
{
    // ensure list is not null or empty
    if ((NULL == p_list_t) || (0 == p_list_t->size)){
        return -1;
    }
    // check if removing from the head
    list_elem * p_old = NULL;
    if (NULL == p_elem_t){
        p_old = p_list_t->p_head; 
        p_list_t->p_head = p_old->p_next;
    }
    else {
        p_old = p_elem_t->p_next;
        if (NULL == p_old){
            return -1;
        }
        p_elem_t->p_next = p_old->p_next;
        if (NULL != p_old->p_next){
            p_old->p_next->p_prev = p_elem_t; 
        }
    }
    // check if removing the tail
    if (p_old == p_list_t->p_tail){
        p_list_t->p_tail = p_old->p_prev;
    }
    // free any user defined data if a destroy function was specified
    if (NULL != p_list_t->destroy){
        p_list_t->destroy(p_old->p_data);
    }
    // free the old element
    free(p_old);
    // decrease list size
    p_list_t->size--;
    return 0;
}

/*
 * @brief iterates over a list and conducts a function on each element
 * @param p_list_t pointer to list to iterate through
 * @param func function to run on each list element
 * @return 0 on success else -1
 */
int8_t list_iter(list * p_list_t, void (* func)(list_elem * elem))
{
    // check for invalid list or function
    if ((NULL == p_list_t) || (0 == p_list_t->size) || (NULL == func)){
        return -1;    
    }

    list_elem * p_elem_t = p_list_t->p_head;
    while(NULL != p_elem_t){
        func(p_elem_t);
        p_elem_t = p_elem_t->p_next;
    }

    return 0;
}

list_elem * list_search(list * p_list_t, void * data)
{
    // check for valid values
    if ((NULL == p_list_t) || (NULL == data) || (NULL == p_list_t->compare)){
        return NULL;
    }

    // iterate over list and compare
    list_elem * p_temp = NULL;
    list_elem * p_elem_t = p_list_t->p_head;
    while(NULL != p_elem_t){
        if (0 == (p_list_t->compare(p_elem_t->p_data, data))){
            p_temp = p_elem_t; 
        }; 
        p_elem_t = p_elem_t->p_next;
    }
    return p_temp;
}

void list_data_swap(list_elem * p_elem1, list_elem * p_elem2)
{
    void * p_temp = p_elem1->p_data;
    p_elem1->p_data = p_elem2->p_data;
    p_elem2->p_data = p_temp;
}

void list_data_set(list_elem * p_elem, void * data)
{
    p_elem->p_data = data;
}

// getters
uint16_t list_size (list * p_list_t)
{
    return p_list_t->size;
}

void * list_data(list_elem * p_elem)
{
    return p_elem->p_data;
}

list_elem * list_head(list * p_list_t)
{
    return p_list_t->p_head;
}

int list_remove(list * p_list, void * p_data)
{
    // do not remove from null list or list that is empty;
    if ((NULL == p_list) || (0 == p_list->size) || (NULL == p_data)){
        return -1;
    }
    // find the data in the list
    list_elem * p_old = list_search(p_list, p_data);
    if (NULL == p_old){
        return -1;
    }
    // check if there is a new head
    if (p_list->p_head == p_old){
        p_list->p_head = p_old->p_next;
        if (NULL != p_old->p_next){
            p_old->p_next->p_prev = NULL;
        }
    }
    else {
        //remove from anywhere else
        p_old->p_prev->p_next = p_old->p_next;
        if (NULL != p_old->p_next){
            p_old->p_next->p_prev = p_old->p_prev;
        }
    }
    // check if there is a new tail
    if (p_list->p_tail == p_old){
        p_list->p_tail = p_old->p_prev;
    }
    // destroy the list data if user defined destroy exists
    if (NULL != p_list->destroy){
        p_list->destroy(p_old->p_data);
    }
    free(p_old);
    p_list->size--;
    return 0;
}


/*
 * @brief gets the next element from a list_element
 * @param p_elem the list element to get the next element from
 */
list_elem * list_next(list_elem * p_elem)
{
    // cant get the next element from a null element
    if (NULL == p_elem){
        return NULL;
    }
    return p_elem->p_next;
}

list_elem * list_tail(list * p_list)
{
    // cant get the tail from a null list
    if (NULL == p_list){
        return NULL;
    }
    return p_list->p_tail;
   
}
