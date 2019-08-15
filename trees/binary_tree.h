#ifndef BINARY_TREE
#define BINARY_TREE

#define TREE_MAX_SIZE 65535 // 16 bits
#define ARR_SIZE TREE_MAX_SIZE + 1
#define NIL 0

typedef struct remove_result {
    int spliced_index;
    int spliced_child_index;
} remove_result;

int parent[ARR_SIZE];
int left[ARR_SIZE];
int right[ARR_SIZE];
int key[ARR_SIZE];
int colour[ARR_SIZE];

int root;
int tree_size;

void create_tree(int root_key);
int tree_insert(int insert_key);
remove_result tree_remove(int remove_key);
int tree_search(int search_key);
int tree_minimum();
int tree_successor(int index);
void tree_left_rotate(int node_index);
void tree_right_rotate(int node_index);

void tree_print();
void tree_get_heights(int *max_height, int *min_height);

#endif