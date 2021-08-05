#include <set.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct list members;
struct set {
    void (*destroy)(void * data);
    int8_t (*compare)(void * key1, void * key2);
    members * p_members;
    size_t size;
};

set * set_init(void (* destroy)(void * p_data), int8_t (* compare)(void * key1, void * key2))
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

static void set_add(set * p_dest, set * p_source)
{
    member * p_member = list_head(p_source->p_members);
    while (NULL != p_member){
        set_insert(p_dest, list_data(p_member));
        p_member = list_next(p_member);
    }
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

int8_t set_remove(set * p_set, void * p_data)
{
    // do not remove from NULL or empty list;
    if ((NULL == p_set) || (0 == p_set->size) || (NULL == p_data)){
        return -1;
    }
    int retval = list_remove(p_set->p_members, p_data);
    // removal was successful
    if (0 == retval){
        p_set->size--;
        return 0;
    }
    return -1;
}

set * set_union(set * p_set1, set * p_set2)
{
    // create a new set that will be the union
    set * p_setu = set_init(p_set1->destroy, p_set1->compare);
    // add the nodes from the sets
    set_add(p_setu, p_set1);
    set_add(p_setu, p_set2);
    // return the union set
    return p_setu;
}

set * set_intersection(set * p_set1, set * p_set2)
{
    // create the set for the intersection
    set * p_seti = set_init(p_set1->destroy, p_set2->compare);
    member * p_member1 = list_head(p_set1->p_members);
    member * p_member2 = list_head(p_set2->p_members);
    // try to get the smallest member to use as the iterator
    member * p_iter = (p_set1->size < p_set2->size) ? p_member1 : \
            ((p_set1->size > p_set2->size) ? p_member2 : p_member1);
    while(NULL != p_iter){
        if (set_is_member(p_set2, list_data(p_iter))){
            set_insert(p_seti, list_data(p_iter));
        }
        p_iter = list_next(p_iter);
    }
    // return intersection set
    return p_seti;
}

set * set_difference(set * p_set1, set * p_set2);

member * set_is_member(set * p_set, void * p_data)
{
    // do not search in a null or empty set or null data
    if ((NULL == p_set) || (0 == p_set->size) || (NULL == p_data)){
        return NULL;
    }
    // check if data is in the set
    member * p_member = list_search(p_set->p_members, p_data);
    return p_member;
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
