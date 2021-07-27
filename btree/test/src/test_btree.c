#include <test_btree.h>
#include <check.h>
#include <btree.h>
#include <stdio.h>

static btree * p_tree = NULL;
static int num = 5;

static void setup_test_btree(void)
{
    p_tree = btree_init(&num, NULL, NULL); 
}

static void teardown_test_btree()
{
    btree_destroy(p_tree);
    p_tree = NULL;
}

START_TEST(test_btree_init)
{
    ck_assert(NULL != p_tree);
} END_TEST

START_TEST(test_btree_data)
{
    ck_assert_int_eq(num, *(int *)btree_data(p_tree, btree_root(p_tree))); 
} END_TEST

START_TEST(test_btree_right)
{
    ck_assert(NULL == btree_right(p_tree, NULL));
} END_TEST

START_TEST(test_btree_left)
{
    ck_assert(NULL == btree_left(p_tree, NULL));
} END_TEST

START_TEST(test_btree_size)
{
    ck_assert_int_eq(1, btree_size(p_tree));
} END_TEST

START_TEST(test_btree_ins_left)
{
    int num_left = 2;
    btnode * p_node = btree_ins_left(p_tree, btree_root(p_tree), &num_left);
    ck_assert(NULL != p_node);
    ck_assert_int_eq(num_left, *(int *)btree_data(p_tree, p_node));
    ck_assert_int_eq(2, btree_size(p_tree));
} END_TEST

START_TEST(test_btree_ins_right)
{
    int num_right = 10;
    btnode * p_node = btree_ins_right(p_tree, btree_root(p_tree), &num_right);
    ck_assert(NULL != p_node);
    ck_assert_int_eq(num_right, *(int *)btree_data(p_tree, p_node));
    ck_assert_int_eq(2, btree_size(p_tree));
} END_TEST

START_TEST(test_btree_rm_left)
{
    btree_rm_left(p_tree, NULL);    
    ck_assert_int_eq(0, btree_size(p_tree));
    ck_assert(NULL == btree_root(p_tree));
} END_TEST

START_TEST(test_btree_rm_right)
{
    btree_rm_right(p_tree, NULL);
    ck_assert_int_eq(0, btree_size(p_tree));
    ck_assert(NULL == btree_root(p_tree));
} END_TEST

Suite * suite_btree(void)
{
    // create suite and case
    Suite * p_suite = suite_create("BTREE");
    TCase * p_case = tcase_create("CORE");
    // setup test fixture
    tcase_add_checked_fixture(p_case, setup_test_btree, teardown_test_btree);
    // add the test
    tcase_add_test(p_case, test_btree_init);
    tcase_add_test(p_case, test_btree_data);
    tcase_add_test(p_case, test_btree_right);
    tcase_add_test(p_case, test_btree_left);
    tcase_add_test(p_case, test_btree_size);
    tcase_add_test(p_case, test_btree_ins_left);
    tcase_add_test(p_case, test_btree_ins_right);
    tcase_add_test(p_case, test_btree_rm_left);
    tcase_add_test(p_case, test_btree_rm_right);
    // add the case to the suite
    suite_add_tcase(p_suite, p_case);
    // return the suite
    return p_suite;
}
