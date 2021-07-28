#include <set.h>
#include <stdlib.h>

set * set_init(void * p_data, void (* destroy)(void * p_data), int (* compare)(void * key1, void * key2));
member * set_insert(set * p_set, void * p_data);
int set_remove(set * p_set, void * p_data);
set * set_union(set * p_set1, set * p_set2);
set * set_intersection(set * p_set1, set * p_set2);
set * set_difference(set * p_set1, set * p_set2);
bool set_is_member(set * p_set, void * p_data);
bool set_is_subset(set * p_set, set * p_set2);
bool set_is_equal(set * p_set1, set * P_set2);
