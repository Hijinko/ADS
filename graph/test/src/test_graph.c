#include <check.h>
#include <test_graph.h>
#include <graph.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static graph * p_graph = NULL;

static void test_free(void * data)
{
    free((char *)data); 
}

static int test_search(void * key1, void * key2)
{
    int match = strcmp((char *)key1, (char *)key2);
    return match;
}

static void start_graph(void)
{
    
}

static void teardown_graph(void)
{

}

START_TEST(test_graph_init)
{
    ck_assert(NULL != p_graph);    
} END_TEST

// create suite
Suite * suite_graph(void)
{
    Suite * p_suite = suite_create("graph");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_graph, teardown_graph);
    tcase_add_test(p_core, test_graph_init);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

