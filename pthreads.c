#include "stdio.h"
#include "pthread.h"

pthread_mutex_t print_mut;
int g_val = 0;

void *print_number_and_square(void *arg) {
    while (1) {
        int val = ++g_val;
        int sqr = val * val;
        pthread_mutex_lock(&print_mut);
        printf("%d", val);
        printf(", %d\n", sqr);
        pthread_mutex_unlock(&print_mut);
    }
    return 0;
}

int main() {
    pthread_t threads[100];
    pthread_mutex_init(&print_mut, 0);
    for (int i = 0; i < 100; i++) {
        pthread_create(&threads[i], 0, &print_number_and_square, 0);
    }

    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], 0);
    }
}