#include <test_avltree.h>
#include <check.h>
#include <avltree.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static avltree * p_tree = NULL;
static int num = 5;

int8_t test_compare(void * key1, void * key2)
{
    // return 0 if the numbers are equal
    // -1 if key1 < key2
    // 1 if key1 > key2
    int num1 = *(int *)key1;
    int num2 = *(int *)key2;
    return (num1 == num2) ? 0 : ((num1 < num2) ? -1 : 1);
}

static void setup_test_avltree(void)
{
    p_tree = avltree_init(&num, free, test_compare); 
}

static void teardown_test_avltree()
{
    avltree_destroy(p_tree);
}

START_TEST(test_avltree_init)
{
    ck_assert(NULL != p_tree);
} END_TEST

START_TEST(test_avltree_size)
{
    ck_assert_int_eq(1, avltree_size(p_tree));
} END_TEST

START_TEST(test_avltree_insert)
{
    int num2 = 10;
    avltree_insert(p_tree, &num2); 
    ck_assert_int_eq(2, avltree_size(p_tree));
} END_TEST

START_TEST(test_avltree_remove)
{
    int num2 = 10;
    int num3 = 3;
    int num4 = 20;
    avltree_insert(p_tree, &num2);
    avltree_insert(p_tree, &num3);
    avltree_insert(p_tree, &num4);
    ck_assert_int_eq(4, avltree_size(p_tree));
    avltree_remove(p_tree, &num2);
    ck_assert_int_eq(4, avltree_size(p_tree));
} END_TEST

Suite * suite_avltree(void)
{
    // create suite and case
    Suite * p_suite = suite_create("AVLTREE");
    TCase * p_case = tcase_create("CORE");
    // setup test fixture
    tcase_add_checked_fixture(p_case, setup_test_avltree, teardown_test_avltree);
    // add the test
    tcase_add_test(p_case, test_avltree_init);
    tcase_add_test(p_case, test_avltree_size);
    tcase_add_test(p_case, test_avltree_insert);
    tcase_add_test(p_case, test_avltree_remove);
    // add the case to the suite
    suite_add_tcase(p_suite, p_case);
    // return the suite
    return p_suite;
}
