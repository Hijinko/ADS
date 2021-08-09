#ifndef _BFS_H
#define _BFS_H
#include <list.h>
#include <graph.h>
#include <stdint.h>
typedef struct bfs_vertex bfsvertex;
int8_t bfs(graph * p_graph, bfsvertex * start, list * hops);
#endif
