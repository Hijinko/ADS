#ifndef _SET_H
#define _SET_H
#include <stdlib.h>
#include <stdbool.h>
#include <list.h>
typedef struct list members;
typedef struct list_elem member;
typedef struct set set;
set * set_init(void (* destroy)(void * p_data), int (* compare)(void * key1, void * key2));
void set_destroy(set * p_set);
member * set_insert(set * p_set, void * p_data);
int set_remove(set * p_set, void * p_data);
set * set_union(set * p_set1, set * p_set2);
set * set_intersection(set * p_set1, set * p_set2);
set * set_difference(set * p_set1, set * p_set2);
member * set_is_member(set * p_set, void * p_data);
bool set_is_subset(set * p_set, set * p_set2);
bool set_is_equal(set * p_set1, set * P_set2);
// getters
size_t set_size(set * p_set1);
#endif
