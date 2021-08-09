#include <graph.h>
#include <list.h>
#include <queue.h>
#include <set.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum Color {WHITE, GREY, BLACK} Color;

/*
 * @brief node inside of a graph a.k.a vertex
 * @param hops count used for bfs
 * @param color color of the node used for bfs
 * @param p_data the data that the vertex holds
 * @param p_adjacent the set of adjacent vertices 
 */
struct graph_node {
    int16_t hops;
    Color color;
    void * p_data;
    set * p_adjacent;
};

/*
 * @brief graph data type structure
 * @param vcount the number of vertices in the graph
 * @param ecount the number of edges in the graph
 * @param destroy user defined destroy function for the data in the vertices
 * @param compare user defined compare function for the data in the vertices
 * @param p_vertices list of vertices in the graph
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
vertex * graph_search(graph * p_graph, void * p_data)
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
 * @return a pointer to newly created vertex or NULL on error
 */
static vertex * graph_vertex_init(void * p_data, int8_t (* compare)(void * p_key1, void * p_key2))
{
    // cant create a vertex with null data
    if (NULL == p_data){
        perror("graph_vertex_init NULL ");
        return NULL;
    }
    // create and allocate space for the vertex
    vertex * p_vertex = calloc(1, sizeof(*p_vertex));
    if (NULL == p_vertex){
        perror("graph_vertex_init ");
        return NULL;
    }
    // set the vertex values
    p_vertex->hops = -1;
    p_vertex->color = WHITE;
    p_vertex->p_data = p_data;
    p_vertex->p_adjacent = set_init(NULL, compare);
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
    p_graph->p_vertices = list_init(NULL, compare);
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
            if ((NULL != p_graph->destroy) && (NULL != p_vertex->p_data)){
                p_graph->destroy(p_vertex->p_data);   
            }
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
    vertex * p_vertex = graph_vertex_init(p_data, p_graph->compare);
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
    if (NULL == graph_search(p_graph, p_data2)){
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

/*
 * @brief adds a undirected edge from one vertex to another
 * @param p_graph the graph to insert the edge into
 * @param p_data1 the first vertex the edge connects
 * @param p_data2 the second vertex the edge connects
 * @return 0 if edge added successfully else -1
 */
int8_t graph_ins_undirected_edge(graph * p_graph, void * p_data1, void * p_data2)
{
    int8_t first_edge = 0;
    int8_t second_edge = 0;
    first_edge = graph_ins_edge(p_graph, p_data1, p_data2);
    second_edge = graph_ins_edge(p_graph, p_data2, p_data1);
    // if either edge is 0 then successful edge was added
    // if both edge is -1 then undirected edge was not added successfully
    if ((-1 == first_edge) && (-1 == second_edge)){
        return -1;
    }
    return 0; 
}

/*
 * @brief removes a vertex from a graph
 * @param p_graph the graph to remove the vertex from
 * @param p_data the data that the vertex to remove contains  
 * @return 0 if the vertex is successfully removed else -1
 */
int8_t graph_rm_vertex(graph * p_graph, void * p_data)
{
    // cant remove a node from a null or empty graph and cant remove NULL vertex
    if ((NULL == p_graph) || (0 == p_graph->vcount) || (NULL == p_data)){
        return -1;
    }
    // find the vertex to remove
    vertex * p_vertex = graph_search(p_graph, p_data);
    // cant remove a vertex that doesn't exist in the graph
    if (NULL == p_vertex){
        return -1;
    }
    // remove the set from the vertex
    set_destroy(p_vertex->p_adjacent);
    free(p_vertex);
    // decrease the graph vertex count
    p_graph->vcount--;
    return 0;
}

/*
 * @brief removes an edge from a graph
 * @param p_graph the graph to remove the edge from
 * @param p_data1 the data in the vertex to remove the edge from
 * @param p_data2 the data in the vertex to remove the edge to
 * @return 0 if edge was removed successfully else -1
 */
int8_t graph_rm_edge(graph * p_graph, void * p_data1, void * p_data2)
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
    // remove the edge from the vertex
    if (0 == set_remove(p_vertex1->p_adjacent, p_data2)){
        p_graph->ecount--;
        return 0; 
    }
    return -1;
}

/*
 * @brief gets the adjacent vertices of a vertex
 * @param p_graph the graph the vertex resides in
 * @param p_data the data the vertex holds to get the adjacent vertices
 * @return an array of adjacent vertices or NULL if no adjacent
 *  vertices exist
 */
vertex * graph_vertex(graph * p_graph, void * p_data)
{
    // cant search in a NULL graph, a graph with no edges or for NULL vertices
    if ((NULL == p_graph) || (0 == p_graph->ecount) || (NULL == p_data)){
        return NULL; 
    }
    // get the vertex that contains data1 
    vertex * p_vertex = graph_search(p_graph, p_data);
    // if the data is not in the graph then p_vertex will be NULL
    return p_vertex;

}

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

/*
 * @brief get the list of vertices from a graph
 * @param p_graph the graph to get the list of vertices
 * @return the list of vertices in a graph or NULL on error
 */
list * graph_vertices(graph *p_graph)
{
    // cant get the vertices list from a NULL graph
    if (NULL == p_graph){
        return NULL; 
    }   
    return p_graph->p_vertices;
}

/*
 * @brief gets the data in a graph vertex
 * @param p_vertex the vertex to get the data from
 * @return pointer to the data in the vertex or NULL on error
 */
void * graph_data(vertex * p_vertex)
{
    // cant get the data from a NULL vertex
    if (NULL == p_vertex){
        return NULL;
    }
    return p_vertex->p_data;
}

/*
 * @brief prints all the vertices data in a graph
 */
void graph_print(graph * p_graph)
{
    list_elem * p_elem = NULL;
    for (p_elem = list_head(p_graph->p_vertices); p_elem != NULL; p_elem = list_next(p_elem)){
        printf("%s\n", (char *)((vertex *)list_data(p_elem))->p_data);
    }
}

/*
 * @brief perform a breadth first search on a graph
 * @param p_graph the graph to perform the search on
 * @param p_start the data for the starting vertex
 * @param p_end the data for the end vertex
 * @return a list with the shortest path from start to end or NULL
 *  on error or if there is no path form start to end
 */
list * graph_bfs(graph * p_graph, void * p_start, void * p_end)
{
    // cant do a bfs in a NULL or empty graph or from NULL data
    if ((NULL == p_graph) || (0 == p_graph->vcount) || (NULL == p_start) || (NULL == p_end)){
        return NULL;
    }
    // create a queue to hold the found vertexes
    queue * p_queue = queue_init(NULL, NULL);
    if (NULL == p_queue){
        return NULL;
    }
    // get the starting vertex and setup the queue
    vertex * p_svertex = graph_search(p_graph, p_start);
    if (NULL == p_svertex){
        return NULL;
    }
    // get the ending vertex
    vertex * p_evertex = graph_search(p_graph, p_end);
    if (NULL == p_evertex){
        return NULL;
    }
    // the starting vertex color changes to grey and hops to 0
    p_svertex->hops = 0;
    p_svertex->color = GREY;
    // add the starting vertex to the queue
    queue_enqueue(p_queue, p_svertex);
    vertex * p_vertex = NULL;
    vertex * p_adj_vertex = NULL;
    list_elem * p_elem = NULL;
    // while the queue is not empty conduct search
    while (queue_size(p_queue)){
        // for each element in the queue visit adjacent vertices 
        p_vertex = queue_peek(p_queue);
        for (p_elem = set_head(p_vertex->p_adjacent);\
             p_elem != NULL; p_elem = list_next(p_elem)){
             p_adj_vertex = graph_search(p_graph, (char *)list_data(p_elem));
             // check the color of the adjacent vertex
             if (WHITE == p_adj_vertex->color){
                p_adj_vertex->color = GREY;
                p_adj_vertex->hops = p_vertex->hops + 1;
                // enqueue the adjacent vertex
                queue_enqueue(p_queue, p_adj_vertex);
             }
        }
        // color the vertex at the front of the queue black and dequeue the vertex
        p_vertex->color = BLACK;
        queue_dequeue(p_queue);
    }
    // destroy the queue
    queue_destroy(p_queue);
    // pass back all the vertex hop count in a list
    list * p_hops = list_init(NULL, p_graph->compare);
    for (p_elem = list_head(p_graph->p_vertices); NULL != p_elem; p_elem = list_next(p_elem)){
        p_vertex = (vertex *)list_data(p_elem);
        if (-1 != p_vertex->hops){
            list_ins_next(p_hops, list_tail(p_hops), p_vertex);
        }
    }
    return p_hops;
}

int16_t graph_hops(vertex * p_vertex)
{
    return p_vertex->hops;
}

// gets the shortes path but has memory leaks
list * graph_path(graph * p_graph, vertex * p_start, vertex * p_end)
{
    // initialize all the colors
    list_elem * p_temp_elem = list_head(p_graph->p_vertices);
    for (; NULL != p_temp_elem; p_temp_elem = list_next(p_temp_elem)){
        vertex * p_temp_vertex = list_data(p_temp_elem); 
        p_temp_vertex->color = WHITE;
    }
    // create a path that is a list of vectors
    list * p_path = list_init(p_graph->destroy, p_graph->compare);
    list_ins_next(p_path, list_tail(p_path), p_start);
    p_start->color = GREY;
    // queue will store paths
    queue * p_queue = queue_init(NULL, NULL);
    // add the first list to the queue
    queue_enqueue(p_queue, p_path); 
    // until the queue is empty continue to add to path 
    while (0 != queue_size(p_queue)){
        // get the path from the front of the queue
        list * p_cur_path = queue_peek(p_queue);   
        // check if last node of the current path is the destination
        vertex * p_cur_vertex = list_data(list_tail(p_cur_path));
        if (p_cur_vertex == p_end){
            queue_destroy(p_queue);
            return p_cur_path;
        }
        else {
            // for each adjacent vertex in the current vertex
            // if the adjacent vertex is not in the current path
            // copy previous path append adjacent vertex and add to queue
            list_elem * p_adj_elem = NULL;
            vertex * p_adj_vertex = NULL;
            for (p_adj_elem = set_head(p_cur_vertex->p_adjacent); NULL != p_adj_elem; \
                                                 p_adj_elem = list_next(p_adj_elem)){
                p_adj_vertex = graph_search(p_graph, (char *)list_data(p_adj_elem));
                if ((NULL == list_search(p_cur_path, p_adj_vertex))){
                    if (WHITE == p_adj_vertex->color){
                        p_adj_vertex->color = GREY;
                        // create new path
                        list * p_new_path = list_init(NULL, p_graph->compare); 
                        // copy old path
                        list_elem * p_elem = list_head(p_cur_path);
                        for (;NULL != p_elem; p_elem = list_next(p_elem)){
                            list_ins_next(p_new_path, list_tail(p_new_path), list_data(p_elem));
                        }
                        // append adj_vertex to the new path
                        list_ins_next(p_new_path, list_tail(p_new_path), p_adj_vertex);
                        // insert the new path into the queue
                        queue_enqueue(p_queue, p_new_path);
                    }
                }
            }
            p_cur_vertex->color = BLACK;
            queue_dequeue(p_queue);
        }
    }
    return NULL;
}

