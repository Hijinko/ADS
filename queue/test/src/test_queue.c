#include <check.h>
#include <test_queue.h>
#include <queue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static queue * p_queue = NULL;

static int8_t test_search(void * key1, void * key2)
{
    int match = strcmp((char *)key1, (char *)key2);
    return match;
}

static void start_queue(void)
{
    p_queue = queue_init(NULL, test_search); 
}

static void teardown_queue(void)
{
    queue_destroy(p_queue);
}

START_TEST(test_queue_init)
{
    ck_assert(NULL != p_queue);    
} END_TEST

START_TEST(test_queue_enqueue)
{
    int num1 = 10;
    int num2 = 20;
    ck_assert(0 == queue_enqueue(p_queue, &num1));
    ck_assert(0 == queue_enqueue(p_queue, &num2));
} END_TEST

// create suite
Suite * suite_queue(void)
{
    Suite * p_suite = suite_create("queue");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_queue, teardown_queue);
    tcase_add_test(p_core, test_queue_init);
    tcase_add_test(p_core, test_queue_enqueue);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

