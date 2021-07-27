#include <avltree.h>
#include <btree.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct btree avltree_t;
typedef struct avltree_node {
    void * p_data;
    bool hidden;
    int factor;
} avltree_node;

/*
 * @brief creates and initializes a avltree
 * @param p_data the data for the root node of the tree
 * @param destroy user defined destroy function for the tree
 * @param compare user defined function to compare the data in the tree
 * @return a pointer to the newly created avltree
 */
avltree * avltree_init(void * p_data, void (* destroy)(void * p_data), int (* compare)(void * key1, void * key2))
{
    // do not allow creating a tree without a data for its root node
    if (NULL == p_data){
        return;
    }
    // create the root node
    avlnode * p_root = calloc(1, sizeof(*avlnode));
    if (NULL == p_root){
        return NULL;
    }
    p_root->p_data = p_data;
    p_root->hidden = false;
    p_root->factor = BALANCED;
    // create the avltree and return
    avltree * p_tree = btree_init(p_root, destroy, compare);
    if (NULL == avltree){
        return NULL;
    } 
    return avltree;
}
void avltree_destroy(avltree * p_tree);
avltree_node * avltree_insert(avltree * p_tree, void * p_data);
int8_t avltree_remove(avltree * p_tree, void * p_data);
avltree_node * avltree_find(avltree * p_tree, void * p_data);
int64_t avltree_size(avltree * p_tree);
void * avltree_data(avltree * p_tree, avlnode * p_node);
