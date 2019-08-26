#include "stdlib.h"
#include "treap.h"

int priority[ARR_SIZE];

void treap_create(int root_key) {
    create_tree(root_key);
    priority[0] = 0;
    priority[1] = rand();
}

int treap_insert(int insert_key) {
    int insert_index = tree_insert(insert_key);
    if (priority[insert_index] == 0) {
        priority[insert_index] = rand();
    }

    while (priority[insert_index] < priority[parent[insert_index]]) {
        if (left[parent[insert_index]] == insert_index) {
            tree_right_rotate(parent[insert_index]);
        }
        else {
            tree_left_rotate(parent[insert_index]);
        }
    }

    return insert_index;
}