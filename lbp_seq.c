#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   // for uint8_t
#include <sys/time.h> // for gettimeofday
#include <math.h>     // for pow()

#define IMAGE_HEIGHT 128
#define IMAGE_WIDTH 128
#define INVALID_NUM_INIT_VALUE (-666)

// Apply LBP filter to a single pixel
uint8_t apply_filter_on_pixel(int **img, int row, int col)
{
    uint8_t decimal = 0;
    int center = img[row][col];

    // Clockwise neighbor positions
    int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int i = 0; i < 8; i++)
    {
        decimal <<= 1;
        if (img[row + dx[i]][col + dy[i]] >= center)
            decimal |= 1;
    }

    return decimal;
}

// Create LBP histogram
void create_histogram(int **img, int *histogram)
{
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    for (int i = 1; i < IMAGE_HEIGHT - 1; ++i)
    {
        for (int j = 1; j < IMAGE_WIDTH - 1; ++j)
        {
            uint8_t lbp_value = apply_filter_on_pixel(img, i, j);
            histogram[lbp_value]++;
        }
    }
}

// Compute distance between two histograms (Chi-square)
double distance(int *a, int *b, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        if (a[i] + b[i] != 0)
            sum += pow((double)(a[i] - b[i]), 2) / (double)(a[i] + b[i]);
    }
    return sum;
}

int main()
{
    int people_count = 5;
    int sample_count_per_person = 10;
    int k = 2;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    printf("Starting Sequential LBP computation...\n");

    int **train_histograms = (int **)malloc(sizeof(int *) * people_count);
    for (int i = 0; i < people_count; i++)
    {
        train_histograms[i] = (int *)malloc(sizeof(int) * 256);
    }

    // Read training images and compute histograms
    // for (int i = 0; i < people_count; ++i)
    // {
    //     for (int j = 0; j < sample_count_per_person - k; ++j)
    //     {
    //         char buff[128];
    //         sprintf(buff, "train/person_%d_%d.pgm", i + 1, j + 1);

    //         int **image = read_pgm_file("../CS426_Project3_files/images/1.1.txt", IMAGE_HEIGHT, IMAGE_WIDTH);

    //         create_histogram(image, train_histograms[i]);
    //         dealloc_2d_matrix(image, IMAGE_HEIGHT, IMAGE_WIDTH);
    //     }
    // }
    // Read training images and compute histograms
    for (int i = 0; i < people_count; ++i)
    {
        for (int j = 0; j < sample_count_per_person - k; ++j)
        {
            char image_path[256];
            sprintf(image_path, "../CS426_Project3_files/images/%d.%d.txt", i + 1, j + 1);
            int **image = read_pgm_file(image_path, IMAGE_HEIGHT, IMAGE_WIDTH);

            if (!image)
            {
                printf("ERROR: file open failed for %s\n", image_path);
                return -1;
            }

            create_histogram(image, train_histograms[i]);
            dealloc_2d_matrix(image, IMAGE_HEIGHT, IMAGE_WIDTH);
        }
    }

    // Dummy recognition (replace with actual matching)
    uint8_t found_people_array[5][8]; // Example sizes
    for (int i = 0; i < people_count; ++i)
    {
        for (int j = 0; j < sample_count_per_person - k; ++j)
        {
            int found_person_id = i; // dummy match
            found_people_array[i][j] = (uint8_t)found_person_id;
        }
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     ((end.tv_usec - start.tv_usec) / 1000000.0);
    printf("Sequential LBP completed in %.3f seconds.\n", elapsed);

    // 🧩 Display the histogram for verification (optional debug)
    printf("\n--- Sample Histogram for Person 1 ---\n");
    for (int i = 0; i < 256; i++)
    {
        printf("%d ", train_histograms[0][i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    for (int i = 0; i < people_count; i++)
        free(train_histograms[i]);
    free(train_histograms);

    return 0;
}
