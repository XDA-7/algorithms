#include "random.h"
/*
* Modulus should be a power of 2
* m and the offset c are relatively prime,
* a-1 is divisible by all prime factors of m,
* a-1 is divisible by 4 if m is divisible by 4.
*/
#define RANDOM_SEED 17
#define RANDOM_SIZE 65536
#define RANDOM_MODULUS 65536
#define RANDOM_MULTIPLIER 3
#define RANDOM_INCREMENT 5

float gRandomFloats[RANDOM_SIZE];
int gRandomInts[RANDOM_SIZE];
char gRandomBytes[RANDOM_SIZE];

// Xn+1 = (aXn + c) mod m
void init_random() {
    int currentRandom = RANDOM_SEED;
    for (int i = 1; i < RANDOM_SIZE; i++) {
        currentRandom = (RANDOM_MULTIPLIER * currentRandom + RANDOM_INCREMENT) % RANDOM_MODULUS;
        gRandomInts[i] = currentRandom;
        gRandomBytes[i] = currentRandom / 256;
    }

    for (int i = 1; i < RANDOM_SIZE; i++) {
        gRandomFloats[i] = gRandomInts[i] / (float)RANDOM_MODULUS;
    }
}

float get_random_float(int index) {
    return gRandomFloats[index % RANDOM_SIZE];
}

int get_random_int(int index) {
    return gRandomInts[index % RANDOM_SIZE];
}

char get_random_byte(int index) {
    return gRandomBytes[index % RANDOM_SIZE];
}