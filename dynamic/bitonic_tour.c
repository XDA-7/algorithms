#include "stdio.h"
#include "math.h"
#include "bitonic_tour.h"

float point_dist(Point a, Point b) {
    float x_diff = a.x - b.x;
    float y_diff = a.y - b.y;
    return sqrtf(x_diff * x_diff + y_diff * y_diff);
}

float calculate_bitonic_cycle(Point *p, int n) {
    float d[n][n - 2];
    int o[n];
    d[0][0] = 0;
    d[1][0] = point_dist(p[0], p[1]) * 2.0;
    for (int i = 2; i < n; i++) {
        float dist = INFINITY;
        for (int j = 0; j < i - 1; j++) {
            d[i][j] = d[i - 1][j] + point_dist(p[i], p[i - 1]) + point_dist(p[i], p[j]) - point_dist(p[i - 1], p[j]);
            printf("i: %d -> %d: %f\n", i, j, d[i][j]);
            if (d[i][j] < dist) {
                dist = d[i][j];
                o[i] = j;
            }
        }

        d[i][i - 1] = dist;
        printf("i: %d = %f\n", i, d[i][i - 1]);
    }

    return d[n - 1][n - 2];
}
