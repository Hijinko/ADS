#include <avltree.h>
#include <btree.h>
#include <stdint.h>
typedef struct btree avltree_t;
typedef struct avltree_node {
    void * p_data;
    bool hidden;
    int factor;
} avltree_node;

avltree * avltree_init(void * p_data, void (* destroy)(void * p_data), int (* compare)(void * key1, void * key2));
void avltree_destroy(avltree * p_tree);
avltree_node * avltree_insert(avltree * p_tree, void * p_data);
int8_t avltree_remove(avltree * p_tree, void * p_data);
avltree_node * avltree_find(avltree * p_tree, void * p_data);
int64_t avltree_size(avltree * p_tree);
void * avltree_data(avltree * p_tree, avlnode * p_node);
