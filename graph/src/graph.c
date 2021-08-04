#include <graph.h>
#include <list.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * @brief graph data type structure
 * @param vcount the number of vertices in the graph
 * @param ecount the number of edges in the graph
 * @param destroy user defined destroy function for the data in the vertices
 * @param compare user defined compare function for the data in the vertices
 * @param pp_vertices list of vertices in the graph
 */
struct graph {
    int64_t vcount;
    int64_t ecount;
    void (* destroy)(void * p_data);
    int8_t (* compare)(void * p_key1, void * p_key2);
    list * p_vertices;
};

/*
 * @brief allocates a graph structure and initializes the graphs values 
 * @param destroy user defined destroy function for the data in the vertices
 * @param compare user defined compare function for the data in the vertices
 * @return pointer to the newly allocated graph or NULL on error
 */
graph * graph_init(void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2))
{
    // the graph cannot work if there is no compare function
    if (NULL == compare){
        return NULL;
    }
    // allocate a new graph and assign its values
    graph * p_graph = calloc(1, sizeof(*p_graph));
    if (NULL == p_graph){
        perror("graph_init ");
        return NULL;
    }
    p_graph->vcount = 0;
    p_graph->ecount = 0;
    p_graph->destroy = destroy;
    p_graph->compare = compare;
    p_graph->p_vertices = list_init(destroy, compare);
    if (NULL == p_graph->p_vertices){
        perror("graph_init ");
        return NULL;
    }
    // return the newly allocated graph
    return p_graph;
}

/*
 * @brief tear down a graph structure and free all remaining memory
 * @param p_graph the graph to tear down
 */
void graph_destroy(graph * p_graph)
{
    // cant destroy a NULL graph
    if (NULL == p_graph){
        return;
    }
    // free all the allocated data in the vertices and the
    // vertices list
    list_destroy(p_graph->p_vertices);
    free(p_graph);
}

vertex * graph_ins_vertex(graph * p_graph, void * p_data);
int8_t graph_ins_edge(graph * p_grap, vertex * p_vertex1, vertex * p_vertex2);
int8_t graph_rm_vertex(graph * p_graph, void * p_data); 
int8_t graph_rm_edge(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
vertex ** graph_adjlist(graph * p_grap, vertex * p_vertex);
int8_t graph_is_adjacent(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
int64_t graph_vcount(graph * p_graph);
int64_t graph_ecount(graph * p_graph);
