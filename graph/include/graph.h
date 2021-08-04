#ifndef _GRAPH_H
#define _GRAPH_H
#include <list.h>
#include <stdint.h>
typedef struct graph graph;
typedef struct list_elem vertex;
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
#endif
