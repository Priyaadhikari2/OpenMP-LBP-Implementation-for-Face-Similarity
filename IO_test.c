//
// Created by egebasturk on 4/28/19.
//
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_HEIGHT 128
#define IMAGE_WIDTH 128

int main()
{
    int flag = 15;

    if (flag == 15)
    {
        int **image = read_pgm_file("../CS426_Project3_files/images/1.1.txt", IMAGE_HEIGHT, IMAGE_WIDTH);

        if (image == NULL)
        {
            fprintf(stderr, "Failed to read image file.\n");
            return 1;
        }

        FILE *fptr = fopen("mat.out", "w");
        if (fptr == NULL)
        {
            perror("Error opening output file");
            dealloc_2d_matrix(image, IMAGE_HEIGHT, IMAGE_WIDTH);
            return 1;
        }

        for (int i = 0; i < IMAGE_HEIGHT; ++i)
        {
            for (int j = 0; j < IMAGE_WIDTH; ++j)
            {
                fprintf(fptr, "%d ", image[i][j]);
            }
            fprintf(fptr, "\n");
        }

        fclose(fptr);
        dealloc_2d_matrix(image, IMAGE_HEIGHT, IMAGE_WIDTH);

        printf("✅ Matrix written successfully to mat.out\n");
    }

    return 0;
}
