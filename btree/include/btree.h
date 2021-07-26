#ifndef _BTREE_H
#define _BTREE_H
#include <stdint.h>

typedef struct btree_node_t btnode;
typedef struct btree_t btree;

btree * btree_init(void * p_data, void (* destroy)(void * data), int8_t (*compare)(void * key1, void * key2));
void btree_destroy(btree * p_tree);
void btree_rm_left(btree * p_tree, btnode * p_node);
void btree_rm_right(btree * p_tree, btnode * p_node);
int8_t btree_ins_left(btree * p_tree, btnode * p_node, void * p_data);
int8_t btree_ins_right(btree * p_tree, btnode * p_node, void * p_data);
void * btree_postorder(btree * p_tree, void (* func)(void * data));
void * btree_preorder(btree * p_tree, void (* func)(void * data));
void * btree_inorder(btree * p_tree, void (* func)(void * data));
// getters
int64_t btree_size(btree * p_tree);
btnode * btree_root(btree * p_tree);
btnode * btree_left(btree * p_tree, btnode * p_node);
btnode * btree_right(btree * p_tree, btnode * p_node);
void * btree_data(btnode * p_node);
#endif
