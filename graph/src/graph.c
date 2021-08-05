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
 * @brief searches a graph for a vertex value
 * @param p_graph the graph to search in
 * @param p_data the vertex data to search for
 * @return a pointer to the found vertex or NULL if a matching vertex didn't exist
 */
static vertex * graph_search(graph * p_graph, void * p_data)
{
    // cant search for a value in a null graph or a graph that has no
    // vertices, also cant search for null data
    if ((NULL == p_graph) || (0 == p_graph->vcount) || (NULL == p_data)){
        return NULL;
    }
    // search for the value in all the vertexes 
    list_elem * p_elem = list_head(p_graph->p_vertices);
    for (; NULL != p_elem; p_elem = list_next(p_elem)){
        if (0 == p_graph->compare(((vertex *)list_data(p_elem))->p_data, p_data)){
            // a match was found so return the vertex
            return (vertex *)list_data(p_elem);
        }
    }
    // no match was found
    return NULL;
}

/*
 * @brief creates and initializes a vertex 
 * @param p_data the data that the vertex should have
 */
static vertex * graph_vertex_init(void * p_data, void (* destroy)(void * p_data),\
                                  int8_t (* compare)(void * p_key1, void * p_key2))
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
    if (NULL != graph_search(p_graph, p_data)){
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

/*
 * @brief adds a edge from one vertex to another
 * @param p_graph the graph to insert the edge into
 * @param p_data1 the vertex the edge comes from
 * @param p_data2 the vertex the edge goes into
 * @return 0 if edge added successfully else -1
 */
int8_t graph_ins_edge(graph * p_graph, void * p_data1, void * p_data2)
{
    // cant add an edge in an empty or null graph and cant add a edge if
    // either vertex is null
    if ((NULL == p_graph) || (0 == p_graph->vcount) || \
        (NULL == p_data1) || (NULL == p_data2)){
        return -1;     
    }
    // get the vertex for p_data1
    vertex * p_vertex1 = graph_search(p_graph, p_data1);
    if (NULL == p_vertex1){
        return -1;
    }
    // get the vertex for p_data2
    vertex * p_vertex2 = graph_search(p_graph, p_data2);
    if (NULL == p_vertex2){
        return -1;
    }
    // create the edge between the vertices  
    if (NULL == set_insert(p_vertex1->p_adjacent, p_data2)){
        return -1;
    }
    // increase edge count in the graph
    p_graph->ecount++;
    return 0;
}

int8_t graph_rm_vertex(graph * p_graph, void * p_data); 
int8_t graph_rm_edge(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
vertex ** graph_adjlist(graph * p_graph, vertex * p_vertex);

/*
 * @brief checks if a vertex is adjacent to another vertex in a graph
 * @param p_graph the graph to check in
 * @param p_data1 the data in the vertex to check adjacency from
 * @param p_data2 the data in the vertex to check adjacency to
 * @return 0 if data2 is adjacent to data1 or -1 if they are not adjacent
 */
int8_t graph_is_adjacent(graph * p_graph, void * p_data1, void * p_data2)
{
    // cant search in a NULL graph, a graph with no edges or for NULL vertices
    if ((NULL == p_graph) || (0 == p_graph->ecount) ||\
        (NULL == p_data1) || (NULL == p_data2)){
        return -1;
    }
    // get the vertex that contains data1 
    vertex * p_vertex1 = graph_search(p_graph, p_data1);
    // if the data is not in the graph then the values aren't adjacent
    if (NULL == p_vertex1){
        return -1;
    }
    // get the vertex that contains data2 
    vertex * p_vertex2 = graph_search(p_graph, p_data2);
    // if the data is not in the graph then the values aren't adjacent
    if (NULL == p_vertex2){
        return -1;
    }
    // see if p_data2 is adjacent to the vertex that contains p_data1
    if (NULL == set_is_member(p_vertex1->p_adjacent, p_data2)){
        return -1;
    }
    return 0;
}

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
