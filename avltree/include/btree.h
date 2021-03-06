#ifndef _BTREE_H
#define _BTREE_H
#include <stdint.h>
#include <stdbool.h>
typedef struct btree_node_t btnode;
typedef struct btree_t btree;

btree * btree_init(void * p_data, void (* destroy)(void * data), int8_t (*compare)(void * key1, void * key2));
void btree_destroy(btree * p_tree);
void btree_rm_left(btree * p_tree, btnode * p_node);
void btree_rm_right(btree * p_tree, btnode * p_node);
btnode * btree_ins_left(btree * p_tree, btnode * p_node, void * p_data);
btnode * btree_ins_right(btree * p_tree, btnode * p_node, void * p_data);
void btree_postorder(btree * p_tree, btnode * p_node, void (* func)(void * data));
void btree_preorder(btree * p_tree, btnode * p_node, void (* func)(void * data));
void btree_inorder(btree * p_tree, btnode * p_node, void (* func)(void * data));
int8_t btree_compare(btree * p_tree, void * key1, void * key2);
// getters
int64_t btree_size(btree * p_tree);
btnode * btree_root(btree * p_tree);
btnode * btree_left(btnode * p_node);
btnode * btree_right(btnode * p_node);
bool btree_is_eob(btnode * p_node);
bool btree_is_leaf(btnode * p_node);
void * btree_data(btnode * p_node);
// setters
void btree_set_left(btnode * p_parent, btnode * p_child);
void btree_set_right(btnode * p_parent, btnode * p_child);
void btree_size_decrease(btree * p_tree);
#endif
