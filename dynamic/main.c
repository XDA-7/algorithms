#include "stdio.h"
#include "bitonic_tour.h"

Point graph[4] = {
    { 1.0, 5.0 },
    { 3.0, 2.0 },
    { 4.0, 6.0 },
    { 5.0, 3.0 },
};

int main() {
    printf("%f\n", calculate_bitonic_cycle(graph, 4));
}