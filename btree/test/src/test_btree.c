#include <test_btree.h>
#include <check.h>
#include <btree.h>


START_TEST(test_btree_init)
{
    ck_assert(0 == 0);
} END_TEST

Suite * suite_btree(void)
{
    Suite * p_suite = suite_create("BTREE");
    TCase * p_case = tcase_create("CORE");
    
    tcase_add_test(p_case, test_btree_init);

    suite_add_tcase(p_suite, p_case);
    return p_suite;
}
