#include <check.h>
#include <time.h>
#include <stdlib.h>
#include <test_btree.h>

int main(void)
{
    // seed a random time
    time_t seed;
    srand(time(&seed));
    // variable to hold failed tests
    int num_failed = 0;
    // create the test suites
    Suite * p_btree = suite_btree();
    // create and add to suite runner
    SRunner * p_srunner = srunner_create(p_btree);
    srunner_set_fork_status(p_srunner, CK_NOFORK);
    // run all test
    srunner_run_all(p_srunner, CK_NORMAL);
    // save the number of test failed
    num_failed = srunner_ntests_failed(p_srunner);
    srunner_free(p_srunner);
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
// end of source
