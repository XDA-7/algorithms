#include "avl_tree.h"
#include "stdio.h"

int recalculate_height(int index) {
    if (height[left[index]] > height[right[index]]) {
        return height[left[index]] + 1;
    }
    else {
        return height[right[index]] + 1;
    }
}

void avl_tree_left_rotate(int node_index) {
    tree_left_rotate(node_index);
    height[node_index] = recalculate_height(node_index);
    height[parent[node_index]] = recalculate_height(parent[node_index]);
}

void avl_tree_right_rotate(int node_index) {
    tree_right_rotate(node_index);
    height[node_index] = recalculate_height(node_index);
    height[parent[node_index]] = recalculate_height(parent[node_index]);
}

int balance_left_heavy(int subtree_index) {
    int left_subtree = left[subtree_index];
    if (height[right[left_subtree]] == height[left[left_subtree]] + 1) {
        avl_tree_left_rotate(left_subtree);
        left_subtree = parent[left_subtree];
    }

    avl_tree_right_rotate(subtree_index);
    return parent[subtree_index];
}

int balance_right_heavy(int subtree_index) {
    int right_subtree = right[subtree_index];
    if (height[left[right_subtree]] == height[right[right_subtree]] + 1) {
        avl_tree_right_rotate(right_subtree);
        right_subtree = parent[right_subtree];
    }

    avl_tree_left_rotate(subtree_index);
    return parent[subtree_index];
}

void avl_tree_create(int root_key) {
    create_tree(root_key);
    height[NIL] = 0;
    height[root] = 1;
    for (int i = 2; i < ARR_SIZE; i++) {
        height[i] = 0;
    }
}

int avl_tree_insert(int insert_key) {
    int insert_index = tree_insert(insert_key);
    if (height[insert_index] != 0) {
        return insert_index;
    }

    height[insert_index] = 1;
    int ancestor_index = insert_index;
    while (ancestor_index != NIL && height[ancestor_index] == height[parent[ancestor_index]]) {
        ancestor_index = parent[ancestor_index];
        height[ancestor_index] = recalculate_height(ancestor_index);
        if (height[left[ancestor_index]] + 2 == height[right[ancestor_index]]) {
            ancestor_index = balance_right_heavy(ancestor_index);
            while (ancestor_index != NIL) {
                height[ancestor_index] = recalculate_height(ancestor_index);
                ancestor_index = parent[ancestor_index];
            }
        }
        else if (height[right[ancestor_index]] + 2 == height[left[ancestor_index]]) {
            ancestor_index = balance_left_heavy(ancestor_index);
            while (ancestor_index != NIL) {
                height[ancestor_index] = recalculate_height(ancestor_index);
                ancestor_index = parent[ancestor_index];
            }
        }

    }

    return insert_index;
}

void avl_tree_remove(int remove_key) {}

int verify_height_integrity() {
    for (int i = 1; i <= tree_size; i++) {
        if (height[left[i]] > height[right[i]] + 1 || height[right[i]] > height[left[i]] + 1 || height[i] != recalculate_height(i)) {
            printf("i: %d, height: %d, left height: %d, right height: %d\n", i, height[i], height[left[i]], height[right[i]]);
            return i;
        }
    }

    return 0;
}

void print_heights() {
    for (int i = 0; i <= tree_size; i++) {
        printf("node: %d, height: %d\n", i, height[i]);
    }
}