#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //convert each cell so that the sum of the avg colours in pixel is new output
    float average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //average = round((red + blue + green) / 3);
            average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.000);
            //each colour = average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //basically need to flip LH & RH pixels around

    RGBTRIPLE temp; //discovered that you can overwrite directly instead of for each colour

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++) //plan is to simply switch on LH side only
        {
            //store first number into temp
            temp = image[i][j];
            //make last number into first
            image[i][j] = image[i][width - j - 1];
            //reassign right side to temp values
            image[i][width - j - 1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //algorithm needs to look into each pixel, and take avg of pixels around
    //for each colour
    //note edge conditions should only function with cells that exist

    RGBTRIPLE copy[height][width]; //copy of original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int sumR; //tally for red
    int sumG; //tally for green
    int sumB; //tally for blue
    float count; //counter if valid spot

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //reset tallies
            sumR = 0;
            sumG = 0;
            sumB = 0;
            count = 0.00;

            //averaging algorithm if within picture
            for (int y = -1; y < 2; y++)
            {
                if (i + y < 0 || i + y > height - 1) //out of bounds element
                {
                    continue;
                }
                for (int x = -1; x < 2; x++)
                {
                    if (j + x < 0 || j + x > width - 1) // out of bounds element
                    {
                        continue;
                    }
                    sumR += copy[i + y][j + x].rgbtRed;
                    sumG += copy[i + y][j + x].rgbtGreen;
                    sumB += copy[i + y][j + x].rgbtBlue;
                    count++;
                }
            }

            //readjustment of original image
            image[i][j].rgbtRed = round(sumR / count);
            image[i][j].rgbtGreen = round(sumG / count);
            image[i][j].rgbtBlue = round(sumB / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //colours are different on left side of line are one value, other side quite dif
    //convolutional matrix
    //Gx = [(-1,0,1),(-2,0,2),(-1,0,1)]
    //Gy = [(-1,-2,-1),(0,0,0),(1,2,1)]
    //form a 3x3 matrix around a center pixel, analyze each colour
    //multiply the rows by rows and add the sum of the rows.
    //Sum of 0 means no bound, large number means boundary
    //Basically: compute Gx & Gy for each channel r, g, b
    //pixels beyond the border can be assumed as all 0 values
    //each new pixel will be computed as the square root of the sum of Gx^2 and Gy^2
    //remember to cap the value at 255

    //create a copy of the original matrix
    RGBTRIPLE copy[height + 2][width + 2]; //copy of original with borders

    for (int i = 0; i < height + 2; i++) //file scan and expand for border addition
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || j == 0 || i == height + 1 || j == width + 1)
            {
                copy[i][j].rgbtRed = 0;
                copy[i][j].rgbtGreen = 0;
                copy[i][j].rgbtBlue = 0;
            }
            else
            {
                copy[i][j] = image[i - 1][j - 1];
            }
        }
    }

    //calculate transformed values
    float valRx, valGx, valBx, valRy, valGy, valBy; //Gx and Gy matrix vals

    for (int i = 1; i < height + 1; i++) //now don't have to worry about border condition
    {
        for (int j = 1; j < width + 1; j++)
        {
            //reinitialize values, need to reinitialize float
            valRx = valGx = valBx = valRy = valGy = valBy = 0.0;
            float ssR, ssG, ssB;
            ssR = ssG = ssB = 0.0;
            //Gx & Gy matrices
            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    if (y == 0 || x == 0)
                    {
                        valRx += 2 * x * copy[i + y][j + x].rgbtRed;
                        valRy += 2 * y * copy[i + y][j + x].rgbtRed;
                        valGx += 2 * x * copy[i + y][j + x].rgbtGreen;
                        valGy += 2 * y * copy[i + y][j + x].rgbtGreen;
                        valBx += 2 * x * copy[i + y][j + x].rgbtBlue;
                        valBy += 2 * y * copy[i + y][j + x].rgbtBlue;
                    }
                    else
                    {
                        valRx += x * copy[i + y][j + x].rgbtRed;
                        valRy += y * copy[i + y][j + x].rgbtRed;
                        valGx += x * copy[i + y][j + x].rgbtGreen;
                        valGy += y * copy[i + y][j + x].rgbtGreen;
                        valBx += x * copy[i + y][j + x].rgbtBlue;
                        valBy += y * copy[i + y][j + x].rgbtBlue;
                    }
                }
            }
            //sum of squares
            ssR = round(sqrt(valRx * valRx + valRy * valRy));
            ssG = round(sqrt(valGx * valGx + valGy * valGy));
            ssB = round(sqrt(valBx * valBx + valBy * valBy));
            //cap check
            if (ssR > 255)
            {
                ssR = 255;
            }
            if (ssG > 255)
            {
                ssG = 255;
            }
            if (ssB > 255)
            {
                ssB = 255;
            }
            //computed output
            image[i - 1][j - 1].rgbtRed = (int)ssR;
            image[i - 1][j - 1].rgbtGreen = (int)ssG;
            image[i - 1][j - 1].rgbtBlue = (int)ssB;
        }
    }

    return;
}
