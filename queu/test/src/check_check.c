#include <check.h>
#include <stdlib.h>
#include <test_queu.h>

int main(void)
{
    int num_failed = 0;
    // create the test suites
    Suite * p_queu = suite_queu();
    // create and add to suite runner
    SRunner * p_srunner = srunner_create(p_queu);
    srunner_set_fork_status(p_srunner, CK_NOFORK);
    // run all test
    srunner_run_all(p_srunner, CK_NORMAL);
    // save the number of test failed
    num_failed = srunner_ntests_failed(p_srunner);
    srunner_free(p_srunner);
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
// end of source
