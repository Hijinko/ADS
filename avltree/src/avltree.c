#include <avltree.h>
#include <btree.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * @brief avltree node structure
 * @param p_data data for the node
 * @param b_hidden bool value to note if node should be hidden or not
 * @factor the balance factor of the node
 */
typedef struct avltree_node {
    void * p_data;
    bool b_hidden;
    int factor;
} avltree_node;

static void avltree_rotate_left(btnode ** p_node)
{
    btnode * p_left = btree_left(*p_node);
    btnode * p_grandchild = NULL;
    // check if left node is left heavy and perform left left rotation
    if (((avlnode *)btree_data(p_left))->factor == LEFT_HEAVY){
        btree_set_left(*p_node, btree_right(p_left));
        btree_set_right(p_left, *p_node);
        // reset the balance levels of the after the rotation
        ((avlnode *)btree_data(*p_node))->factor = BALANCED;
        ((avlnode *)btree_data(p_left))->factor = BALANCED;
        *p_node = p_left;
    }
    else {
        // else node is right heavy so perform a left right rotation
        p_grandchild = btree_right(p_left);
        btree_set_right(p_left, btree_left(p_grandchild));
        btree_set_left(p_grandchild, p_left);
        btree_set_left(*p_node, btree_right(p_grandchild));
        btree_set_right(p_grandchild, *p_node);
        // set the new balance factors based on current values
        switch (((avlnode *)btree_data(p_grandchild))->factor){
            case LEFT_HEAVY:
            ((avlnode *)btree_data(*p_node))->factor = RIGHT_HEAVY;
            ((avlnode *)btree_data(p_left))->factor = BALANCED;
            break;
            case BALANCED:
            ((avlnode *)btree_data(*p_node))->factor = BALANCED;
            ((avlnode *)btree_data(p_left))->factor = BALANCED;
            break;
            case RIGHT_HEAVY:
            ((avlnode *)btree_data(*p_node))->factor = BALANCED;
            ((avlnode *)btree_data(p_left))->factor = LEFT_HEAVY;
            break;
        }
        ((avlnode *)btree_data(p_grandchild))->factor = BALANCED;
        *p_node = p_grandchild;
    }
}

static void avltree_rotate_right(btnode ** p_node)
{
    btnode * p_right = btree_right(*p_node);
    btnode * p_grandchild = NULL;
    // check if right node is right heavy and perform right right rotation
    if (((avlnode *)btree_data(p_right))->factor == RIGHT_HEAVY){
        btree_set_right(*p_node, btree_left(p_right));
        btree_set_left(p_right, *p_node);
        // reset the balance levels of the after the rotation
        ((avlnode *)btree_data(*p_node))->factor = BALANCED;
        ((avlnode *)btree_data(p_right))->factor = BALANCED;
        *p_node = p_right;
    }
    else {
        // else node is left heavy so perform a right left rotation
        p_grandchild = btree_left(p_right);
        btree_set_left(p_right, btree_right(p_grandchild));
        btree_set_right(p_grandchild, p_right);
        btree_set_right(*p_node, btree_left(p_grandchild));
        btree_set_left(p_grandchild, *p_node);
        // set the new balance factors based on current values
        switch (((avlnode *)btree_data(p_grandchild))->factor){
            case LEFT_HEAVY:
            ((avlnode *)btree_data(*p_node))->factor = BALANCED;
            ((avlnode *)btree_data(p_right))->factor = RIGHT_HEAVY; 
            break;
            case BALANCED:
            ((avlnode *)btree_data(*p_node))->factor = BALANCED;
            ((avlnode *)btree_data(p_right))->factor = BALANCED;
            break;
            case RIGHT_HEAVY:
            ((avlnode *)btree_data(*p_node))->factor = LEFT_HEAVY;
            ((avlnode *)btree_data(p_right))->factor = BALANCED;
            break;
        }
        ((avlnode *)btree_data(p_grandchild))->factor = BALANCED;
        *p_node = p_grandchild;
    }
}

static void avltree_rm_left(avltree * p_tree, btnode * p_node)
{
    // do not allow deletion from null or empty tree
    if ((NULL == p_tree) || (0 == btree_size(p_tree))){
        return;
    }
    btree_rm_left(p_tree, p_node);
}

static void avltree_rm_right(avltree * p_tree, btnode * p_node)
{
    // do not allow deletion from null or empty tree
    if ((NULL == p_tree) || (0 == btree_size(p_tree))){
        return;
    }
    btree_rm_right(p_tree, p_node);
}

static int insert(avltree * p_tree, btnode ** pp_node, void * p_data, int * balanced){
    avlnode *p_avl_data = NULL;
    int cmpval = 0;
    int retval = 0;
    // insert data into the tree
    if (btree_is_eob(*pp_node)){
        // insert into empty tree
        p_avl_data = calloc(1, sizeof(*p_avl_data));
        if (NULL == p_data){
            return -1;
        }
        p_avl_data->p_data = p_data;
        p_avl_data->b_hidden = 0;
        p_avl_data->factor = BALANCED;
        return (NULL == btree_ins_left(p_tree, *pp_node, p_avl_data)) ? -1 : 0 ;
    }
    else {
        // insert into a tree that is not empty
        cmpval = btree_compare(p_tree, p_data, ((avlnode *)btree_data(*pp_node))->p_data);
        if (cmpval < 0){
            // move to the left
            if (btree_is_eob(btree_left(*pp_node))){
                p_avl_data = calloc(1, sizeof(*p_avl_data));
                if (NULL == p_avl_data){
                    return -1;
                }
                p_avl_data->p_data = p_data;
                p_avl_data->b_hidden = 0;
                p_avl_data->factor = BALANCED;
                return (NULL == btree_ins_left(p_tree, *pp_node, p_avl_data)) ? -1 : 0 ;
                *balanced = 0;
            }
            else {
                btnode * p_left = btree_left(*pp_node);
                if ((retval = (insert(p_tree, &p_left, p_data, balanced))) != 0){
                    return retval;
                }
            }
            // keep the tree balanced
            if (!(*balanced)){
                switch (((avlnode *)btree_data(*pp_node))->factor){
                    case LEFT_HEAVY:
                        avltree_rotate_left(pp_node);
                        *balanced = 1;
                    break;
                    case BALANCED:
                        ((avlnode *)btree_data(*pp_node))->factor = LEFT_HEAVY;
                    break;
                    case RIGHT_HEAVY:
                        ((avlnode *)btree_data(*pp_node))->factor = LEFT_HEAVY;
                        *balanced = 1;
                    break;
                }
            }
        }
        else if (cmpval > 0){
            // move to the right
            if (btree_is_eob(btree_right(*pp_node))){
                p_avl_data = calloc(1, sizeof(*p_avl_data));
                if (NULL == p_avl_data){
                    return -1;
                }
                p_avl_data->p_data = p_data;
                p_avl_data->b_hidden = 0;
                p_avl_data->factor = BALANCED;
                return (NULL == btree_ins_right(p_tree, *pp_node, p_avl_data)) ? -1 : 0 ;
                *balanced = 0;
            }
            else {
                btnode * p_right = btree_right(*pp_node);
                if ((retval = (insert(p_tree, &p_right, p_data, balanced))) != 0){
                    return retval;
                }
            }
            // keep the tree balanced
            if (!(*balanced)){
                switch (((avlnode *)btree_data(*pp_node))->factor){
                    case LEFT_HEAVY:
                        ((avlnode *)btree_data(*pp_node))->factor = BALANCED;
                        *balanced = 1;
                    break;
                    case BALANCED:
                        ((avlnode *)btree_data(*pp_node))->factor = RIGHT_HEAVY;
                    break;
                    case RIGHT_HEAVY:
                        avltree_rotate_right(pp_node);
                        *balanced = 1;
                    break;
                }
            }
        }
        else {
            // handle copy of data found
            if (!((avlnode *)btree_data(*pp_node))->b_hidden){
                // the data is already in the tree and not hidden
                return -1;
            }
            else {
                // insert the new data and label it not hidden
                btree_destroy(((avlnode *)btree_data(*pp_node))->p_data);
                ((avlnode *)btree_data(*pp_node))->p_data = p_data;
                ((avlnode *)btree_data(*pp_node))->b_hidden = 0;
                // no rebalance needed
                *balanced = 1;
            }
        }
    }
        return 0;
}

static int hide(avltree * p_tree, btnode * p_node, void * p_data)
{
    int cmpval = 0;
    int retval = 0;
    // data cant be found if node is end of branch
    if (btree_is_eob(p_node)){
        return -1;
    }
    cmpval = btree_compare(p_tree, p_data, ((avlnode *)btree_data(p_node))->p_data);
    // move to the lef
    if (cmpval < 0){
        retval = hide(p_tree, btree_left(p_node), p_data);
    }
    else if (cmpval > 0){
        // move to the right
        retval = hide(p_tree, btree_right(p_node), p_data);
    }
    else {
        // the node is hidden
        ((avlnode *)btree_data(p_node))->b_hidden = true;
        retval = 0;
    }
    return retval;
}

static int lookup(avltree * p_tree, btnode * p_node, void ** pp_data)
{
    int cmpval = 0;
    int retval = 0;
    // data cant be found in a node that is end of branch
    if (btree_is_eob(p_node)){
        return -1;
    }
    cmpval = btree_compare(p_tree, *pp_data, ((avlnode *)btree_data(p_node))->p_data);
    if (cmpval < 0){
        // move to the left
        retval = lookup(p_tree, btree_left(p_node), pp_data);
    }
    else if (cmpval > 0){
        // move to the right
        retval = lookup(p_tree, btree_right(p_node), pp_data);
    }
    else {
        if (!(((avlnode *)btree_data(p_node))->b_hidden)){
            // return data from the tree;
            *pp_data = ((avlnode *)btree_data(p_node))->p_data;
            retval = 0;
        }
        else {
            // data was not found
            return -1;
        }
    }   
    return retval;
}

/*
 * @brief creates and initializes a avltree
 * @param p_data the data for the root node of the tree
 * @param destroy user defined destroy function for the tree
 * @param compare user defined function to compare the data in the tree
 * @return a pointer to the newly created avltree
 */
avltree * avltree_init(void * p_data, void (* destroy)(void * p_data), int8_t (* compare)(void * key1, void * key2))
{
    // do not allow creating a tree without a data for its root node
    if (NULL == p_data){
        return NULL;
    }
    // create the root node
    avlnode * p_root = calloc(1, sizeof(*p_root));
    if (NULL == p_root){
        return NULL;
    }
    p_root->p_data = p_data;
    p_root->b_hidden = false;
    p_root->factor = BALANCED;
    // create the avltree and return
    avltree * p_tree = btree_init(p_root, destroy, compare);
    if (NULL == p_tree){
        return NULL;
    } 
    return p_tree;
}

/*
 * @brief tears down an avltree
 * @param p_tree avltree to tear down
 */
void avltree_destroy(avltree * p_tree)
{
    // only tear down if p_tree is not null
    if (NULL != p_tree){
        btree_destroy(p_tree);
    }
}

avltree_node * avltree_insert(avltree * p_tree, void * p_data);
int8_t avltree_remove(avltree * p_tree, void * p_data);
avltree_node * avltree_find(avltree * p_tree, void * p_data);
int64_t avltree_size(avltree * p_tree);
void * avltree_data(avltree * p_tree, avlnode * p_node);
