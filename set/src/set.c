#include <set.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct list members;
struct set {
    void (*destroy)(void * data);
    int (*compare)(void * key1, void * key2);
    members * p_members;
    size_t size;
};

set * set_init(void (* destroy)(void * p_data), int (* compare)(void * key1, void * key2))
{
    // create a list structure for the members 
    members * p_members = list_init(destroy, compare);
    if (NULL == p_members){
        return NULL;
    }
    // create the set structure and assign the values
    set * p_set = calloc(1, sizeof(*p_set));
    if (NULL == p_set){
        return NULL;
    }
    p_set->destroy = destroy;
    p_set->compare = compare;
    p_set->p_members = p_members;
    p_set->size = 0;
    return p_set;
}

void set_destroy(set * p_set)
{
    // do not tear down null or empty set
    if (NULL == p_set){
        return;
    }
    list_destroy(p_set->p_members); 
    free(p_set);
}

member * set_insert(set * p_set, void * p_data)
{
    // do not insert in null list
    if (NULL == p_set){
        return NULL;
    }
    if (set_is_member(p_set, p_data)){
        return NULL;
    }
    member * p_member = list_ins_next(p_set->p_members, NULL, p_data);
    p_set->size++;
    return p_member;
}

int set_remove(set * p_set, void * p_data)
{
    // do not remove from NULL or empty list;
    if ((NULL == p_set) || (0 == p_set->size) || (NULL == p_data)){
        return -1;
    }
    //list_remove(p_set, p_data);
    return 0;
}

set * set_union(set * p_set1, set * p_set2);
set * set_intersection(set * p_set1, set * p_set2);
set * set_difference(set * p_set1, set * p_set2);
member * set_is_member(set * p_set, void * p_data)
{
    // do not search in a null or empty set or null data
    if ((NULL == p_set) || (0 == p_set->size) || (NULL == p_data)){
        return false;
    }
    // check if data is in the set
    return list_search(p_set->p_members, p_data);
}

bool set_is_subset(set * p_set, set * p_set2);
bool set_is_equal(set * p_set1, set * P_set2);
// getters

size_t set_size(set * p_set)
{
    // don't get the size of a null set
    if (NULL == p_set){
        return 0;
    }
    return p_set->size;
}
