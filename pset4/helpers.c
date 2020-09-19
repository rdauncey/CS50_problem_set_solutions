#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

//Prototype for assisting fns
BYTE box_avg(int height, int width, RGBTRIPLE image[height][width], int row, int column, char colour);
BYTE get_new_value(int height, int width, RGBTRIPLE image[height][width], int row, int column, char colour);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //The RGB triple at pixel i,j is image[i][j]
            //Take the average and round
            int sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;


            BYTE avg = round((float) sum / 3);

            //Set the RGB values to avg
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Counting columns, count up to width / 2 rounded down
    for (int j = 0, n = floor(width / 2); j < n; j++)
    {
        //Switch the columns
        for (int i = 0; i < height; i++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //We will need to create a temp image to avoid using the new pixel values to
    //calculate other new pixel values
    RGBTRIPLE temp[height][width];
    memcpy(temp, image, sizeof(temp));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = box_avg(height, width, temp, i, j, 'b');
            image[i][j].rgbtGreen = box_avg(height, width, temp, i, j, 'g');
            image[i][j].rgbtRed = box_avg(height, width, temp, i, j, 'r');
        }
    }
}

//Average R, G or B components over a 3x3 block
BYTE box_avg(int height, int width, RGBTRIPLE image[height][width], int row, int column, char colour)
{
    //Check central index is valid
    if (row >= height || row < 0 || column >= width || column < 0)
    {
        return -1;
    }

    else
    {
        //Note sum must be an int as we will store numbers larger than 255 in it
        int sum = 0;
        int boxes = 9;

        //Iterate over the 3x3 box around (row, column), with switch determining which of RGB to sum
        switch (colour)
        {
            //Summing R components
            case 'r':
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, we don't count a box, so subtract 1 from the number of boxes summed
                        if ((row + i) < 0 || (row + i) >= height || (column + j) < 0 || (column + j) >= width)
                        {
                            boxes = boxes - 1;
                        }

                        else
                        {
                            sum += image[row + i][column + j].rgbtRed;
                        }
                    }
                }
                break;

            //Summing G components
            case 'g':
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, we don't count a box, so subtract 1 from the number of boxes summed
                        if (row + i < 0 || row + i >= height || column + j < 0 || column + j >= width)
                        {
                            boxes = boxes - 1;
                        }

                        else
                        {
                            sum += image[row + i][column + j].rgbtGreen;
                        }
                    }
                }
                break;

            //Summing B components
            case 'b':
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, we don't count a box, so subtract 1 from the number of boxes summed
                        if (row + i < 0 || row + i >= height || column + j < 0 || column + j >= width)
                        {
                            boxes = boxes - 1;
                        }

                        else
                        {
                            sum += image[row + i][column + j].rgbtBlue;
                        }
                    }
                }
                break;
        }
        //Use the sum and the number of boxes included in the sum to get the average (rounded)
        BYTE average = round((float) sum / boxes);
        return average;
    }
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //As in blur, we need a temp array
    RGBTRIPLE temp[height][width];
    memcpy(temp, image, sizeof(temp));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = get_new_value(height, width, temp, i, j, 'b');
            image[i][j].rgbtGreen = get_new_value(height, width, temp, i, j, 'g');
            image[i][j].rgbtRed = get_new_value(height, width, temp, i, j, 'r');
        }
    }
}

BYTE get_new_value(int height, int width, RGBTRIPLE image[height][width], int row, int column, char colour)
{
    //Check central index is valid
    if (row >= height || row < 0 || column >= width || column < 0)
    {
        return -1;
    }

    else
    {
        int Gx_sum = 0;
        int Gy_sum = 0;

        int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

        //Iterate over the 3x3 box around (row, column), with switch determining which of RGB to sum
        switch (colour)
        {
            //Summing R components
            case 'r':

                //Calculate Gx
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, assume there is a black pixel border
                        if ((row + i) < 0 || (row + i) >= height || (column + j) < 0 || (column + j) >= width)
                        {
                            Gx_sum += Gx[i + 1][j + 1] * 0x0;
                            Gy_sum += Gx[i + 1][j + 1] * 0x0;
                        }

                        else
                        {
                            //Multiply the pixel colour values by the corresponding kernel value
                            Gx_sum += Gx[i + 1][j + 1] * image[row + i][column + j].rgbtRed;
                            Gy_sum += Gy[i + 1][j + 1] * image[row + i][column + j].rgbtRed;
                        }
                    }
                }
                break;

            //Summing G components
            case 'g':
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, assume there is a black pixel border
                        if ((row + i) < 0 || (row + i) >= height || (column + j) < 0 || (column + j) >= width)
                        {
                            Gx_sum += Gx[i + 1][j + 1] * 0x0;
                            Gy_sum += Gx[i + 1][j + 1] * 0x0;
                        }

                        else
                        {
                            //Multiply the pixel colour values by the corresponding kernel value
                            Gx_sum += Gx[i + 1][j + 1] * image[row + i][column + j].rgbtGreen;
                            Gy_sum += Gy[i + 1][j + 1] * image[row + i][column + j].rgbtGreen;
                        }
                    }
                }
                break;

            //Summing B components
            case 'b':
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //If index out of range, assume there is a black pixel border
                        if ((row + i) < 0 || (row + i) >= height || (column + j) < 0 || (column + j) >= width)
                        {
                            Gx_sum += Gx[i + 1][j + 1] * 0x0;
                            Gy_sum += Gx[i + 1][j + 1] * 0x0;
                        }

                        else
                        {
                            //Multiply the pixel colour values by the corresponding kernel value
                            Gx_sum += Gx[i + 1][j + 1] * image[row + i][column + j].rgbtBlue;
                            Gy_sum += Gy[i + 1][j + 1] * image[row + i][column + j].rgbtBlue;
                        }
                    }
                }
                break;
        }

        //The pixel value is sqrt(Gx_sum^2 + Gy_sum^2)
        //Use the sum and the number of boxes included in the sum to get the average (rounded)
        int new_pixel_value = round(sqrt((float)(Gx_sum * Gx_sum) + (Gy_sum * Gy_sum)));

        //Make sure value is in fact between 0 and 255
        if (new_pixel_value > 255)
        {
            new_pixel_value = 255;
        }
        else if (new_pixel_value < 0)
        {
            new_pixel_value = 0;
        }

        return (BYTE) new_pixel_value;
    }
}
