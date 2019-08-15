
#include "binary_tree.h"
#include "stdio.h"

void tree_print() {
    printf("root: %d\n", root);
    for (int i = 0; i <= tree_size; i++) {
        printf("node: %d, parent: %d, left: %d, right: %d, key: %d\n", i, parent[i], left[i], right[i], key[i]);
    }
}

void tree_get_heights(int *max_height, int *min_height) {
    *max_height = 0;
    *min_height = 0;
    int current_path_height = 0;
    int previous_index = NIL;
    int current_index = root;
    while (current_index != NIL) {
        /*modify heights if at leaf*/
        if (left[current_index] == NIL && right[current_index] == NIL) {
            if (current_path_height > *max_height) {
                *max_height = current_path_height;
            }
            if (*min_height == 0 || current_path_height < *min_height) {
                *min_height = current_path_height;
            }
        }

        if (previous_index == parent[current_index]) {
            previous_index = current_index;
            if (left[current_index] != NIL) {
                current_index = left[current_index];
                current_path_height++;
            }
            else if (right[current_index] != NIL) {
                current_index = right[current_index];
                current_path_height++;
            }
            else {
                current_index = parent[current_index];
                current_path_height--;
            }
        }
        else if (previous_index == left[current_index]) {
            previous_index = current_index;
            if (right[current_index] != NIL) {
                current_index = right[current_index];
                current_path_height++;
            }
            else {
                current_index = parent[current_index];
                current_path_height--;
            }
        }
        else {
            previous_index = current_index;
            current_index = parent[current_index];
            current_path_height--;
        }
    }
}