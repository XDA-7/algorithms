#include "stdio.h"

int main() {
    int probe = 0;
    int mod = 16;
    int perm[mod];
    for (int i = 0; i < mod; i++) {
        perm[i] = 0;
    }
    
    for (int i = 0; i < mod; i++) {
        probe += i;
        perm[probe % mod] = 1;
        printf("%d\n", probe /*% mod*/);
    }

    for (int i = 0; i < mod; i++) {
        if (!perm[i]) {
            printf("Failed for %d\n", i);
        }
    }

    printf("done\n");
}