#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <cs50.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            int average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
}

// New function for reflect which swaps 2 pixels
void swapPixel(RGBTRIPLE *source, RGBTRIPLE *dest)
{
    RGBTRIPLE temp = *source;
    *source = *dest;
    *dest = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width / 2 ; j++)
        {
            swapPixel(&image[i][j], &image[i][width - 1 - j]);
        }
    }
}

// Returns whether the pixel can be used for blur calculation
bool is_valid_pixel(int i, int j, int height, int width)
{
    return i >= 0 && i < height && j >= 0 && j < width;
}

// Creates new RGB-values for blurred pixel
RGBTRIPLE get_blurred_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int totalRed = 0, totalGreen = 0, totalBlue = 0;
    int counter = 0;
    RGBTRIPLE output;

    for (int x = -1 ; x <= 1 ; x++)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            if (is_valid_pixel(i + x, j + y, height, width))
            {
                totalRed += image[i + x][j + y].rgbtRed;
                totalGreen += image[i + x][j + y].rgbtGreen;
                totalBlue += image[i + x][j + y].rgbtBlue;
                counter++;
            }
        }
    }
    output.rgbtRed = round((float) totalRed / counter);
    output.rgbtGreen = round((float) totalGreen / counter);
    output.rgbtBlue = round((float) totalBlue / counter);

    return output;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy to store blurred pixels
    RGBTRIPLE imageCopy[height][width];

    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            imageCopy[i][j] = get_blurred_pixel(i, j, height, width, image);
        }
    }

    // Copy temporary imageCopy into output file
    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            image[i][j] = imageCopy[i][j];
        }
    }
}

// function which caps RBG-value
int capValue(int value)
{
    return value > 255 ? 255 : value;
}

// Returns pixel after calculating through Gx- and Gy kernel
RGBTRIPLE get_edge_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int totalGxRed, totalGxGreen, totalGxBlue, totalGyRed, totalGyGreen, totalGyBlue;
    totalGxRed = totalGxGreen = totalGxBlue = totalGyRed = totalGyGreen = totalGyBlue = 0;
    int gx_kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}; //can be used for both Gx and Gy kernel
    RGBTRIPLE output;

    for (int x = -1 ; x <= 1 ; x++)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            if (is_valid_pixel(i + x, j + y, height, width))
            {
                int gx_weight = gx_kernel[x + 1][y + 1];
                int gy_weight = gx_kernel[y + 1][x + 1];
                totalGxRed += gx_weight * image[i + x][j + y].rgbtRed;
                totalGxGreen += gx_weight * image[i + x][j + y].rgbtGreen;
                totalGxBlue += gx_weight * image[i + x][j + y].rgbtBlue;

                totalGyRed += gy_weight * image[i + x][j + y].rgbtRed;
                totalGyGreen += gy_weight * image[i + x][j + y].rgbtGreen;
                totalGyBlue += gy_weight * image[i + x][j + y].rgbtBlue;
            }
        }
    }
    output.rgbtRed = capValue(round(sqrt(totalGxRed * totalGxRed + totalGyRed * totalGyRed)));
    output.rgbtGreen = capValue(round(sqrt(totalGxGreen * totalGxGreen + totalGyGreen * totalGyGreen)));
    output.rgbtBlue = capValue(round(sqrt(totalGxBlue * totalGxBlue + totalGyBlue * totalGyBlue)));
    return output;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy to store edged pixels
    RGBTRIPLE imageCopy[height][width];

    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            imageCopy[i][j] = get_edge_pixel(i, j, height, width, image);
        }
    }

    // Copy temporary imageCopy into output file
    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            image[i][j] = imageCopy[i][j];
        }
    }
}
