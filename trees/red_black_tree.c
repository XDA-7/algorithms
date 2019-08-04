#include "stdio.h"

#define TREE_MAX_SIZE 1048575
#define ARR_SIZE TREE_MAX_SIZE + 1
#define NIL 0
#define RED 0
#define BLACK 1

typedef int value_type;

int parent[ARR_SIZE];
int left[ARR_SIZE];
int right[ARR_SIZE];
int key[ARR_SIZE];
value_type value[ARR_SIZE];
int colour[ARR_SIZE];

int root;
int tree_size;

void create_tree(int root_key, value_type root_value, value_type nil_value);
void insert(int insert_key, value_type insert_value);
void delete(int delete_key);
void insert_fixup(int insert_index);
void delete_fixup(int spliced_child_index);
int search(int search_key);
int minimum(int root);
int successor(int index);
void left_rotate(int node_index);
void right_rotate(int node_index);
void swap_child(int former_child, int new_child);

void print_tree();

void manual_input() {
    printf("begin\n");
    int option = -1;
    int input = 0;
    while (option) {
        scanf("%d", &option);
        if (option == 1) {
            printf("insert\n");
            scanf("%d", &input);
            insert(input, input * input);
        }
        else if (option == 2) {
            printf("delete\n");
            scanf("%d", &input);
            delete(input);
        }
        else if (option == 3) {
            printf("search\n");
            scanf("%d", &input);
            printf("result: %d\n", value[search(input)]);
        }
        else if (option == 4) {
            print_tree();
        }
    }
}

int main() {
    create_tree(0, 0, -1);

    for (int i = 0; i < TREE_MAX_SIZE; i++) {
        int val = i * 3;
        insert(val, val * val);
    }

    manual_input();

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

void delete(int delete_key) {
    int delete_index = search(delete_key);
    int spliced_index;
    if (left[delete_index] == NIL || right[delete_index] == NIL) {
        spliced_index = delete_index;
    }
    else {
        spliced_index = successor(delete_index);
    }

    int spliced_child_index;
    if (left[spliced_index] != NIL) {
        spliced_child_index = left[spliced_index];
    }
    else {
        spliced_child_index = right[spliced_index];
    }

    parent[spliced_child_index] = parent[spliced_index];
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

    if (spliced_index != delete_index) {
        key[delete_index] = key[spliced_index];
        value[delete_index] = value[spliced_index];
    }

    if (colour[spliced_index] == BLACK) {
        delete_fixup(spliced_child_index);
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

void delete_fixup(int recolour_index) {
    while (recolour_index != root && colour[recolour_index] == BLACK) {
        if (recolour_index == left[parent[recolour_index]]) {
            int sibling = right[parent[recolour_index]];
            if (colour[sibling] == RED) {
                colour[sibling] = BLACK;
                colour[parent[recolour_index]] = RED;
                left_rotate(parent[recolour_index]);
                sibling = right[parent[recolour_index]];
            }

            if (colour[left[sibling]] == BLACK && colour[right[sibling]] == BLACK) {
                colour[sibling] = RED;
                recolour_index = parent[recolour_index];
            }
            else {
                if (colour[right[sibling]] == BLACK) {
                    colour[left[sibling]] = BLACK;
                    colour[sibling] = RED;
                    right_rotate(sibling);
                    sibling = right[parent[recolour_index]];
                }

                colour[sibling] = colour[parent[recolour_index]];
                colour[parent[recolour_index]] = BLACK;
                colour[right[sibling]] = BLACK;
                left_rotate(parent[recolour_index]);
                recolour_index = root;
            }
        }
        else {
            int sibling = left[parent[recolour_index]];
            if (colour[sibling] == RED) {
                colour[sibling] = BLACK;
                colour[parent[recolour_index]] = RED;
                right_rotate(parent[recolour_index]);
                sibling = left[parent[recolour_index]];
            }

            if (colour[left[sibling]] == BLACK && colour[right[sibling]] == BLACK) {
                colour[sibling] = RED;
                recolour_index = parent[recolour_index];
            }
            else {
                if (colour[left[sibling]] == BLACK) {
                    colour[right[sibling]] = BLACK;
                    colour[sibling] = RED;
                    left_rotate(sibling);
                    sibling = left[parent[recolour_index]];
                }

                colour[sibling] = colour[parent[recolour_index]];
                colour[parent[recolour_index]] = BLACK;
                colour[left[sibling]] = BLACK;
                right_rotate(parent[recolour_index]);
                recolour_index = root;
            }
        }
    }

    colour[recolour_index] = BLACK;
}

int search(int search_key) {
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

int minimum(int root) {
    while (left[root] != NIL) {
        root = left[root];
    }

    return root;
}

int successor(int index) {
    if (right[index] != NIL) {
        return minimum(right[index]);
    }

    int parent_index = parent[index];
    while (parent_index != NIL && index == right[parent_index]) {
        index = parent_index;
        parent_index = parent[index];
    }

    return parent_index;
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