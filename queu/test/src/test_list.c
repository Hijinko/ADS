#include <check.h>
#include <test_queu.h>
#include <queu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static queu * p_queu = NULL;
static queu_elem * p_elem1 = NULL;
static queu_elem * p_elem2 = NULL;
static queu_elem * p_elem3 = NULL;
static queu_elem * p_elem4 = NULL;

static void test_free(void * data)
{
    free((char *)data); 
}

static int test_search(void * key1, void * key2)
{
    int match = strcmp((char *)key1, (char *)key2);
    return match;
}

static void start_queu(void)
{
    p_queu = queu_init(test_free, test_search); 
    char * p_name1 = calloc(20, sizeof(*p_name1));
    char * p_name2 = calloc(20, sizeof(*p_name2));
    char * p_name3 = calloc(20, sizeof(*p_name3));
    char * p_name4 = calloc(20, sizeof(*p_name4));
    strncpy(p_name1, "Kevin", strlen("Kevin") + 1);
    strncpy(p_name2, "Joe", strlen("Joe") + 1);
    strncpy(p_name3, "James", strlen("James") + 1);
    strncpy(p_name4, "Dave", strlen("Dave") + 1);
    p_elem1 = queu_ins_next(p_queu, NULL, p_name1);
    p_elem2 = queu_ins_next(p_queu, NULL, p_name2);
    p_elem3 = queu_ins_next(p_queu, NULL, p_name3);
    p_elem4 = queu_ins_next(p_queu, NULL, p_name4);
}

static void teardown_queu(void)
{
    queu_destroy(p_queu);
}

START_TEST(test_queu_init)
{
    ck_assert(NULL != p_queu);    
} END_TEST

// create suite
Suite * suite_queu(void)
{
    Suite * p_suite = suite_create("queu");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_queu, teardown_queu);
    tcase_add_test(p_core, test_queu_init);
    tcase_add_test(p_core, test_queu_ins_next);
    tcase_add_test(p_core, test_queu_rm_next);    
    tcase_add_test(p_core, test_queu_search);    
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

