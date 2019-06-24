#include "stdio.h"

#define left_index(i) (i * 2)
#define right_index(i) (i * 2 + 1)
#define parent_index(i) (i / 2)

void heapsort(int *heap, int length);
void build_max_heap(int *heap, int length);
void max_heapify(int *heap, int length, int i);

int main() {
    int heap[] = { 0, 9, 34, 11, 76, 12, 6, 43, 100, 23, 72, 11, 44, 85, 34, 81 };
    heapsort(heap, 16);
    for (int i = 1; i != 16; i++) {
        printf("%d\n", heap[i]);
    }
}

void heapsort(int *heap, int length) {
    build_max_heap(heap, length);
    for (int i = length - 1; i != 1; i--) {
        int temp = heap[1];
        heap[1] = heap[i];
        heap[i] = temp;
        printf("assigned %d to %d\n", temp, i);
        max_heapify(heap, i - 1, 1);
    }
}

void build_max_heap(int *heap, int length) {
    for (int i = length / 2; i != 0; i--) {
        max_heapify(heap, length, i);
    }
}

void max_heapify(int *heap, int length, int i) {
    int left = left_index(i);
    int right = right_index(i);
    int largest_index;
    if (left <= length && heap[left] > heap[i]) {
        largest_index = left;
    }
    else {
        largest_index = i;
    }

    if (right <= length && heap[right] > heap[largest_index]) {
        largest_index = right;
    }

    if (largest_index != i) {
        int temp = heap[i];
        heap[i] = heap[largest_index];
        heap[largest_index] = temp;
        max_heapify(heap, length, largest_index);
    }
}