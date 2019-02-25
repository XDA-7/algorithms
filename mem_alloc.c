#include "stdio.h"

// 32 bit -> 4294967296
char gHeap[1073741824];

typedef struct MemNode MemNode;
struct MemNode {
    // size of the array that appears after the MemNode struct
    int size;
    MemNode *next;
};

MemNode *gFreeList = (MemNode*)gHeap;

void mem_alloc_init();
// memory allocator implemented with a simple free-list
void *mem_alloc(int size);
void mem_free(void *mem);

void debug_memnode(void *mem);
void debug_free_list();

int main() {
    mem_alloc_init();

    int *singles = mem_alloc(2000 * sizeof(int));
    int *doubles = mem_alloc(2000 * sizeof(int));
    int *triples = mem_alloc(2000 * sizeof(int));

    for (int i = 0; i < 2000; i++) {
        singles[i] = i;
        doubles[i] = i * 2;
        triples[i] = i * 3;
    }

    mem_free(doubles);
    int *retrievedValues = mem_alloc(2000 * sizeof(int));
    for (int i = 0; i < 2000; i++) {
        printf("%d\n", retrievedValues[i]);
    }

    mem_free(triples);
    mem_free(doubles);
    mem_free(singles);
    debug_free_list();
}

void *mem_alloc(int size) {
    MemNode **freeNode = &gFreeList;
    MemNode *allocNode = 0;
    while(allocNode == 0 && (*freeNode) != 0)
    {
        // freeNode can be split into two smaller nodes
        if (((*freeNode)->size - sizeof(MemNode)) > size) {
            // create a new free node that comes after the allocated node
            MemNode *newFreeNode = (MemNode*)((char*)(*freeNode) + size + sizeof(MemNode));
            newFreeNode->size = (*freeNode)->size - (sizeof(MemNode) + size);
            newFreeNode->next = (*freeNode)->next;
            // the freeNode now becomes the allocated block
            (*freeNode)->size = size;
            (*freeNode)->next = 0;
            // the old free node is now the allocated node
            allocNode = *freeNode;
            // and is replaced in the list by the new free node
            *freeNode = newFreeNode;
        }
        // not enough space left over to split node
        else if ((*freeNode)->size >= size) {
            allocNode = *freeNode;
            *freeNode = (*freeNode)->next;
        }

        freeNode = &(*freeNode)->next;
    }
    
    if (allocNode != 0) {
        return (MemNode*)allocNode + 1;
    }
    else {
        return 0;
    }
}

void mem_alloc_init() {
    gFreeList->size = 1073741824 - sizeof(MemNode);
    gFreeList->next = 0;
}

void mem_free(void *mem) {
    MemNode *node = (MemNode*)mem - 1;
    MemNode **nextFreeNode = &gFreeList;
    while (*nextFreeNode < node) {
        nextFreeNode = &(*nextFreeNode)->next;
    }

    node->next = *nextFreeNode;
    *nextFreeNode = node;
}

void debug_memnode(void *mem) {
    MemNode *node = (MemNode*)mem - 1;
    printf("base address: %d, size: %d, next: %d\n", node, node->size, node->next);
}

void debug_free_list() {
    printf("free list\n");
    MemNode *node = gFreeList;
    while(node->next != 0) {
        printf("base address: %d, size: %d, next: %d\n", node, node->size, node->next);
        node = node->next;
    }

    printf("base address: %d, size: %d, next: %d\n", node, node->size, node->next);
}