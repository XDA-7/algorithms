#include "math.h"
#include "stdio.h"
#include "hashing.h"
#define TABLE_SIZE 32

typedef float value_type;

int key_table[TABLE_SIZE];
value_type value_table[TABLE_SIZE];

void insert(int key, value_type value);
value_type search(int key);

int search_probe(int key);
int insert_probe(int key);
int probe(int key, int target_value);
int linear_probe(int key, int i);
int quadratic_probe(int key, int i);
int double_hash(int key, int i);

#define PROBE_HASH(key, target_value) double_hash(key, target_value)

int main() {
    universal_hash_init(839062);
    for (int i = 0; i != TABLE_SIZE; i++) {
        key_table[i] = 0;
        value_table[i] = 0;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        insert(i, i / 3.0f);
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%f\n", search(i));
    }
}

void insert(int key, value_type value) {
    int probe_key = insert_probe(key);
    key_table[probe_key] = key;
    value_table[probe_key] = value;
}

value_type search(int key) {
    int probe_key = search_probe(key);
    return value_table[probe_key];
}

int search_probe(int key) { return probe(key, key); }
int insert_probe(int key) { return probe(key, 0); }

int probe(int key, int target_value) {
    int probe_key;
    int resolved_key;
    int i = 0;
    do {
        probe_key = PROBE_HASH(key, i);
        resolved_key = key_table[probe_key];
        if (resolved_key == target_value) {
            return probe_key;
        }

        i++;
    } while (key_table[probe_key] != 0 && i != TABLE_SIZE);

    return -1;
}

int linear_probe(int key, int i) {
    return (universal_hash(key, TABLE_SIZE) + i) % TABLE_SIZE;
}

int quadratic_probe(int key, int i) {
    return ((i * i + i) / 2 + key) % TABLE_SIZE;
}

int double_hash(int key, int i) {
    return (universal_hash(key, TABLE_SIZE) + i * ((key / 2 + 1) % TABLE_SIZE)) % TABLE_SIZE;
}