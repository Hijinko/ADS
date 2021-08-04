#include <check.h>
#include <test_graph.h>
#include <graph.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static graph * p_graph = NULL;
static graph_elem * p_elem1 = NULL;
static graph_elem * p_elem2 = NULL;
static graph_elem * p_elem3 = NULL;
static graph_elem * p_elem4 = NULL;

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
    p_graph = graph_init(test_free, test_search); 
    char * p_name1 = calloc(20, sizeof(*p_name1));
    char * p_name2 = calloc(20, sizeof(*p_name2));
    char * p_name3 = calloc(20, sizeof(*p_name3));
    char * p_name4 = calloc(20, sizeof(*p_name4));
    strncpy(p_name1, "Kevin", strlen("Kevin") + 1);
    strncpy(p_name2, "Joe", strlen("Joe") + 1);
    strncpy(p_name3, "James", strlen("James") + 1);
    strncpy(p_name4, "Dave", strlen("Dave") + 1);
    p_elem1 = graph_ins_next(p_graph, NULL, p_name1);
    p_elem2 = graph_ins_next(p_graph, NULL, p_name2);
    p_elem3 = graph_ins_next(p_graph, NULL, p_name3);
    p_elem4 = graph_ins_next(p_graph, NULL, p_name4);
}

static void teardown_graph(void)
{
    graph_destroy(p_graph);
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
    tcase_add_test(p_core, test_graph_ins_next);
    tcase_add_test(p_core, test_graph_rm_next);    
    tcase_add_test(p_core, test_graph_search);    
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

