#include "stdio.h"

void to_skewed_space(float *x, float *y) {
    // (square_root(3.0) - 1.0) / 2.0;
    static float skewCoefficient = 0.366071f;
    float skew = (*x + *y) * skewCoefficient;
    *x = *x + skew;
    *y = *y + skew;
}

void from_skewed_space(float *x, float *y) {
    // (1.0 - (1.0 / square_root(3.0))) / 2.0
    static float unskewCoefficient = 0.211340f;
    float unskew = (*x + *y) * unskewCoefficient;
    *x = *x - unskew;
    *y = *y - unskew;
}

int main() {
    float x = 5.66f, y = 0.2f;
    to_skewed_space(&x, &y);
    printf("x: %f, y: %f\n", x, y);
    from_skewed_space(&x, &y);
    printf("x: %f, y: %f\n", x, y);
}