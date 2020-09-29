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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pic[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int r5 = image[h][w].rgbtRed;
            int g5 = image[h][w].rgbtGreen;
            int b5 = image[h][w].rgbtBlue; // box 5

            int r1 = 0;
            int g1 = 0;
            int b1 = 0;

            if (h > 0 && w > 0)
            {
                r1 = image[h - 1][w - 1].rgbtRed;
                g1 = image[h - 1][w - 1].rgbtGreen;
                b1 = image[h - 1][w - 1].rgbtBlue; // box 1
            }

            int r4 = 0;
            int g4 = 0;
            int b4 = 0;

            if (w > 0)
            {
                r4 = image[h][w - 1].rgbtRed;
                g4 = image[h][w - 1].rgbtGreen;
                b4 = image[h][w - 1].rgbtBlue; // box 4
            }

            int r2 = 0;
            int g2 = 0;
            int b2 = 0;

            if (h > 0)
            {
                r2 = image[h - 1][w].rgbtRed;
                g2 = image[h - 1][w].rgbtGreen;
                b2 = image[h - 1][w].rgbtBlue; // box 2
            }

            int r3 = 0;
            int g3 = 0;
            int b3 = 0;

            if (w < width - 1 && h > 0)
            {
                r3 = image[h - 1][w + 1].rgbtRed;
                g3 = image[h - 1][w + 1].rgbtGreen;
                b3 = image[h - 1][w + 1].rgbtBlue; // box 3
            }

            int r6 = 0;
            int g6 = 0;
            int b6 = 0;

            if (w < width - 1)
            {
                r6 = image[h][w + 1].rgbtRed;
                g6 = image[h][w + 1].rgbtGreen;
                b6 = image[h][w + 1].rgbtBlue; // box 6
            }

            int r7 = 0;
            int g7 = 0;
            int b7 = 0;

            if (h < height - 1 && w > 0)
            {
                r7 = image[h + 1][w - 1].rgbtRed;
                g7 = image[h + 1][w - 1].rgbtGreen;
                b7 = image[h + 1][w - 1].rgbtBlue; // box 7
            }

            int r8 = 0;
            int g8 = 0;
            int b8 = 0;

            if (h < height - 1)
            {
                r8 = image[h + 1][w].rgbtRed;
                g8 = image[h + 1][w].rgbtGreen;
                b8 = image[h + 1][w].rgbtBlue; // box 8
            }

            int r9 = 0;
            int g9 = 0;
            int b9 = 0;

            if (h < height - 1 && w < width - 1)
            {
                r9 = image[h + 1][w + 1].rgbtRed;
                g9 = image[h + 1][w + 1].rgbtGreen;
                b9 = image[h + 1][w + 1].rgbtBlue; // box 9
            }

            int Gxr1 = -1 * r1 + 0 * r2 + 1 * r3;
            int Gxr2 = -2 * r4 + 0 * r5 + 2 * r6;
            int Gxr3 = -1 * r7 + 0 * r8 + 1 * r9;
            int GxR = Gxr1 + Gxr2 + Gxr3;
            double Gxr = GxR * GxR;

            int Gxg1 = -1 * g1 + 0 * g2 + 1 * g3;
            int Gxg2 = -2 * g4 + 0 * g5 + 2 * g6;
            int Gxg3 = -1 * g7 + 0 * g8 + 1 * g9;
            int GxG = Gxg1 + Gxg2 + Gxg3;
            double Gxg = GxG * GxG;

            int Gxb1 = -1 * b1 + 0 * b2 + 1 * b3;
            int Gxb2 = -2 * b4 + 0 * b5 + 2 * b6;
            int Gxb3 = -1 * b7 + 0 * b8 + 1 * b9;
            int GxB = Gxb1 + Gxb2 + Gxb3;
            double Gxb = GxB * GxB;

            int Gyr1 = -1 * r1 + -2 * r2 + -1 * r3;
            int Gyr2 = 0 * r4 + 0 * r5 + 0 * r6;
            int Gyr3 = 1 * r7 + 2 * r8 + 1 * r9;
            int GyR = Gyr1 + Gyr2 + Gyr3;
            double Gyr = GyR * GyR;

            int Gyg1 = -1 * g1 + -2 * g2 + -1 * g3;
            int Gyg2 = 0 * g4 + 0 * g5 + 0 * g6;
            int Gyg3 = 1 * g7 + 2 * g8 + 1 * g9;
            int GyG = Gyg1 + Gyg2 + Gyg3;
            double Gyg = GyG * GyG;

            int Gyb1 = -1 * b1 + -2 * b2 + -1 * b3;
            int Gyb2 = 0 * b4 + 0 * b5 + 0 * b6;
            int Gyb3 = 1 * b7 + 2 * b8 + 1 * b9;
            int GyB = Gyb1 + Gyb2 + Gyb3;
            double Gyb = GyB * GyB;

            double R1 = Gxr + Gyr;
            double G1 = Gxg + Gyg;
            double B1 = Gxb + Gyb;

            int R = round(sqrt(R1));
            int G = round(sqrt(G1));
            int B = round(sqrt(B1));

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