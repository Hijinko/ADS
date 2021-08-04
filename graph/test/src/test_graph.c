#include <check.h>
#include <test_graph.h>
#include <graph.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static graph * p_graph = NULL;

static int8_t test_compare(void * key1, void * key2)
{
    int num1 = *(int *)key1;
    int num2 = *(int *)key2;
    return (num1 < num2) ? -1 : ((num1 == num2) ? 0 : 1);
}

static void start_graph(void)
{
    p_graph = graph_init(NULL, test_compare);
}

static void teardown_graph(void)
{
    graph_destroy(p_graph); 
}

START_TEST(test_graph_init)
{
    ck_assert(NULL != p_graph);    
} END_TEST

START_TEST(test_graph_ins_vertex)
{
    int num1 = 10;
    ck_assert(NULL != graph_ins_vertex(p_graph, &num1));
} END_TEST

// create suite
Suite * suite_graph(void)
{
    Suite * p_suite = suite_create("graph");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_graph, teardown_graph);
    tcase_add_test(p_core, test_graph_init);
    tcase_add_test(p_core, test_graph_ins_vertex);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

