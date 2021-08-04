#include <graph.h>
#include <stdint.h>
#include <list.h>

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
    list ** pp_vertices;
};

/*
 * @brief allocates a graph structure and initializes the graphs values 
 * @param destroy user defined destroy function for the data in the vertices
 * @param compare user defined compare function for the data in the vertices
 * @return pointer to the newly allocated graph or NULL on error
 */
graph * graph_init(void (* destory)(void * p_data), int8_t (* compare)(void * p_key1, void * p_key2));
void graph_destroy(graph * p_graph);
vertex * graph_ins_vertex(graph * p_graph, void * p_data);
int8_t graph_ins_edge(graph * p_grap, vertex * p_vertex1, vertex * p_vertex2);
int8_t graph_rm_vertex(graph * p_graph, void * p_data); 
int8_t graph_rm_edge(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
vertex ** graph_adjlist(graph * p_grap, vertex * p_vertex);
int8_t graph_is_adjacent(graph * p_graph, vertex * p_vertex1, vertex * p_vertex2);
int64_t graph_vcount(graph * p_graph);
int64_t graph_ecount(graph * p_graph);
