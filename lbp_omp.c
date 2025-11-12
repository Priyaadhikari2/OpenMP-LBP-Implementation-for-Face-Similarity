#include "util.h"
#include <math.h>
#include <stdint.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_HEIGHT 128
#define IMAGE_WIDTH 128
#define INVALID_NUM_INIT_VALUE (-666)
#define GET_TIME omp_get_wtime()

uint8_t apply_filter_on_pixel(int **img, int row, int col)
{
    uint8_t decimal = 0;
    int neighbors[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

    for (int i = 0; i < 8; i++)
    {
        int r = row + neighbors[i][0];
        int c = col + neighbors[i][1];
        if (img[r][c] >= img[row][col])
            decimal |= (1 << (7 - i));
    }
    return decimal;
}

void create_histogram(int **img, int *hist)
{
#pragma omp parallel for collapse(2)
    for (int i = 1; i < IMAGE_HEIGHT - 1; ++i)
    {
        for (int j = 1; j < IMAGE_WIDTH - 1; ++j)
        {
            uint8_t val = apply_filter_on_pixel(img, i, j);
#pragma omp atomic
            hist[val]++;
        }
    }
}

double distance(int *a, int *b, int size)
{
    double sum = 0.0;
    for (int i = 0; i < size; ++i)
    {
        if (a[i] + b[i] != 0)
            sum += pow(a[i] - b[i], 2) / (double)(a[i] + b[i]);
    }
    return sum;
}

int main()
{
    double start_seq = GET_TIME;

    printf("Starting LBP OMP computation...\n");

    // Dummy example
    int **image = read_pgm_file("../cs426_project3_code/images/1.1.txt", IMAGE_HEIGHT, IMAGE_WIDTH);

    int hist[256] = {0};
    for (int i = 0; i < 256; i++)
        hist[i] = 0;

    create_histogram(image, hist);

    double end_seq = GET_TIME;
    printf("Execution time: %lf seconds\n", end_seq - start_seq);

    dealloc_2d_matrix(image, IMAGE_HEIGHT, IMAGE_WIDTH);
    return 0;
}
