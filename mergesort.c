#include "stdio.h"

int list[] = {
    5, 8, 2, 4, 8, 3, 9, 0, 2, 3, 7, 6, 3, 7, 2, 6, 5, 3, 2, 1, 8, 5, 8, 3, 5, 0, 8, 6, 4, 2, 5, 3, 1, 5
};

int auxiliary_list[34];

int * mergesort(int *list, int *auxList, int listSize);

void merge(int left, int middle, int right, int *from, int *to);

int main() {
    int *sortedList = mergesort(list, auxiliary_list, 34);
    for (int i = 0; i < 34; i++) {
        printf("%d\n", sortedList[i]);
    }
}

int * mergesort(int *list, int *auxList, int listSize) {
    int lastElement = listSize - 1;
    int sortedListSize = 1;
    int increment = 2;
    int *a = list;
    int *b = auxList;
    while (sortedListSize < listSize)
    {
        for (int i = 0; i < listSize; i += increment) {
            int middle = i + sortedListSize;
            int right = i + increment;
            if (middle > listSize) {
                middle = listSize;
                right = listSize;
            }
            else if (right > listSize) {
                right = listSize;
            }

            merge(i, middle, right, a, b);
        }

        sortedListSize = increment;
        increment *= 2;
        int *temp = a;
        a = b;
        b = temp;
    }
    
    return a;
}

// Middle and right indexes are the first elements not in the left and right arrays respectively
void merge(int left, int middle, int right, int *from, int *to) {
    int toIndex = left;
    int leftIndex = left;
    int rightIndex = middle;
    int leftValue = from[left];
    int rightValue = from[middle];
    while(leftIndex != middle && rightIndex != right) {
        if (rightValue >= leftValue) {
            to[toIndex] = leftValue;
            toIndex++;
            leftIndex++;
            leftValue = from[leftIndex];
        }

        if (leftValue >= rightValue) {
            to[toIndex] = rightValue;
            toIndex++;
            rightIndex++;
            rightValue = from[rightIndex];
        }
    }

    while (leftIndex != middle) {
        to[toIndex] = from[leftIndex];
        toIndex++;
        leftIndex++;
    }

    while (rightIndex != right) {
        to[rightIndex] = from[rightIndex];
        toIndex++;
        rightIndex++;
    }
}