#include <stdio.h>
#include <SDL.h>
#include "random.h"
#include "square_root.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define UNIT_SIZE 1
#define CELL_COUNT_HORIZONTAL (SCREEN_WIDTH / UNIT_SIZE)
#define CELL_COUNT_VERTICAL (SCREEN_HEIGHT / UNIT_SIZE)
#define GRID_SIZE 200
// need one extra x and y layer for cells on the edge
#define GRID_COUNT_VERTICAL ((CELL_COUNT_VERTICAL / GRID_SIZE) + 1)
#define GRID_COUNT_HORIZONTAL ((CELL_COUNT_HORIZONTAL / GRID_SIZE) + 1)
#define GRID_COUNT (GRID_COUNT_VERTICAL * GRID_COUNT_HORIZONTAL)

FILE * gErrorLog = 0;

SDL_Event e;
SDL_Window *gWindow = 0;
SDL_Surface *gWindowSurface = 0;
SDL_Renderer *gRenderer = 0;
SDL_Rect gDrawingRect;

void init();
void close();
void log_message(char *message);
void log_int(int message);
void log_float(float message);

void clear_screen();
void color_cell(int x, int y, int value);

void draw_simplex_noise();
void get_grid_components(int x, int y, int *gridX, int *gridY, float *gridPosX, float *gridPosY);
void to_skewed_space(float *x, float *y);
void from_skewed_space(float *x, float *y);
float get_kernel_contribution(float gradX, float gradY, float distX, float distY);

float lerp(float x, float y, float t);
float bilerp(float v00, float v01, float v10, float v11, float x, float y);

int main(int argc, char * argv[])
{
    init();
    init_random();
    draw_simplex_noise();
    SDL_RenderPresent(gRenderer);

    int quit = 0;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    close();
    return 0;
}

void draw_simplex_noise() {
    float minVal = 0.0f;
    float maxVal = 0.0f;
    for (int i = 0; i < CELL_COUNT_VERTICAL; i++) {
        for (int j = 0; j < CELL_COUNT_HORIZONTAL; j++) {
            float skewedX = j;
            float skewedY = i;
            to_skewed_space(&skewedX, &skewedY);
            int skewedGridX, skewedGridY;
            float skewedGridPosX, skewedGridPosY;
            get_grid_components(skewedX, skewedY, &skewedGridX, &skewedGridY, &skewedGridPosX, &skewedGridPosY);
            int simplexCoordinates[6];
            simplexCoordinates[0] = skewedGridX;
            simplexCoordinates[1] = skewedGridY;
            if (skewedGridPosX > skewedGridPosY) {
                simplexCoordinates[2] = skewedGridX + 1;
                simplexCoordinates[3] = skewedGridY;
            }
            else {
                simplexCoordinates[2] = skewedGridX;
                simplexCoordinates[3] = skewedGridY + 1;
            }

            simplexCoordinates[4] = skewedGridX + 1;
            simplexCoordinates[5] = skewedGridY + 1;
            float kernelValue = 0.0f;
            for (int k = 0; k < 6; k += 2) {
                int simplexX = simplexCoordinates[k];
                int simplexY = simplexCoordinates[k + 1];
                float unskewedSimplexX = (float)simplexX, unskewedSimplexY = (float)simplexY;
                from_skewed_space(&unskewedSimplexX, &unskewedSimplexY);
                float distX = (j / (float)GRID_SIZE) - unskewedSimplexX;
                float distY = (i / (float)GRID_SIZE) - unskewedSimplexY;
                int simplexGradIndex = (simplexX * GRID_COUNT_HORIZONTAL + simplexY) * 2;
                float gradX = get_random_byte(simplexGradIndex);
                float gradY = get_random_byte(simplexGradIndex + 1);
                float contribution = get_kernel_contribution(gradX, gradY, distX, distY);
                kernelValue += contribution;
            }

            kernelValue = (kernelValue + 5.2f) * 20.0f;
            if (kernelValue > maxVal) {
                maxVal = kernelValue;
            }
            if (kernelValue < minVal) {
                minVal = kernelValue;
            }
            color_cell(j, i, (int)kernelValue);
        }
    }

    log_float(maxVal);
    log_float(minVal);
    log_message("drawing complete");
}

void get_grid_components(int x, int y, int *gridX, int *gridY, float *gridPosX, float *gridPosY) {
    *gridX = x / GRID_SIZE;
    *gridY = y / GRID_SIZE;
    *gridPosX = (float)(x % GRID_SIZE) / GRID_SIZE;
    *gridPosY = (float)(y % GRID_SIZE) / GRID_SIZE;
}

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

// (max(0, r^2 - d^2))^4 * (<distX, distY> . <gradX, gradY>)
// r^2 = 0.6 by default, set to 0.5 to eliminate discontinuities
float get_kernel_contribution(float gradX, float gradY, float distX, float distY) {
    static float rSqrd = 0.6f;
    float distSqrd = distX * distX + distY * distY;
    float coefficient = rSqrd - distSqrd;
    if (coefficient < 0.0f) {
        coefficient = 0.0f;
    }

    coefficient = coefficient * coefficient * coefficient * coefficient;
    return coefficient * (gradX * distX + gradY * distY);
}

void clear_screen() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gRenderer);
}

void color_cell(int x, int y, int value) {
    gDrawingRect = (SDL_Rect){
        x * UNIT_SIZE,
        y * UNIT_SIZE,
        UNIT_SIZE,
        UNIT_SIZE
    };
    SDL_SetRenderDrawColor(gRenderer, value, value, value, 0xFF);
    SDL_RenderFillRect(gRenderer, &gDrawingRect);
}

float lerp(float x, float y, float t) {
    return (1 - 1) * x + t * y;
}

float bilerp(float v00, float v01, float v10, float v11, float x, float y) {
    float invX = 1.0f - x;
    float invY = 1.0f - y;
    return
        v00 * invX * invY +
        v10 * x * invY +
        v01 * invX * y +
        v11 * x * y;
}

void log_message(char *message) {
    fputs(message, gErrorLog);
    fputc('\n', gErrorLog);
    fflush(gErrorLog);
}

void log_int(int message) {
    fprintf(gErrorLog, "%d\n", message);
    fflush(gErrorLog);
}

void log_float(float message) {
    fprintf(gErrorLog, "%f\n", (double)message);
    fflush(gErrorLog);
}

void init() {
    gErrorLog = fopen("errorlog.txt", "w+");
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("automata", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gWindowSurface = SDL_GetWindowSurface(gWindow);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

void close() {
    if (SDL_GetError()) {
        fputs(SDL_GetError(), gErrorLog);
    }

    fclose(gErrorLog);
    gErrorLog = 0;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = 0;
    SDL_DestroyWindow(gWindow);
    gWindow = 0;
    SDL_Quit();
}