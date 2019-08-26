#include "stdio.h"
#include "matrix_mult.h"

int main() {
    add_first_matrix(10, 50);
    add_matrix(20);
    add_matrix(20);
    calculate_mult_order();
    printf("%d\n", get_final_score());
    printf("done");
}