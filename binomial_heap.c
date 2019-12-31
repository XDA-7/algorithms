#include "stdio.h"

#define MAX_HEAP_SIZE 65535
#define MAX_KEY_SIZE 65535

int head[MAX_HEAP_SIZE];
int head_count = 0;
int element_count = 0;
int key[MAX_HEAP_SIZE];
int parent[MAX_HEAP_SIZE];
int child[MAX_HEAP_SIZE];
int sibling[MAX_HEAP_SIZE];
int degree[MAX_HEAP_SIZE];

void print_binomial_tree(int index) {
    int i = index;
    while(i != 0) {
        printf("%d,",key[i]);
        i = sibling[i];
    }
    printf("\n");
    i = index;
    while(i != 0) {
        printf("children of %d\n",key[i]);
        print_binomial_tree(child[i]);
        i = sibling[i];
    }
}

void print_binomial_heap(int heap) {
    int list = head[heap];
    printf("heap\n");
    while(list != 0) {
        printf("tree root: %d\n",key[list]);
        print_binomial_tree(child[list]);
        list = sibling[list];
    }
}

int make_binomial_heap() {
    head[++head_count] = 0;
    return head_count;
}

int binomial_heap_minimum(int h) {
    int min_index = -1;
    int min_key = MAX_KEY_SIZE + 1;
    int i = head[h];
    while (i != 0) {
        if(key[i] < min_key) {
            min_index = i;
            min_key = key[i];
        }
        i = sibling[i];
    }
    return min_index;
}

int binomial_heap_merge(int h1,int h2) {
    int l1 = head[h1];
    int l2 = head[h2];
    if (l1 == 0) {
        return l2;
    }
    if(l2 == 0) {
        return l1;
    }
    int merged_list;
    if (degree[l1] < degree[l2]) {
        merged_list = l1;
        l1 = sibling[l1];
    }
    else {
        merged_list = l2;
        l2 = sibling[l2];
    }
    int list_iter = merged_list;
    while(l1 != 0 && l2 != 0) {
        if (degree[l2] < degree[l1]) {
            sibling[list_iter] = l2;
            l2 = sibling[l2];
        }
        else {
            sibling[list_iter] = l1;
            l1 = sibling[l1];
        }
        list_iter = sibling[list_iter];
    }
    sibling[list_iter] = l1 == 0 ? l2 : l1;
    return merged_list;
}

// z will become the parent of y
void binomial_link(int y, int z) {
    parent[y] = z;
    sibling[y] = child[z];
    child[z] = y;
    ++degree[z];
}

int binomial_heap_union(int h1,int h2) {
    int heap = make_binomial_heap();
    head[heap] = binomial_heap_merge(h1,h2);
    if(head[heap] == 0) {
        return heap;
    }
    int prev_x = 0;
    int x = head[heap];
    int next_x = sibling[x];
    while(next_x != 0) {
        // cases
        // 1: x and next-x are not equal
        // 2: x and next-x are equal and so is next-next x
        if(degree[x] != degree[next_x] ||
            (sibling[next_x] != 0 && degree[sibling[next_x] == degree[x]])) {
                prev_x = x;
                x = next_x;
        }
        else if(key[x] < key[next_x]) {
            // case 3: make x the parent of next-x
            sibling[x] = sibling[next_x];
            binomial_link(next_x,x);
        }
        else {
            // case 4: make next-x the parent of x
            // assign the sibling of prev-x to be next-x only if prev-x exists, otherwise make next-x the head of the list
            if(prev_x == 0) {
                head[heap] = next_x;
            }
            else {
                sibling[prev_x] = next_x;
            }
            binomial_link(x,next_x);
            x = next_x;
        }
        next_x = sibling[x];
    }
    return heap;
}

int binomial_heap_extract_min(int* h) {
    int list = head[*h];
    if(list == 0) {
        return -1;
    }
    // find the root in the root list with the smallest key
    int min_key = key[list];
    int min_index = list;
    int min_prev_index = -1;
    while(sibling[list] != 0) {
        int list_sibling = sibling[list];
        if (key[list_sibling] < min_key) {
            min_key = key[list_sibling];
            min_index = list_sibling;
            min_prev_index = list;
        }
        list = list_sibling;
    }
    // extract said key from the list
    if (min_prev_index == -1) {
        head[*h] = sibling[min_index];
    }
    else {
        sibling[min_prev_index] = sibling[min_index];
    }
    // reverse the extracted binomial tree
    int new_heap = make_binomial_heap();
    int reverse_list_head = child[min_index];
    int reverse_list_tail = 0;
    while (sibling[reverse_list_head] != 0) {
        int new_head = sibling[reverse_list_head];
        sibling[reverse_list_head] = reverse_list_tail;
        reverse_list_tail = reverse_list_head;
        reverse_list_head = new_head;
    }
    sibling[reverse_list_head] = reverse_list_tail;
    // set the list as the head of the new heap and take the union of the heaps
    head[new_heap] = reverse_list_head;
    *h = binomial_heap_union(*h,new_heap);
    return min_index;
}

int binomial_heap_insert(int heap,int k) {
    int x = ++element_count;
    key[x] = k;
    parent[x] = 0;
    child[x] = 0;
    sibling[x] = 0;
    degree[x] = 0;
    int new_heap = make_binomial_heap();
    head[new_heap] = x;
    return binomial_heap_union(heap,new_heap);
}

int main(){
    int heap = make_binomial_heap();
    int ks[] = {5,23,666,3,66,4,21,17,62,4574,7,91};
    for(int i = 0; i != 12; ++i) {
        heap = binomial_heap_insert(heap,ks[i]);
    }
    for(int i = 0; i != 12; ++i) {
        printf("%d\n",key[binomial_heap_extract_min(&heap)]);
    }
}