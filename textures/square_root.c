#include "square_root.h"

int rough_estimate(float x) {
    int tx = x;
    int msb = 0;
    while (tx) {
        msb++;
        tx = tx / 2;
    }

    return 1 << (msb / 2);
}

/*
 * Newton's method: x(n + 1) = x(n) - f(x(n)) / f'(x(n))
 * where values x are approximations of a root of the function
 * let f(x) = x^2 - c where c is the value we want the sqrt of
 * f'(x) = 2x
 */
float square_root(float x) {
    float n = (float)rough_estimate(x);
    float nSqr = n * n;
    while (nSqr < (x * 0.999f) || nSqr > (x * 1.001f)) {
        n = n - ((nSqr - x) / (2 * n));
        nSqr = n * n;
    }

    return n;
}