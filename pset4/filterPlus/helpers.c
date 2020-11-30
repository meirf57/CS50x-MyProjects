#include "helpers.h"
# include <math.h>
# include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float R = image[h][w].rgbtRed;
            float G = image[h][w].rgbtGreen;
            float B = image[h][w].rgbtBlue;
            int RGB = round((R + G + B) / 3);
            image[h][w].rgbtRed = RGB;
            image[h][w].rgbtGreen = RGB;
            image[h][w].rgbtBlue = RGB;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int R = round(.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue);
            if (R > 255)
            {
                R = 255;
            }
            int G = round(.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue);
            if (G > 255)
            {
                G = 255;
            }
            int B = round(.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue);
            if (B > 255)
            {
                B = 255;
            }
            image[h][w].rgbtRed = R;
            image[h][w].rgbtGreen = G;
            image[h][w].rgbtBlue = B;
        }
    }
    return;
}

void swap(RGBTRIPLE **x, RGBTRIPLE **y);
// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        int w1 = width - 1;
        for (int w = 0; w < round(width / 2); w++)
        {
            RGBTRIPLE *a = &image[h][w];
            RGBTRIPLE *b = &image[h][w1];

            swap(&a, &b);

            image[h][w] = *a;
            image[h][w1] = *b;

            w1--;
        }
    }
    return;
}

void swap(RGBTRIPLE **x, RGBTRIPLE **y)
{
    RGBTRIPLE z = **x;
    **x = **y;
    **y = z;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pic[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            double r1 = image[h][w].rgbtRed;
            double g1 = image[h][w].rgbtGreen;
            double b1 = image[h][w].rgbtBlue; // box 5

            int x = 1;

            if (h > 0 && w > 0)
            {
                r1 = r1 + image[h - 1][w - 1].rgbtRed;
                g1 = g1 + image[h - 1][w - 1].rgbtGreen;
                b1 = b1 + image[h - 1][w - 1].rgbtBlue; // box 1
                x++;
            }

            if (w > 0)
            {
                r1 = r1 + image[h][w - 1].rgbtRed;
                g1 = g1 + image[h][w - 1].rgbtGreen;
                b1 = b1 + image[h][w - 1].rgbtBlue; // box 4
                x++;
            }

            if (h > 0)
            {
                r1 = r1 + image[h - 1][w].rgbtRed;
                g1 = g1 + image[h - 1][w].rgbtGreen;
                b1 = b1 + image[h - 1][w].rgbtBlue; // box 2
                x++;
            }

            if (w < width - 1 && h > 0)
            {
                r1 = r1 + image[h - 1][w + 1].rgbtRed;
                g1 = g1 + image[h - 1][w + 1].rgbtGreen;
                b1 = b1 + image[h - 1][w + 1].rgbtBlue; // box 3
                x++;
            }

            if (w < width - 1)
            {
                r1 = r1 + image[h][w + 1].rgbtRed;
                g1 = g1 + image[h][w + 1].rgbtGreen;
                b1 = b1 + image[h][w + 1].rgbtBlue; // box 6
                x++;
            }

            if (h < height - 1 && w > 0)
            {
                r1 = r1 + image[h + 1][w - 1].rgbtRed;
                g1 = g1 + image[h + 1][w - 1].rgbtGreen;
                b1 = b1 + image[h + 1][w - 1].rgbtBlue; // box 7
                x++;
            }

            if (h < height - 1)
            {
                r1 = r1 + image[h + 1][w].rgbtRed;
                g1 = g1 + image[h + 1][w].rgbtGreen;
                b1 = b1 + image[h + 1][w].rgbtBlue; // box 8
                x++;
            }

            if (h < height - 1 && w < width - 1)
            {
                r1 = r1 + image[h + 1][w + 1].rgbtRed;
                g1 = g1 + image[h + 1][w + 1].rgbtGreen;
                b1 = b1 + image[h + 1][w + 1].rgbtBlue; // box 9
                x++;
            }

            int R = round(r1 / x);
            int G = round(g1 / x);
            int B = round(b1 / x);

            if (R > 255)
            {
                R = 255;
            }
            if (G > 255)
            {
                G = 255;
            }
            if (B > 255)
            {
                B = 255;
            }

            pic[h][w].rgbtRed = R;
            pic[h][w].rgbtGreen = G;
            pic[h][w].rgbtBlue = B;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = pic[i][j];
        }
    }
    return;
}