#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define BYTE_SIZE 8

typedef struct Code {
    int code_length;
    unsigned int code;
} Code;

typedef struct Path {
    int left;
    int right;
    int code;
    float cost;
} Path;

typedef struct QueuedPath {
    int path;
    int next;
} QueuedPath;

unsigned char wordset[] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144 };
float frequencies[] = { 0.0, 0.05, 0.1, 0.025, 0.025, 0.2, 0.05, 0.05, 0.1, 0.05, 0.2, 0.05, 0.1 };
Code codes[13];

int path_root;
int allocated_paths;
Path paths[23];
#define QUEUE_SENTINEL 0
int allocated_queue_paths = 1;
QueuedPath queued_paths[25];

unsigned char initial_set[20000];
unsigned char compressed_words[20000];

void insert_path(int path) {
    QueuedPath qp;
    qp.path = path;
    float cost = paths[path].cost;
    int prev = QUEUE_SENTINEL;
    int current = queued_paths[QUEUE_SENTINEL].next;
    do {
        if (current != QUEUE_SENTINEL && cost > paths[queued_paths[current].path].cost) {
            prev = current;
            current = queued_paths[current].next;
        }
        else {
            qp.next = current;
            queued_paths[prev].next = allocated_queue_paths;
            queued_paths[allocated_queue_paths] = qp;
            allocated_queue_paths++;
            return;
        }
    } while(1);
}

int extract_min() {
    QueuedPath head = queued_paths[queued_paths[QUEUE_SENTINEL].next];
    queued_paths[QUEUE_SENTINEL].next = head.next;
    return head.path;
}

void write_huffman_code_recur(int path, unsigned char *partial_code, int partial_code_size, unsigned char next_bit) {
    unsigned char next_code[partial_code_size + 1];
    memcpy(next_code, partial_code, partial_code_size);
    next_code[partial_code_size] = next_bit;
    if (paths[path].code) {
        unsigned int code = 0;
        for (int i = 0; i < partial_code_size + 1; i++) {
            code = (code << 1) | next_code[i];
        }

        codes[paths[path].code].code = code;
        codes[paths[path].code].code_length = partial_code_size + 1;
    }
    else {
        write_huffman_code_recur(paths[path].left, next_code, partial_code_size + 1, 0u);
        write_huffman_code_recur(paths[path].right, next_code, partial_code_size + 1, 1u);
    }
}

void write_huffman_code(int path) {
    unsigned char empty_code[0];
    write_huffman_code_recur(paths[path].left, empty_code, 0, 0u);
    write_huffman_code_recur(paths[path].right, empty_code, 0, 1u);
}

void create_huffman_code() {
    allocated_paths = 12;
    for (int i = 0; i < 12; i++) {
        paths[i] = (Path){ 0, 0, i + 1, frequencies[i + 1] };
        insert_path(i);
    }

    for (int i = 1; i < 12; i++) {
        Path p;
        p.code = 0;
        p.left = extract_min();
        p.right = extract_min();
        p.cost = paths[p.left].cost + paths[p.right].cost;
        paths[allocated_paths] = p;
        insert_path(allocated_paths);
        allocated_paths++;
    }

    path_root = extract_min();
    write_huffman_code(path_root);
}

unsigned char get_bit(unsigned char charset[], int position) {
    int char_index = position / BYTE_SIZE;
    int bit_position = position % BYTE_SIZE;
    return (charset[char_index] >> bit_position) & 1u;
}

void flip_bit(unsigned char charset[], int position) {
    int char_index = position / BYTE_SIZE;
    int bit_position = position % BYTE_SIZE;
    unsigned char bit_mask = 1u << bit_position;
    charset[char_index] = charset[char_index] ^ bit_mask;
}

unsigned char get_random_char() {
    float val = (float)rand() / RAND_MAX;
    for (int i = 1; i < 13; i++) {
        val -= frequencies[i];
        if (val <= 0.0) {
            return wordset[i];
        }
    }
}

void compress_data() {
    int compressed_word_index = 0;
    for (int i = 0; i < 20000; i++) {
        unsigned char word = initial_set[i];
        int code;
        int code_length;
        for (int j = 1; j < 13; j++) {
            if (word == wordset[j]) {
                code = codes[j].code;
                code_length = codes[j].code_length;
                break;
            }
        }

        for (int j = code_length - 1; j >= 0; j--) {
            if ((code >> j) & 1u) {
                flip_bit(compressed_words, compressed_word_index);
            }

            compressed_word_index++;
        }
    }

    printf("compressed size: %d\n", compressed_word_index / BYTE_SIZE);
}

unsigned char decompress_data(int* index) {
    int path_current = path_root;
    while (!paths[path_current].code) {
        unsigned char bit = get_bit(compressed_words, *index);
        path_current = bit ? paths[path_current].right : paths[path_current].left;
        (*index)++;
    }

    return wordset[paths[path_current].code];
}

int main() {
    for (int i = 0; i < 20000; i++) {
        initial_set[i] = get_random_char();
    }

    create_huffman_code();
    /*for (int i = 1; i < 13; i++) {
        printf("word: %d, code: %d, code length: %d\n", wordset[i], codes[i].code, codes[i].code_length);
    }*/

    printf("create huffman\n");
    compress_data();
    printf("compress data\n");
    int index = 0;
    /*for (int i = 0; i < 100; i++) {
        printf("%u, %u\n", initial_set[i], decompress_data(&index));
    }*/

    printf("fin\n");
}