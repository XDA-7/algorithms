#include "stdlib.h"
#include "hashing.h"

int a;
int b;

int p = 999983; // Max key size, prime

void universal_hash_init(int seed) {
    srand(seed);

    a = (rand() % (p - 1)) + 1;
    b = rand() % p;
}

int universal_hash(int key, int mod) {
    return ((a * key + b) % p) % mod;
}