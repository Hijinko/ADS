#include <check.h>
#include <test_queue.h>
#include <queue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static queue * p_queue = NULL;
static queue_elem * p_elem1 = NULL;
static queue_elem * p_elem2 = NULL;
static queue_elem * p_elem3 = NULL;
static queue_elem * p_elem4 = NULL;

static void test_free(void * data)
{
    free((char *)data); 
}

static int test_search(void * key1, void * key2)
{
    int match = strcmp((char *)key1, (char *)key2);
    return match;
}

static void start_queue(void)
{
    p_queue = queue_init(test_free, test_search); 
    char * p_name1 = calloc(20, sizeof(*p_name1));
    char * p_name2 = calloc(20, sizeof(*p_name2));
    char * p_name3 = calloc(20, sizeof(*p_name3));
    char * p_name4 = calloc(20, sizeof(*p_name4));
    strncpy(p_name1, "Kevin", strlen("Kevin") + 1);
    strncpy(p_name2, "Joe", strlen("Joe") + 1);
    strncpy(p_name3, "James", strlen("James") + 1);
    strncpy(p_name4, "Dave", strlen("Dave") + 1);
    p_elem1 = queue_ins_next(p_queue, NULL, p_name1);
    p_elem2 = queue_ins_next(p_queue, NULL, p_name2);
    p_elem3 = queue_ins_next(p_queue, NULL, p_name3);
    p_elem4 = queue_ins_next(p_queue, NULL, p_name4);
}

static void teardown_queue(void)
{
    queue_destroy(p_queue);
}

START_TEST(test_queue_init)
{
    ck_assert(NULL != p_queue);    
} END_TEST

// create suite
Suite * suite_queue(void)
{
    Suite * p_suite = suite_create("queue");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_queue, teardown_queue);
    tcase_add_test(p_core, test_queue_init);
    tcase_add_test(p_core, test_queue_ins_next);
    tcase_add_test(p_core, test_queue_rm_next);    
    tcase_add_test(p_core, test_queue_search);    
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

