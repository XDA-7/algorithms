#include "stdlib.h"
#include "stdio.h"
#include "binary_tree.h"
#include "avl_tree.h"

#define TEST_SIZE 7000

int key_replica[TEST_SIZE];
int values[TEST_SIZE];

int main() {
    srand(78265);
    avl_tree_create(20);
    for (int i = 1; i < TEST_SIZE; i++) {
        int key = rand() + 1;
        key_replica[i] = key;
        int insert_index = avl_tree_insert(key);
        if (verify_height_integrity()) {
            tree_print();
        }

        values[insert_index] = -key;
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int key = key_replica[i];
        int search_index = tree_search(key);
        if (values[search_index] != -key) {
            printf("error retrieving key %d at %d\n", key, i);
        }
        else {
            // printf("found value %d\n", values[search_index]);
        }
    }

    int min;
    int max;
    tree_get_heights(&max, &min);
    printf("max: %d, min: %d\n", max, min);
    printf("done\n");
}