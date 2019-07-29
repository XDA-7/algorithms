#include "stdio.h"

#define TREE_MAX_SIZE 1024
#define NIL 0
#define RED 0
#define BLACK 1

typedef const char * value_type;

int parent[TREE_MAX_SIZE];
int left[TREE_MAX_SIZE];
int right[TREE_MAX_SIZE];
int key[TREE_MAX_SIZE];
value_type value[TREE_MAX_SIZE];
int colour[TREE_MAX_SIZE];

int root;
int tree_size;

void create_tree(int root_key, value_type root_value, value_type nil_value);
void insert(int insert_key, value_type insert_value);
void insert_fixup(int insert_index);
value_type search(int search_key);
void left_rotate(int node_index);
void right_rotate(int node_index);
void swap_child(int former_child, int new_child);

void print_tree();

int main() {
    create_tree(43, "a", "NIL");
    print_tree();
    insert(2, "c");
    print_tree();
    insert(7, "d");
    print_tree();
    insert(11, "b");
    print_tree();
    insert(101, "e");
    print_tree();
    insert(111, "g");
    print_tree();
    insert(120, "f");
    print_tree();

    printf("done\n");
}

void create_tree(int root_key, value_type root_value, value_type nil_value) {
    root = 1;
    tree_size = 1;

    value[NIL] = nil_value;
    colour[NIL] = BLACK;

    key[1] = root_key;
    value[1] = root_value;
    parent[1] = NIL;
    left[1] = NIL;
    right[1] = NIL;
    colour[1] = BLACK;
}

void insert(int insert_key, value_type insert_value) {
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

    if (current_index != NIL) {
        value[current_index] = insert_value;
    }
    else {
        tree_size++;
        current_index = tree_size;

        parent[current_index] = parent_index;
        left[current_index] = NIL;
        right[current_index] = NIL;
        key[current_index] = insert_key;
        value[current_index] = insert_value;
        colour[current_index] = RED;

        *parent_child = current_index;
        insert_fixup(current_index);
    }
}

void recolour_ancestors_and_move_up(int *current_index, int uncle) {
    colour[uncle] = BLACK;
    colour[parent[*current_index]] = BLACK;
    colour[parent[parent[*current_index]]] = RED;
    *current_index = parent[parent[*current_index]];
}

void insert_fixup(int insert_index) {
    int current_index = insert_index;
    while (colour[parent[current_index]] == RED) {
        if (parent[current_index] == left[parent[parent[current_index]]]) {
            int uncle = right[parent[parent[current_index]]];
            if (colour[uncle] == RED) {
                recolour_ancestors_and_move_up(&current_index, uncle);
            }
            else {
                if (current_index == right[parent[current_index]]) {
                    current_index = parent[current_index];
                    left_rotate(current_index);
                }

                colour[parent[current_index]] = BLACK;
                colour[parent[parent[current_index]]] = RED;
                right_rotate(parent[parent[current_index]]);
            }
        }
        else {
            int uncle = left[parent[parent[current_index]]];
            if (colour[uncle] == RED) {
                recolour_ancestors_and_move_up(&current_index, uncle);
            }
            else {
                if (current_index == left[parent[current_index]]) {
                    current_index = parent[current_index];
                    right_rotate(current_index);
                }

                colour[parent[current_index]] = BLACK;
                colour[parent[parent[current_index]]] = RED;
                left_rotate(parent[parent[current_index]]);
            }
        }
    }

    colour[root] = BLACK;
}

value_type search(int search_key) {
    int current_index = root;
    int current_key = key[root];
    while (current_index != NIL) {
        if (current_key == search_key) {
            return value[current_index];
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

    return value[NIL];
}

/*
 * The node's right child is made the parent of the node, which is now the left child.
 * The right child's left child becomes the right child of the node
 */
void left_rotate(int node_index) {
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

void right_rotate(int node_index) {
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

void print_tree() {
    printf("root: %d\n", root);
    for (int i = 0; i <= tree_size; i++) {
        printf("node: %d, parent: %d, left: %d, right: %d, key: %d, colour: %d\n", i, parent[i], left[i], right[i], key[i], colour[i]);
    }
}