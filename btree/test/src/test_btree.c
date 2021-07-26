#include <test_btree.h>
#include <check.h>
#include <btree.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

START_TEST(test_btree_init)
{
    // create a random number between 1 and 100;
    int8_t num = rand() % 100 + 1; 
    // create the tree
    btree * p_tree = btree_init(&num, NULL, NULL);
    // check for successful creation;
    ck_assert_int_eq(1, btree_size(p_tree));
    btnode * p_root = btree_root(p_tree);
    ck_assert(NULL != p_root);
    ck_assert_int_eq(num, *(int8_t *)btree_data(p_root));
    btree_destroy(p_tree);
} END_TEST

Suite * suite_btree(void)
{
    Suite * p_suite = suite_create("BTREE");
    TCase * p_case = tcase_create("CORE");
    
    tcase_add_test(p_case, test_btree_init);

    suite_add_tcase(p_suite, p_case);
    return p_suite;
}
