#include "stdio.h"

typedef struct Node Node;

struct Node {
    Node *p;
    Node *left;
    Node *right;
    int key;
};

void print_tree(Node *root);
void set_node(Node *node, int key, Node *p, Node *left, Node *right);

int main() {
    Node tree[15];
    set_node(&tree[0], 0, 0, &tree[1], &tree[2]);
    set_node(&tree[1], 1, &tree[0], &tree[3], &tree[4]);
    set_node(&tree[2], 2, &tree[0], &tree[5], &tree[6]);
    set_node(&tree[3], 3, &tree[1], &tree[7], &tree[8]);
    set_node(&tree[4], 4, &tree[1], &tree[9], &tree[10]);
    set_node(&tree[5], 5, &tree[2], &tree[11], &tree[12]);
    set_node(&tree[6], 6, &tree[2], &tree[13], &tree[14]);
    set_node(&tree[7], 7, &tree[3], 0, 0);
    set_node(&tree[8], 8, &tree[3], 0, 0);
    set_node(&tree[9], 9, &tree[4], 0, 0);
    set_node(&tree[10], 10, &tree[4], 0, 0);
    set_node(&tree[11], 11, &tree[5], 0, 0);
    set_node(&tree[12], 12, &tree[5], 0, 0);
    set_node(&tree[13], 13, &tree[6], 0, 0);
    set_node(&tree[14], 14, &tree[6], 0, 0);

    print_tree(tree);
}

void print_tree(Node *root) {
    Node *prev = 0;
    Node *cur = root;
    Node *next = 0;
    while (cur) {
        next = 0;
        if (prev == cur->p) {
            printf("%d\n", cur->key);
            next = cur->left;
        }
        else if (prev == cur->left) {
            next = cur->right;
        }

        prev = cur;
        cur = next ? next : cur->p;
    }
}

void set_node(Node *node, int key, Node *p, Node *left, Node *right) {
    node->key = key;
    node->p = p;
    node->left = left;
    node->right = right;
}