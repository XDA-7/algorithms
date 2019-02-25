#include "stdio.h"

void quicksort(int *list, int left, int right);

int partition(int *list, int left, int right);

void dutch_partition(int *list, int left, int right, int *leftPartition, int *rightPartition);

int list[] = { 5, 8, 2, 4, 8, 3, 9, 0, 2, 3, 7, 6, 3, 7, 2, 6, 5, 3, 2, 1, 8, 5, 8, 3, 5, 0, 8, 6, 4, 2, 5, 3, 1, 5 };

// int list[] = { 6, 1, 5, 1, 7, 1, 2 };

int main() {
    quicksort(list, 0, 33);
    for (int i = 0; i < 34; i++) {
        printf("%d,", list[i]);
    }
    printf("\n");
}

void quicksort(int *list, int left, int right) {
    if (right > left) {
        int leftPartition, rightPartition;
        dutch_partition(list, left, right, &leftPartition, &rightPartition);
        quicksort(list, left, leftPartition - 1);
        quicksort(list, rightPartition, right);
    }
}

int partition(int *list, int left, int right) {
    int pivot = list[(left + right) / 2];
    while(1) {
        while (list[left] < pivot) {
            left++;
        }

        while (list[right] > pivot) {
            right--;
        }

        if (left >= right) {
            return right;
        }

        int leftValue = list[left];
        int rightValue = list[right];
        if (leftValue != pivot || rightValue != pivot) {
            list[left] = rightValue;
            list[right] = leftValue;
        }
        else {
            right--;
        }
    }
}

void dutch_partition(int *list, int left, int right, int *leftPartition, int *rightPartition) {
    int mid = list[(left + right) / 2];
    // printf("mid: %d\n", mid);
    int i = left, j = left, n = right;
    while (j <= n) {
        // printf("i: %d, j: %d, n: %d\n", i, j, n);
        int value = list[j];
        if (value < mid) {
            list[j] = list[i];
            list[i] = value;
            i++;
            j++;
        }
        else if (value > mid) {
            list[j] = list[n];
            list[n] = value;
            n--;
        }
        else {
            j++;
        }
    }

    *leftPartition = i;
    *rightPartition = j;
}