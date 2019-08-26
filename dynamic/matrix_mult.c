#include "limits.h"
#include "matrix_mult.h"

#define MATRIX_COUNT 255
#define MATRIX_ARR_SIZE (MATRIX_COUNT + 1)
#define MATRIX_RANGE_ARR_SIZE (MATRIX_ARR_SIZE * MATRIX_ARR_SIZE)
#define RANGE_INDEX(i, j) ((i) * MATRIX_COUNT + (j))
#define MATRIX_ROW_COUNT(i) (matrix_dims[(i) - 1])
#define MATRIX_COLUMN_COUNT(i) (matrix_dims[i])

int matrix_count;
int matrix_dims[MATRIX_ARR_SIZE];

int optimal_score[MATRIX_RANGE_ARR_SIZE];
int optimal_split_index[MATRIX_RANGE_ARR_SIZE];

void add_first_matrix(int rows, int columns) {
    matrix_dims[0] = rows;
    matrix_dims[1] = columns;
    matrix_count = 1;
}

void add_matrix(int columns) {
    matrix_count++;
    matrix_dims[matrix_count] = columns;
}

void calculate_mult_order() {
    for (int i = 1; i <= matrix_count; i++) {
        optimal_score[RANGE_INDEX(i, i)] = 0;
    }

    for (int chain_length = 2; chain_length <= matrix_count; chain_length++) {
        for (int i = 1; i <= matrix_count - chain_length + 1; i++) {
            int j = i + chain_length - 1;
            int ijRangeIndex = RANGE_INDEX(i, j);
            optimal_score[ijRangeIndex] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = optimal_score[RANGE_INDEX(i, k)] + optimal_score[RANGE_INDEX(k + 1, j)];
                cost += MATRIX_ROW_COUNT(i) * MATRIX_COLUMN_COUNT(k) * MATRIX_COLUMN_COUNT(j);
                if (cost < optimal_score[ijRangeIndex]) {
                    optimal_score[ijRangeIndex] = cost;
                    optimal_split_index[ijRangeIndex] = k;
                }
            }
        }
    }
}

int get_final_score() {
    return optimal_score[RANGE_INDEX(1, matrix_count)];
}