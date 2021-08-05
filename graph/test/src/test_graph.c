#include <check.h>
#include <test_graph.h>
#include <graph.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static graph * p_graph = NULL;
static int num1 = 10;
static int num2 = 20;

static int8_t test_compare(void * key1, void * key2)
{
    int num_k1 = *(int *)key1;
    int num_k2 = *(int *)key2;
    return (num_k1 < num_k2) ? -1 : ((num_k1 == num_k2) ? 0 : 1);
}

static void start_graph(void)
{
    p_graph = graph_init(NULL, test_compare);
    graph_ins_vertex(p_graph, &num1);
    graph_ins_vertex(p_graph, &num2);
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
    int num3 = 30;
    int num4 = 40;
    int num5 = 50;
    int num6 = 50;
    ck_assert(NULL != graph_ins_vertex(p_graph, &num3));
    ck_assert(NULL != graph_ins_vertex(p_graph, &num4));
    ck_assert(NULL != graph_ins_vertex(p_graph, &num5));
    ck_assert(NULL == graph_ins_vertex(p_graph, &num6));
} END_TEST

START_TEST(test_graph_vcount)
{
    ck_assert_int_eq(2, graph_vcount(p_graph));
} END_TEST

START_TEST(test_graph_ins_edge)
{
    ck_assert_int_eq(0, graph_ins_edge(p_graph, &num1, &num2));
} END_TEST

START_TEST(test_graph_ecount)
{
    int num3 = 30;
    graph_ins_vertex(p_graph, &num3);
    ck_assert_int_eq(0, graph_ins_edge(p_graph, &num1, &num2));
    ck_assert_int_eq(1, graph_ecount(p_graph));
    ck_assert_int_eq(0, graph_ins_edge(p_graph, &num1, &num3));
    ck_assert_int_eq(2, graph_ecount(p_graph));
    ck_assert_int_eq(-1, graph_ins_edge(p_graph, &num1, &num3));
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
    tcase_add_test(p_core, test_graph_vcount);
    tcase_add_test(p_core, test_graph_ins_edge);
    tcase_add_test(p_core, test_graph_ecount);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

