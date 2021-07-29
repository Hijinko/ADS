#include <check.h>
#include <test_set.h>
#include <set.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
static void test_free(void * data)
{
    free((char *)data); 
}

static int test_search(void * key1, void * key2)
{
    int match = strcmp((char *)key1, (char *)key2);
    return match;
}
*/

static int test_compare(void * key1, void * key2)
{
    return *(int *)key1 == *(int*)key2 ? 0 : -1;
}

set * p_set1 = NULL;
static void start_set(void)
{
    p_set1 = set_init(NULL, test_compare); 
}

static void teardown_set(void)
{
    set_destroy(p_set1);
}

START_TEST(test_set_init)
{
    ck_assert(NULL != p_set1);    
} END_TEST

START_TEST(test_set_insert)
{
    int num1 = 10; 
    int num2 = 10; 
    int num3 = 30; 
    set_insert(p_set1, &num1);
    ck_assert_int_eq(1, set_size(p_set1));
    set_insert(p_set1, &num2);
    set_insert(p_set1, &num3);
    ck_assert_int_eq(2, set_size(p_set1));
} END_TEST

START_TEST(test_set_remove)
{
    int num1 = 10; 
    int num2 = 20; 
    int num3 = 30; 
    set_insert(p_set1, &num1);
    set_insert(p_set1, &num2);
    set_insert(p_set1, &num3);
    ck_assert_int_eq(3, set_size(p_set1));
    ck_assert_int_eq(0, set_remove(p_set1, &num2));
    ck_assert_int_eq(2, set_size(p_set1));
} END_TEST

// create suite
Suite * suite_set(void)
{
    Suite * p_suite = suite_create("SET");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_set, teardown_set);
    tcase_add_test(p_core, test_set_init);
    tcase_add_test(p_core, test_set_insert);
    tcase_add_test(p_core, test_set_remove);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

