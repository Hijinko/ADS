#ifndef _LIST_H
#define _LIST_H
#include <stdint.h>
typedef struct list_elem list_elem;
typedef struct list list;

list * list_init(void (* destroy)(void * data), int8_t (* compare)(void * key1, void * key2));
void list_destroy(list * p_list_t);
list_elem * list_ins_next(list * p_list_t, list_elem * p_elem_t, void * p_data);
int8_t list_rm_next(list * p_list_t, list_elem * p_elem_t);
int8_t list_iter(list * p_list_t, void (* func)(list_elem * elem));
list_elem * list_search(list * p_list_t, void * data);
void list_data_swap(list_elem * p_elem1, list_elem * p_elem2);
void list_data_set(list_elem * p_elem, void * data);
// getters
uint16_t list_size (list * p_list_t);
void * list_data(list_elem * p_elem);
list_elem * list_head(list * p_list_t);
list_elem * list_next(list_elem * p_list_t);
int list_remove(list * p_list, void * p_data);
#endif

