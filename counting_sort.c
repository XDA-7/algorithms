#include "stdio.h"
#include "stdlib.h"

void counting_sort(int *list, int list_size, int max_val, int *sorted_list);

int main() {
    int unsorted[] = {
        4, 1, 8, 3, 13, 3, 6, 4, 8
    };
    int sorted[9];
    counting_sort(unsorted, 9, 14, sorted);
    int i = 0;
    while (i < 9) {
        printf("%d\n", sorted[i]);
        ++i;
    }
}

void counting_sort(int *list, int list_size, int max_val, int *sorted_list) {
    int count[max_val];
    int i = 0;
    while (i < max_val) {
        count[i] = 0;
        ++i;
    }

    i = 0;
    while (i < list_size) {
        ++count[list[i]];
        ++i;
    }

    i = 1;
    while (i < max_val) {
        count[i] += count[i - 1];
        ++i;
    }

    i = list_size - 1;
    while (i > -1) {
        sorted_list[count[list[i]] - 1] = list[i];
        --count[list[i]];
        --i;
    }
}