#include <btree.h>
#include <stdint.h>

/*
 * @breif a binary tree node structure
 * @param p_data the data that the node holds
 * @param p_left the left child node of the parent
 * @param p_right the right child node of the parent
 */
typedef struct btree_node_t {
    void * p_data;
    struct btree_t * p_left;
    struct btree_t * p_right;
} btree_node_t;

/*
 * @brief a binary tree structure
 * @param size the number of nodes in the tree
 * @param p_root the root node of the tree
 */
typedef struct btree_t {
    int64_t size;
    btnode * p_node;
    void (* destroy)(void * p_data);
    int8_t (* compare)(void * key1, void * key2);
} btree_t;

/*
 * @brief creates and initializes a binary tree
 * @param p_data the data of the root node
 * @param destroy user defined function to tear down the data
 * @param compare user defined function to compare nodes
 *  should have a return value of -1, 0, or 1 for 
 *  less than equal and greater than respectively
 */
btree * btree_init(void * p_data, void (* destroy)(void * data), int8_t (*compare)(void * key1, void * key2));
void * btree_destroy(btree * p_tree);
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
void * btree_data(btree * p_tree, btnode * p_node);
