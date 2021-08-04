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
int num = 10;
static void start_heap(void)
{
    p_heap = heap_init(MAX, NULL, test_compare); 
    heap_insert(p_heap, &num);
}

static void teardown_heap(void)
{
    heap_destroy(p_heap);
}

START_TEST(test_heap_init)
{
    ck_assert(NULL != p_heap);    
} END_TEST

START_TEST(test_heap_size)
{
    ck_assert_int_eq(1, heap_size(p_heap));
} END_TEST

START_TEST(test_heap_insert)
{
    int num2 = 20;
    int num4 = 40;
    int num3 = 15;
    ck_assert(NULL != heap_insert(p_heap, &num2));
    heap_insert(p_heap, &num3);
    heap_insert(p_heap, &num4);
    ck_assert_int_eq(num4, *(int *)heap_data(heap_peek(p_heap)));
} END_TEST

START_TEST(test_heap_peek)
{
    ck_assert(NULL != heap_peek(p_heap));
} END_TEST

START_TEST(test_heap_data)
{
    ck_assert_int_eq(10, *(int *)(heap_data(heap_peek(p_heap))));
} END_TEST

START_TEST(test_heap_init_null)
{
    ck_assert(NULL == heap_init(MIN, NULL, NULL));
} END_TEST

START_TEST(test_heap_init_min)
{
    heap * p_min_heap = heap_init(MIN, NULL, test_compare);
    int num1 = 10;
    int num2 = 20;
    int num3 = 30;
    heap_insert(p_min_heap, &num1);
    heap_insert(p_min_heap, &num2);
    heap_insert(p_min_heap, &num3);
    ck_assert_int_eq(num1, *(int *)(heap_data(heap_peek(p_min_heap))));
    heap_destroy(p_min_heap);
} END_TEST

// create suite
Suite * suite_heap(void)
{
    Suite * p_suite = suite_create("Heap");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_heap, teardown_heap);
    tcase_add_test(p_core, test_heap_init);
    tcase_add_test(p_core, test_heap_size);
    tcase_add_test(p_core, test_heap_insert);
    tcase_add_test(p_core, test_heap_peek);
    tcase_add_test(p_core, test_heap_data);
    tcase_add_test(p_core, test_heap_init_null);
    tcase_add_test(p_core, test_heap_init_min);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

