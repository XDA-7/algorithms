#include "stdlib.h"
#include "stdio.h"

typedef struct Node Node;

struct Node {
    int value;
    Node *next;
};

void insert_value_to_list(Node **head, Node *new_node, int value) {
    new_node->value = value;
    Node **next = head;
    while (*next != 0 && (*next)->value < value) {
        next = &(*next)->next;
    }

    new_node->next = *next;
    *next = new_node;
}

void move_list_to_array(Node *head, int *arr, int *arr_index) {
    Node *next = head;
    while (next != 0) {
        arr[*arr_index] = next->value;
        (*arr_index)++;
        next = next->next;
    }
}

void bucket_sort(int * list, int list_size, int *sorted_list) {
    Node nodes[list_size];
    Node *buckets[list_size];
    for (int i = 0; i < list_size; i++) {
        buckets[i] = 0;
    }

    int window = (RAND_MAX / list_size) + 1;
    for (int i = 0; i < list_size; i++) {
        int value = list[i];
        int bucket_index = value / window;
        insert_value_to_list(&buckets[bucket_index], &nodes[i], value);
    }

    int sorted_list_index = 0;
    for (int i = 0; i < list_size; i++) {
        move_list_to_array(buckets[i], sorted_list, &sorted_list_index);
    }
}

int main() {
    const int list_size = 20000;
    srand(1);
    int list[list_size];
    for (int i = 0; i < list_size; i++) {
        list[i] = rand();
    }

    int sorted_list[list_size];
    bucket_sort(list, list_size, sorted_list);
    for (int i = 0; i < list_size; i++) {
        printf("i: %d\n", i);
        printf("%d\n", sorted_list[i]);
    }
}
