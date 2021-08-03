#include <check.h>
#include <test_heap.h>
#include <heap.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int8_t test_compare(void * p_key1, void * p_key2){
    int key1 = *(int*)p_key1;
    int key2 = *(int*)p_key2;
    return (key1 < key2) ? -1 : ((key1 == key2) ? 0 : 1);
}

heap * p_heap = NULL;
static void start_heap(void)
{
    p_heap = heap_init(MAX, NULL, test_compare); 
}

static void teardown_heap(void)
{
    heap_destroy(p_heap);
}

START_TEST(test_heap_init)
{
    ck_assert(NULL != p_heap);    
} END_TEST

// create suite
Suite * suite_heap(void)
{
    Suite * p_suite = suite_create("List");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_heap, teardown_heap);
    tcase_add_test(p_core, test_heap_init);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

