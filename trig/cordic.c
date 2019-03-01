#include "stdio.h"

const int iterations = 12;
const float scaling_value = 0.6072f;
// theta value for tan-1(2^-n)
const float tan_theta_lookup[] = {
    0.760460, 0.463648, 0.244979,
    0.124355, 0.062419, 0.031240,
    0.015624, 0.007812, 0.003906,
    0.001953, 0.000977, 0.000488
};

float cordic_sine(float radians);
float cordic_cosine(float radians);

int main() {
    for (float i = -3.14f; i < 3.14f; i += 0.1f) {
        printf("%f: %f\n", i, cordic_sine(i));
    }

    return 0;
}

float cordic_sine(float radians) {
    float x = scaling_value;
    float y = 0.0f;
    float z = radians;
    int divisor = 1;
    for (int i = 0; i < iterations; i++) {
        float tempX = x;
        if (z > 0.0f) {
            x -= y / divisor;
            y += tempX / divisor;
            z -= tan_theta_lookup[i];
        }
        else {
            x += y / divisor;
            y -= tempX / divisor;
            z += tan_theta_lookup[i];
        }

        divisor *= 2;
    }

    return y;
}

float cordic_cosine(float radians) {
    return 0.0f;
}