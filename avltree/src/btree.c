#include <btree.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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
    btnode ** pp_position = &p_tree->p_root;
    *pp_position = p_node;
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
    if (NULL == p_tree){
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

    if (NULL != *pp_position){
        btree_rm_left(p_tree, *pp_position);
        btree_rm_right(p_tree, *pp_position);
        // run user defined function if it exists
        if (NULL != p_tree->destroy){
            p_tree->destroy((*pp_position)->p_data);
        }
        // decrement tree size
        free(*pp_position);
        *pp_position = NULL;
        p_tree->size--;
    }
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
    if (NULL != *pp_position){
        btree_rm_left(p_tree, *pp_position);
        btree_rm_right(p_tree, *pp_position);
        if (NULL != p_tree->destroy){
            p_tree->destroy((*pp_position)->p_data);
        }
        free(*pp_position);
        *pp_position = NULL;
        p_tree->size--;
    }
}

/*
 * @brief inserts a new binary tree node into a tree as a left child
 * @param p_tree tree to insert a new node into
 * @param p_node parent of the new left node
 * @param p_data data for the new node
 * @return 0 on successful insertion -1 on failure
 */
btnode *  btree_ins_left(btree * p_tree, btnode * p_node, void * p_data)
{
    // prevent inserting into an null or empty tree
    // also prevent inserting null data or at the root
    if ((NULL == p_tree) || (0 == p_tree->size) || (NULL == p_node)){
        return NULL;
    }
    btnode ** pp_position = &p_node->p_left;
    // allocate the new node
    btnode * p_new_node = calloc(1, sizeof(*p_new_node));
    if (NULL == p_new_node){
        return NULL;
    }
    // assign values to new node
    p_new_node->p_data = p_data;
    p_new_node->p_left = NULL;
    p_new_node->p_right = NULL;
    *pp_position = p_new_node;
    // increase the tree size
    p_tree->size++;
    return p_new_node;
}

/*
 * @brief inserts a new binary tree node into a tree as a right child
 * @param p_tree tree to insert a new node into
 * @param p_node parent of the new right node
 * @param p_data data for the new node
 * @return 0 on successful insertion -1 on failure
 */
btnode * btree_ins_right(btree * p_tree, btnode * p_node, void * p_data)
{
    // check if the tree node or data is NULL
    // and also do not allow insertion into a tree of size 0
    if ((NULL == p_tree) || (0 == p_tree->size) || (NULL == p_node) || (NULL == p_data)){
        return NULL;
    }
    btnode ** pp_position = &p_node->p_right;
    // allocate a new node
    btnode * p_new_node = calloc(1, sizeof(*p_new_node));
    if (NULL == p_new_node){
        return NULL;
    }
    // assign values to new node
    p_new_node->p_data = p_data;
    p_new_node->p_left = NULL;
    p_new_node->p_right = NULL;
    *pp_position = p_new_node;
    // increase the tree size
    p_tree->size++;
    return p_new_node;
}

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

int8_t btree_compare(btree * p_tree, void * key1, void * key2)
{
    // ensure nor tree or tree->compare is null
    if ((NULL == p_tree) || (NULL == p_tree->compare)){
        return -2;
    }
    return p_tree->compare(key1, key2);
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
 * @param p_node the parent node for the left child
 * @return pointer to the left child node
 */
btnode * btree_left(btnode * p_node)
{
    // ensure the node is not null
    if (NULL == p_node){
        return NULL;
    }
    return p_node->p_left;
}

/*
 * @brief gets the right child of a node 
 * @param p_node the parent node for the right child
 * @return pointer to the right child node
 */
btnode * btree_right(btnode * p_node)
{
    // ensure the node is not null
    if (NULL == p_node){
        return NULL;
    }
    return p_node->p_right;
}

/*
 * @brief gets the data in a node
 * @param p_node the node the data belongs to
 * @return pointer to the nodes data
 */
void * btree_data(btnode * p_node)
{
    // ensure the node is not null
    if (NULL == p_node){
        return NULL;
    }
    return p_node->p_data;
}

/*
 * @brief determines if the node is at the end of a branch
 * @param p_node node to evaluate
 * @return true if node is end of branch else false
 */
bool btree_is_eob(btnode * p_node)
{
    return (NULL == p_node);
}

/*
 * @brief determines if the node is a leaf node
 * @param p_node node to evaluate
 * @return true if node is a leaf node else false
 */
bool btree_is_leaf(btnode * p_node)
{
    // if p_node is null then it is not a leaf
    if (NULL == p_node){
        return false;
    }
    return ((NULL == p_node->p_left) && (NULL == p_node->p_right));
}

// setters

void btree_set_left(btnode * p_parent, btnode * p_child)
{
    // ensure neither node is null
    if ((NULL == p_parent) || (NULL == p_child)){
        return;
    }
    p_parent->p_left = p_child;
}

void btree_set_right(btnode * p_parent, btnode * p_child)
{
    // ensure neither node is null
    if ((NULL == p_parent) || (NULL == p_child)){
        return;
    }
    p_parent->p_left = p_child;
}

void btree_size_decrease(btree * p_tree)
{
    // do not decrease a null or empty tree
    if ((NULL == p_tree) || (0 == p_tree->size)){
        return;
    }
    p_tree->size--; 
}
