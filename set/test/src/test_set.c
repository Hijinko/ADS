#include <check.h>
#include <test_set.h>
#include <set.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int test_compare(void * key1, void * key2)
{
    return *(int *)key1 == *(int*)key2 ? 0 : -1;
}

static int num1 = 10;
static int num2 = 20;
static int num3 = 30;
static int num4 = 40;
static int num5 = 50;
static set * p_set1 = NULL;
static set * p_set2 = NULL;
static void start_set(void)
{
    p_set1 = set_init(NULL, test_compare); 
    p_set2 = set_init(NULL, test_compare); 
    set_insert(p_set1, &num1);
    set_insert(p_set1, &num2);
    set_insert(p_set1, &num3);

    set_insert(p_set2, &num1);
    set_insert(p_set2, &num2);
    set_insert(p_set2, &num3);
    set_insert(p_set2, &num4);
    set_insert(p_set2, &num5);
}

static void teardown_set(void)
{
    set_destroy(p_set1);
    set_destroy(p_set2);
}

START_TEST(test_set_init)
{
    ck_assert(NULL != p_set1);    
} END_TEST

START_TEST(test_set_insert)
{
    int num6 = 60; 
    int num6_too = 60; 
    int num7 = 70; 
    set_insert(p_set1, &num6);
    ck_assert_int_eq(4, set_size(p_set1));
    set_insert(p_set1, &num6_too);
    set_insert(p_set1, &num7);
    ck_assert_int_eq(5, set_size(p_set1));
} END_TEST

START_TEST(test_set_remove)
{
    int num2 = 20; 
    ck_assert_int_eq(0, set_remove(p_set1, &num2));
    ck_assert_int_eq(2, set_size(p_set1));
} END_TEST

START_TEST(test_set_union)
{
    set * p_setu = set_union(p_set1, p_set2);
    ck_assert_int_eq(5, set_size(p_setu));
    set_destroy(p_setu);
} END_TEST

START_TEST(test_set_intersection)
{
    set * p_seti = set_intersection(p_set1, p_set2);
    ck_assert_int_eq(3, set_size(p_seti));
    set_destroy(p_seti);
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
    tcase_add_test(p_core, test_set_union);
    tcase_add_test(p_core, test_set_intersection);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

