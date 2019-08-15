#include "binary_tree.h"

void create_tree(int root_key) {
    root = 1;
    tree_size = 1;

    key[1] = root_key;
    parent[1] = NIL;
    left[1] = NIL;
    right[1] = NIL;
}

int tree_insert(int insert_key) {
    int current_index = root;
    int current_key = key[root];
    int parent_index = NIL;
    int *parent_child = 0;
    while (current_index != NIL && current_key != insert_key) {
        parent_index = current_index;
        if (current_key > insert_key) {
            current_index = left[current_index];
            parent_child = &left[parent_index];
        }
        else {
            current_index = right[current_index];
            parent_child = &right[parent_index];
        }

        current_key = key[current_index];
    }

    if (current_index == NIL) {
        tree_size++;
        current_index = tree_size;

        parent[current_index] = parent_index;
        left[current_index] = NIL;
        right[current_index] = NIL;
        key[current_index] = insert_key;
        *parent_child = current_index;
    }

    return current_index;
}

remove_result tree_remove(int remove_key) {
    int remove_index = tree_search(remove_key);
    int spliced_index;
    if (left[remove_index] == NIL || right[remove_index] == NIL) {
        spliced_index = remove_index;
    }
    else {
        spliced_index = tree_successor(remove_index);
    }

    int spliced_child_index;
    if (left[spliced_index] != NIL) {
        spliced_child_index = left[spliced_index];
    }
    else {
        spliced_child_index = right[spliced_index];
    }

    parent[spliced_child_index] = right[spliced_index];
    if (parent[spliced_child_index] == NIL) {
        root = spliced_child_index;
    }
    else {
        if (spliced_index == left[parent[spliced_index]]) {
            left[parent[spliced_index]] = spliced_child_index;
        }
        else {
            right[parent[spliced_index]] = spliced_child_index;
        }
    }

    if (spliced_index != remove_index) {
        key[remove_index] = key[spliced_index];
    }

    remove_result result;
    result.spliced_index = spliced_index;
    result.spliced_child_index = spliced_child_index;
    return result;
}

int tree_search(int search_key) {
    int current_index = root;
    int current_key = key[root];
    while (current_index != NIL) {
        if (current_key == search_key) {
            return current_index;
        }
        else if (current_key > search_key) {
            current_index = left[current_index];
            current_key = key[current_index];
        }
        else {
            current_index = right[current_index];
            current_key = key[current_index];
        }
    }

    return NIL;
}

int tree_minimum(int root) {
    while (left[root] != NIL) {
        root = left[root];
    }

    return root;
}

int tree_successor(int index) {
    if (right[index] != NIL) {
        return tree_minimum(right[index]);
    }

    int parent_index = parent[index];
    while (parent_index != NIL && index == right[parent_index]) {
        index = parent_index;
        parent_index = parent[index];
    }

    return parent_index;
}

void swap_child(int former_child, int new_child) {
    int parent_index = parent[former_child];
    parent[new_child] = parent_index;
    if (parent_index != NIL) {
        if (left[parent_index] == former_child) {
            left[parent_index] = new_child;
        }
        else {
            right[parent_index] = new_child;
        }
    }
    else {
        root = new_child;
    }
}

/*
 * The node's right child is made the parent of the node, which is now the left child.
 * The right child's left child becomes the right child of the node
 */
void tree_left_rotate(int node_index) {
    int right_child = right[node_index];
    int right_left_child = left[right_child];

    swap_child(node_index, right_child);

    parent[node_index] = right_child;
    left[right_child] = node_index;

    right[node_index] = right_left_child;
    if (right_left_child != NIL) {
        parent[right_left_child] = node_index;
    }
}

void tree_right_rotate(int node_index) {
    int left_child = left[node_index];
    int left_right_child = right[left_child];

    swap_child(node_index, left_child);

    parent[node_index] = left_child;
    right[left_child] = node_index;

    left[node_index] = left_right_child;
    if (left_right_child != NIL) {
        parent[left_right_child] = node_index;
    }
}