#ifndef _AVLTREE_H
#define _AVL_TREE_H
#include <btree.h>
#include <stdint.h>
//typedef struct avltree_t avltree;
typedef struct btree_t avltree;
typedef struct avltree_node avlnode;

avltree * avltree_init(void * p_data, void (* destroy)(void * p_data), int8_t (* compare)(void * key1, void * key2));
void avltree_destroy(avltree * p_tree);
int avltree_insert(avltree * p_tree, void * p_data);
int8_t avltree_remove(avltree * p_tree, void * p_data);
avlnode * avltree_find(avltree * p_tree, void * p_data);
int64_t avltree_size(avltree * p_tree);
void * avltree_data(avlnode * p_node);
#define LEFT_HEAVY 1
#define BALANCED 0
#define RIGHT_HEAVY -1
#endif
