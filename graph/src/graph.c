#include <graph.h>
#include <list.h>
#include <set.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * @brief node inside of a graph a.k.a vertex
 * @param p_data the data that the vertex holds
 * @param p_adjacent the set of adjacent vertices 
 */
struct graph_node {
    void * p_data;
    set * p_adjacent;
};

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
 * @brief creates and initializes a vertex 
 * @param p_data the data that the vertex should have
 */
static vertex * graph_vertex_init(void * p_data, void (* destroy)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2))
{
    // cant create a vertex with null data
    if (NULL == p_data){
        return NULL;
    }
    // create and allocate space for the vertex
    vertex * p_vertex = calloc(1, sizeof(*p_vertex));
    if (NULL == p_vertex){
        perror("graph_vertex_init ");
        return NULL;
    }
    // set the vertex values
    p_vertex->p_data = p_data;
    p_vertex->p_adjacent = set_init(destroy, compare);
    if (NULL == p_vertex->p_adjacent){
        perror("graph_vertex_int ");
        return NULL;
    }
    // return the new vertex
    return p_vertex;
}

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
    list_elem * p_elem = list_head(p_graph->p_vertices);
    for (int64_t index = 0; index < p_graph->vcount; index++){
        if (NULL != p_elem){
            vertex * p_vertex = (vertex *)list_data(p_elem);
            set_destroy(p_vertex->p_adjacent);
            free(p_vertex);
            p_elem = list_next(p_elem);
        }
    }
    // vertices list
    list_destroy(p_graph->p_vertices);
    free(p_graph);
}

/*
 * @brief inserts a new vertex into a graph
 * @param p_graph the graph to insert the vertex into
 * @param p_data the data for the new vertex
 * @return a pointer to the new vertex or NULL on error
 */
vertex * graph_ins_vertex(graph * p_graph, void * p_data)
{
    // can't create a vertex from null data or into a null graph
    if ((NULL == p_graph) || (NULL == p_data)){
        return NULL;
    }
    // do not allow insertion of duplicate vertexes
    if (NULL != list_search(p_graph->p_vertices, p_data)){
        return NULL;
    }
    // create the new vertex
    vertex * p_vertex = graph_vertex_init(p_data, p_graph->destroy, p_graph->compare);
    if (NULL == p_vertex){
        return NULL;
    }
    // add the new vertex to the graph
    list_ins_next(p_graph->p_vertices, NULL, p_vertex);
    // increase the vertices count in the graph and return the new vertex
    p_graph->vcount++; 
    return p_vertex;
}

int8_t graph_ins_edge(graph * p_grap, vertex * p_vertex1, vertex * p_vertex2);
int8_t graph_rm_vertex(graph * p_graph, void * p_data); 
int8_t graph_rm_edge(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
vertex ** graph_adjlist(graph * p_grap, vertex * p_vertex);
int8_t graph_is_adjacent(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);

/*
 * @brief gets the vertices count of the graph 
 * @param p_graph the graph to get the vertices count from
 * @return the number of vertices in a graph or -1 on error
 */
int64_t graph_vcount(graph * p_graph)
{
    // cant get the vertices count from a null graph
    if (NULL == p_graph){
        return -1;
    }
    return p_graph->vcount;
}

/*
 * @brief gets the edge count of the graph 
 * @param p_graph the graph to get the edge count from
 * @return the number of edges in a graph or -1 on error
 */
int64_t graph_ecount(graph * p_graph)
{
    // cant get the edge count from a null graph
    if (NULL == p_graph){
        return -1;
    }
    return p_graph->ecount;
}
