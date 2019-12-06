#include "stdio.h"
#include "stdlib.h"

#define MAX_NODES 65536
// degree denotes the number of children of a node
// the number of keys for a given node will be one less than its current degree
#define MIN_CHILDREN 4
#define MIN_KEYS (MIN_CHILDREN - 1)
#define MAX_CHILDREN (MIN_CHILDREN * 2)
#define MAX_KEYS (MAX_CHILDREN - 1)

typedef struct node {
    int index;
    int leaf;
    int key_count;
    int keys[MAX_KEYS];
    int children[MAX_CHILDREN];
} node;

int root;
node nodes[MAX_NODES];

int node_index = 0;
node allocate_node(){
    nodes[node_index].index = node_index;
    return nodes[node_index++];
}

int disk_writes = 0;
int disk_reads = 0;
void disk_write(node*node){
    ++disk_writes;
    nodes[node->index] = *node;
}
node disk_read(int index){
    ++disk_reads;
    return nodes[index];
}

void search(int*node_index,int*child_index,int key);
void search_recur(node*current,int*node_index,int*child_index,int key);
void create();
void insert(int key);
void split_child(node*parent,int child_index,node*child);
void insert_nonfull(node*node,int key);
void print_nodes();

int main(){
    create();
    int keys[75];
    for(int i = 0;i != 75;++i){
        keys[i] = rand();
        insert(keys[i]);
    }
    for(int i = 0;i != 75;++i){
        int node;
        int child;
        search(&node,&child,keys[i]);
    }
    printf("fin\n");
}

void search(int*node_index,int*child_index,int key){
    node current = disk_read(root);
    *node_index = root;
    search_recur(&current,node_index,child_index,key);
}

void search_recur(node*current,int*node_index,int*child_index,int key){
    int i = 0;
    while(i < current->key_count && key > current->keys[i]){++i;}
    if(i <= current->key_count && key == current->keys[i]){
        *child_index = i;
        return;
    }
    if (current->leaf){
        *node_index = -1;
        *child_index = -1;
        return;
    }
    *node_index = current->children[i];
    *current = disk_read(*node_index);
    search_recur(current,node_index,child_index,key);
}

void create(){
    node new_root = allocate_node();
    new_root.leaf = 1;
    new_root.key_count = 0;
    disk_write(&new_root);
    root = new_root.index;
}

void insert(int key){
    node r = disk_read(root);
    if(r.key_count == MAX_KEYS){
        node new_root = allocate_node();
        new_root.leaf = 0;
        new_root.key_count = 0;
        new_root.children[0] = root;
        root = new_root.index;
        split_child(&new_root,0,&r);
        insert_nonfull(&new_root,key);
    }
    else{
        insert_nonfull(&r,key);
    }
}

void split_child(node*parent,int child_index,node*child){
    node new_child = allocate_node();
    new_child.leaf = child->leaf;
    new_child.key_count = MIN_KEYS;
    //transfer the larger keys from child to new_child
    for(int i = 0;i != MIN_KEYS;++i){
        new_child.keys[i] = child->keys[i + MIN_KEYS + 1];
    }
    //likewise transfer the children where applicable
    if(!child->leaf){
        for(int i = 0;i != MIN_CHILDREN;++i){
            new_child.children[i] = child->children[i + MIN_CHILDREN];
        }
    }
    child->key_count = MIN_KEYS;
    //move all children of parent after child up one space
    for(int i = parent->key_count;i != child_index;--i){
        parent->children[i + 1] = parent->children[i];
    }
    //place new_child in the resulting gap
    parent->children[child_index + 1] = new_child.index;
    //move all keys including child's up one space
    //child's old key now belongs to new_child
    for(int i = parent->key_count - 1;i != child_index - 1;--i){
        parent->keys[i + 1] = parent->keys[i];
    }
    //assign the new, lower key for child
    parent->keys[child_index] = child->keys[MIN_KEYS];
    parent->key_count++;
    disk_write(parent);
    disk_write(child);
    disk_write(&new_child);
}

void insert_nonfull(node*node,int key){
    int i = node->key_count - 1;
    if (node->leaf){
        //move the keys that should come after key up one
        //place key in the resulting gap
        while(i >= 0 && key < node->keys[i]){
            node->keys[i + 1] = node->keys[i];
            --i;
        }
        node->keys[i + 1] = key;
        node->key_count++;
        disk_write(node);
    }
    else {
        //find the correct child node for the key
        //if the child is full then split it and pick the correct child of the pair
        //insert into the child
        while(i >= 0 && key < node->keys[i]){--i;}
        ++i;
        struct node child = disk_read(node->children[i]);
        if (child.key_count == MAX_KEYS){
            split_child(node,i,&child);
            if (key > node->keys[i]) {
                child = disk_read(node->children[i + 1]);
            }
        }
        insert_nonfull(&child,key);
    }
}

void print_nodes(){
    printf("%d\n",root);
    for(int i = 0; i != node_index;++i){
        printf("%d\n",i);
        node n = nodes[i];
        printf("leaf:%d,key count:%d\nkeys:\n",n.leaf,n.key_count);
        for(int j = 0;j != n.key_count;++j){
            printf("%d,",n.keys[j]);
        }
        printf("\nchildren:\n");
        for(int j = 0;j != n.key_count + 1;++j){
            printf("%d,",n.children[j]);
        }
        printf("\n\n");
    }
    printf("\n");
}