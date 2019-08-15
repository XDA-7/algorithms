#ifndef AVL_TREE
#define AVL_TREE

#include "binary_tree.h"

int height[ARR_SIZE];

void avl_tree_create(int root_key);
int avl_tree_insert(int insert_key);
void avl_tree_remove(int remove_key);

void avl_tree_left_rotate(int node_index);
void avl_tree_right_rotate(int node_index);

int verify_height_integrity();
void print_heights();

#endif