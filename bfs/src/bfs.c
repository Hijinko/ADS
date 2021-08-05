#include <bfs.h>
#include <list.h>
#include <graph.h>
#include <stdint.h>

/*
 * @brief a breadth first search vertex structure
 * @param p_data the data in vertex
 * @param color the current color to mark the vertex during searching
 * @param hops the number of hops it takes to get to the vertex
 */
struct bfs_vertex {
    void * p_data;
    VertexColor color;
    int32_t hops;
};

/*
 * @brief performs a breadth first search on the provided graph
 * @param p_graph the graph to do the breadth first search on
 * @param p_start the starting vertex to perform the bfs
 * @param hops the list of hops it takes to get to each vertex
 * @return 0 on successful breadth first search else -1
 */
int8_t bfs(graph * p_graph, bfsvertex * start, list * hops);
