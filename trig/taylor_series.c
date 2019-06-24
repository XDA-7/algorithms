#include "stdio.h"

#define ITERATIONS 10

float sine(float radians);

float cosine(float radians);

float arctan(float value);

int main() {
    for (float i = 1.0f; i > (1.0f / 4096.0f); i /= 2.0f) {
        printf("%f: %f\n", i, arctan(i));
    }

    return 0;
}

float sine(float radians) {
    float result = radians;
    float runningX = radians;
    float runningFactorial = 1.0f;
    const int doubleIterations = ITERATIONS * 2;
    for (int i = 2; i < doubleIterations; i += 2) {
        runningX = -runningX * radians * radians;
        runningFactorial = runningFactorial * i * (i + 1);
        result += runningX / runningFactorial;
    }

    return result;
}

float cosine(float radians) {
    float result = 1.0f;
    float runningX = 1.0f;
    float runningFactorial = 1.0f;
    static int doubleIterations = ITERATIONS * 2;
    for (int i = 2; i < doubleIterations; i += 2) {
        runningX = -runningX * radians * radians;
        runningFactorial = runningFactorial * i * (i - 1);
        result += runningX / runningFactorial;
    }

    return result;
}

float arctan(float value) {
    float result = value;
    float runningX = value;
    static int doubleIterations = ITERATIONS * 2;
    for (int i = 3; i < doubleIterations; i += 2) {
        runningX = -runningX * value * value;
        result += runningX / i;
    }

    return result;
}