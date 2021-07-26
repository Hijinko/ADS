#include <btree.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * @breif a binary tree node structure
 * @param p_data the data that the node holds
 * @param p_left the left child node of the parent
 * @param p_right the right child node of the parent
 */
typedef struct btree_node_t {
    void * p_data;
    struct btree_node_t * p_left;
    struct btree_node_t * p_right;
} btree_node_t;

/*
 * @brief a binary tree structure
 * @param size the number of nodes in the tree
 * @param p_root the root node of the tree
 */
typedef struct btree_t {
    int64_t size;
    btnode * p_root;
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
 * @return a pointer to a newly malloced tree on success
 *  NULL on failure
 */
btree * btree_init(void * p_data, void (* destroy)(void * data), int8_t (*compare)(void * key1, void * key2))
{
    // don't allow a new tree with null data to be created
    if (NULL == p_data){
        return NULL;
    }
    // create tree and test for valid allocation
    btree * p_tree = calloc(1, sizeof(*p_tree));
    if (NULL == p_tree){
        return NULL;
    }
    // create the new root node and check for valid allocation
    btnode * p_node = calloc(1, sizeof(*p_node));
    if (NULL == p_node){
        return NULL;
    }
    // set the values for the node
    p_node->p_data = p_data;
    p_node->p_left = NULL;
    p_node->p_right = NULL;
    // set the values for the tree
    p_tree->size = 1;
    p_tree->p_root = p_node;
    p_tree->destroy = destroy;
    p_tree->compare = compare;
    // return pointer to the tree
    return p_tree;
}

/*
 * @brief tears down a binary tree
 * @param p_tree pointer to binary tree to tear down
 */
void btree_destroy(btree * p_tree)
{
    // do not delete from a NULL or empty tree
    if ((NULL == p_tree) || (0 == p_tree->size)){
        return;
    }
    btree_rm_left(p_tree, NULL);
    free(p_tree);
}

/*
 * @brief removes the left child of a given tree
 * @param p_tree the tree to remove the child from
 * @param p_node the node to remove the left nodes from
 *  if set to NULL then the entire tree is deleted
 */
void btree_rm_left(btree * p_tree, btnode * p_node)
{
    // do not allow removal from empty tree
    if ((NULL == p_tree) || (0 == p_tree->size)){
        return;        
    }
    // position will hold the position of the node to delete
    btnode ** pp_position;
    // if node is null than the root is being removed
    if (NULL == p_node){
        pp_position = &p_tree->p_root;
    }
    else {
        pp_position = &p_node->p_left;
    }
    // run user defined function if it exists
    if (NULL != p_tree->destroy){
        btree_postorder(p_tree, *pp_position, p_tree->destroy);
    }
    else {
        btree_postorder(p_tree, *pp_position, free);
    }
    free(*pp_position);
    // decrement tree size
    p_tree->size--;
}

/*
 * @brief removes the right child of a given tree
 * @param p_tree the tree to remove the child from
 * @param p_node the node to remove the right nodes from
 *  if set to NULL then the entire tree is deleted
 */
void btree_rm_right(btree * p_tree, btnode * p_node)
{
    // do not allow removing from empty tree
    if ((NULL == p_tree) || (0 == p_tree->size)){
        return;
    }
    btnode ** pp_position;
    // if node is null than the root is being removed
    if (NULL == p_node){
        pp_position = &p_tree->p_root;
    }
    else {
        pp_position = &p_node->p_right;
    }
    // run user defined destroy function
    if (NULL != p_tree->destroy){
        btree_postorder(p_tree, *pp_position, p_tree->destroy);
    }
    else {
        btree_postorder(p_tree, *pp_position, free);
    }
    free(*pp_position);
    // decrement tree size
    p_tree->size--;
}

/*
 * @brief inserts a new binary tree node into a tree as a left child
 * @param p_tree tree to insert a new node into
 * @param p_node parent of the new left node
 * @param p_data data for the new node
 * @return 0 on successful insertion -1 on failure
 */
btnode *  btree_ins_left(btree * p_tree, btnode * p_node, void * p_data);

/*
 * @brief inserts a new binary tree node into a tree as a right child
 * @param p_tree tree to insert a new node into
 * @param p_node parent of the new right node
 * @param p_data data for the new node
 * @return 0 on successful insertion -1 on failure
 */

btnode * btree_ins_right(btree * p_tree, btnode * p_node, void * p_data);

/*
 * @brief traverses a tree in postorder and runs the provided function on
 *  All the nodes
 * @param p_tree tree to traverse
 * @param func user defined function to run
 */
void btree_postorder(btree * p_tree, btnode * p_node, void (* func)(void * data))
{
    // do not iterate over null or empty tree
    if ((NULL == p_tree) || (0 == p_tree->size) || (NULL == p_node)){
        return;
    }
    // iterate through
    btree_postorder(p_tree, p_node->p_left, func);
    btree_postorder(p_tree, p_node->p_right, func);
    func(p_node);
}

/*
 * @brief traverses a tree in preorder and runs the provided function on
 *  All the nodes
 * @param p_tree tree to traverse
 * @param func user defined function to run
 */
void btree_preorder(btree * p_tree, btnode * p_node, void (* func)(void * data))
{
    // do not iterate over a null or empty tree
    if ((NULL == p_tree) || (0 == p_tree->size) || (NULL == p_node)){
        return;
    }
    func(p_node);
    btree_preorder(p_tree, p_node->p_left, func);
    btree_preorder(p_tree, p_node->p_right, func);
}

/*
 * @brief traverses a tree in inotorder and runs the provided function on
 *  All the nodes
 * @param p_tree tree to traverse
 * @param func user defined function to run
 */
void btree_inorder(btree * p_tree, btnode * p_node, void (* func)(void * data))
{
    // do not iterate over a null or empty tree
    if ((NULL == p_tree) || (0 == p_tree->size) || (NULL == p_node)){
        return;
    }
    btree_inorder(p_tree, p_node->p_left, func);
    func(p_node);
    btree_inorder(p_tree, p_node->p_right, func);
}

// getters

/*
 * @brief gets the number of nodes in a tree
 * @param  p_tree the tree to get the size from
 * @return the size of the tree or -1 on error
 */
int64_t btree_size(btree * p_tree)
{
    // ensure the tree is not null
    if (NULL == p_tree){
        return -1;
    }
    return p_tree->size;
}

/*
 * @brief gets the root node of a binary tree 
 * @param  p_tree the tree to get the root node from
 * @return pointer to the root node in the tree
 */
btnode * btree_root(btree * p_tree)
{
    // ensure the tree is not null
    if (NULL == p_tree){
        return NULL;
    }
    return p_tree->p_root;

}

/*
 * @brief gets the left child of a node 
 * @param  p_tree the tree the node belongs to
 * @param p_node the parent node for the left child
 * @return pointer to the left child node
 */
btnode * btree_left(btree * p_tree, btnode * p_node)
{
    // ensure the tree is not null
    if ((NULL == p_tree) || (NULL == p_node)){
        return NULL;
    }
    return p_node->p_left;
}

/*
 * @brief gets the right child of a node 
 * @param  p_tree the tree the node belongs to
 * @param p_node the parent node for the right child
 * @return pointer to the right child node
 */
btnode * btree_right(btree * p_tree, btnode * p_node)
{
    // ensure the tree is not null
    if ((NULL == p_tree) || (NULL == p_node)){
        return NULL;
    }
    return p_node->p_right;
}

/*
 * @brief gets the data in a node
 * @param  p_tree  the tree the data belongs to
 * @param p_node the node the data belongs to
 * @return pointer to the nodes data
 */
void * btree_data(btree * p_tree, btnode * p_node)
{
    // ensure the tree is not null
    if ((NULL == p_tree) || (NULL == p_node)){
        return NULL;
    }
    return p_node->p_data;
}
