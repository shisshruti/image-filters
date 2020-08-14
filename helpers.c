#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = (int) round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float) image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = (BYTE) avg;
            image[i][j].rgbtGreen = (BYTE) avg;
            image[i][j].rgbtRed = (BYTE) avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (int)(width / 2); j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redx = 0;
            int greenx = 0;
            int bluex = 0;
            int redy = 0;
            int greeny = 0;
            int bluey = 0;

            // loop through the pixels 1 row and 1 column from the pixel
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // consider pixels in edges
                    if (i - 1 + k < 0 || i - 1 + k > height - 1 || j - 1 + l < 0 || j - 1 + l > width - 1)
                    {
                        continue;
                    }
                    // calcuate Gx for each color
                    redx = redx + (image[i - 1 + k][j - 1 + l].rgbtRed * Gx[k][l]);
                    greenx = greenx + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gx[k][l]);
                    bluex = bluex + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gx[k][l]);

                    // calculate Gy for each color
                    redy = redy + (image[i - 1 + k][j - 1 + l].rgbtRed * Gy[k][l]);
                    greeny = greeny + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gy[k][l]);
                    bluey = bluey + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gy[k][l]);
                }
            }
            // calculate square root of Gx2 + Gy2 for each color
            float r = sqrt((redx * redx) + (redy * redy));
            float g = sqrt((greenx * greenx) + (greeny * greeny));
            float b = sqrt((bluex * bluex) + (bluey * bluey));
        
            // round to nearest integer and cap at 255
            int red =  round(r);
            int green = round(g);
            int blue = round(b);

            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
                
            // replace color values with new value
            copy[i][j].rgbtRed = red;
            copy[i][j].rgbtBlue = blue;
            copy[i][j].rgbtGreen = green;
        }
    }
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE avg[height][width];

    //blurring the centre pixels
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            avg[i][j].rgbtRed = (BYTE) round(((float) image[i][j].rgbtRed + (float) image[i][j - 1].rgbtRed +
                                              (float) image[i][j + 1].rgbtRed + (float) image[i - 1][j - 1].rgbtRed + (float) image[i - 1][j].rgbtRed +
                                              (float) image[i - 1][j + 1].rgbtRed + (float) image[i + 1][j - 1].rgbtRed + (float) image[i + 1][j].rgbtRed +
                                              (float) image[i + 1][j + 1].rgbtRed) / 9);

            avg[i][j].rgbtBlue = (BYTE) round(((float) image[i][j].rgbtBlue + (float) image[i][j - 1].rgbtBlue +
                                               (float) image[i][j + 1].rgbtBlue + (float) image[i - 1][j - 1].rgbtBlue + (float) image[i - 1][j].rgbtBlue +
                                               (float) image[i - 1][j + 1].rgbtBlue +
                                               (float) image[i + 1][j - 1].rgbtBlue + (float) image[i + 1][j].rgbtBlue + (float) image[i + 1][j + 1].rgbtBlue) / 9);

            avg[i][j].rgbtGreen = (BYTE) round(((float) image[i][j].rgbtGreen + (float) image[i][j - 1].rgbtGreen +
                                                (float) image[i][j + 1].rgbtGreen +
                                                (float) image[i - 1][j - 1].rgbtGreen + (float) image[i - 1][j].rgbtGreen + (float) image[i - 1][j + 1].rgbtGreen +
                                                (float) image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + (float) image[i + 1][j + 1].rgbtGreen) / 9);
        }
    }

    for (int j = 1; j < width - 1; j++)
    {
        //blurring the 0th row
        avg[0][j].rgbtRed = (BYTE) round(((float) image[0][j].rgbtRed + (float) image[0][j + 1].rgbtRed +
                                          (float) image[1][j].rgbtRed +
                                          (float) image[0][j - 1].rgbtRed + (float) image[1][j + 1].rgbtRed + (float) image[1][j - 1].rgbtRed) / 6);

        avg[0][j].rgbtBlue = (BYTE) round(((float) image[0][j].rgbtBlue + (float) image[0][j + 1].rgbtBlue +
                                           (float) image[0 + 1][j].rgbtBlue +
                                           (float) image[0][j - 1].rgbtBlue + (float) image[0 + 1][j + 1].rgbtBlue + (float) image[0 + 1][j - 1].rgbtBlue) / 6);

        avg[0][j].rgbtGreen = (BYTE) round(((float) image[0][j].rgbtGreen + (float) image[0][j + 1].rgbtGreen +
                                            (float) image[0 + 1][j].rgbtGreen +
                                            (float) image[0][j - 1].rgbtGreen + (float) image[0 + 1][j + 1].rgbtGreen + (float) image[0 + 1][j - 1].rgbtGreen) / 6);

        //blurring the last row
        avg[height - 1][j].rgbtRed = (BYTE) round(((float) image[height - 1][j].rgbtRed +
                                     (float) image[height - 1][j + 1].rgbtRed + (float) image[height - 2][j].rgbtRed +
                                     (float) image[height - 1][j - 1].rgbtRed + (float) image[height - 2][j + 1].rgbtRed +
                                     (float) image[height - 2][j - 1].rgbtRed) / 6);

        avg[height - 1][j].rgbtBlue = (BYTE) round(((float) image[height - 1][j].rgbtBlue +
                                      (float) image[height - 1][j + 1].rgbtBlue + (float) image[height - 2][j].rgbtBlue +
                                      (float) image[height - 1][j - 1].rgbtBlue + (float) image[height - 2][j + 1].rgbtBlue +
                                      (float) image[height - 2][j - 1].rgbtBlue) / 6);

        avg[height - 1][j].rgbtGreen = (BYTE) round(((float) image[height - 1][j].rgbtGreen +
                                       (float) image[height - 1][j + 1].rgbtGreen + (float) image[height - 2][j].rgbtGreen +
                                       (float) image[height - 1][j - 1].rgbtGreen + (float) image[height - 2][j + 1].rgbtGreen +
                                       (float) image[height - 2][j - 1].rgbtGreen) / 6);
    }


    for (int i = 1; i < height - 1; i++)
    {
        //blurring the 0th column
        avg[i][0].rgbtRed = (BYTE) round(((float) image[i][0].rgbtRed + (float) image[i + 1][0].rgbtRed +
                                          (float) image[i - 1][0].rgbtRed +
                                          (float) image[i + 1][1].rgbtRed + (float) image[i][1].rgbtRed + (float) image[i - 1][1].rgbtRed) / 6);

        avg[i][0].rgbtBlue = (BYTE) round(((float) image[i][0].rgbtBlue + (float) image[i + 1][0].rgbtBlue +
                                           (float) image[i - 1][0].rgbtBlue +
                                           (float) image[i + 1][1].rgbtBlue + (float) image[i][1].rgbtBlue + (float) image[i - 1][1].rgbtBlue) / 6);

        avg[i][0].rgbtGreen = (BYTE) round(((float) image[i][0].rgbtGreen + (float) image[i + 1][0].rgbtGreen +
                                            (float) image[i - 1][0].rgbtGreen +
                                            (float) image[i + 1][1].rgbtGreen + (float) image[i][1].rgbtGreen + (float) image[i - 1][1].rgbtGreen) / 6);

        //blurring the (width - 1)th column
        avg[i][width - 1].rgbtRed = (BYTE) round(((float) image[i][width - 1].rgbtRed +
                                    (float) image[i + 1][width - 1].rgbtRed +
                                    (float) image[i - 1][width - 1].rgbtRed + (float) image[i + 1][width - 2].rgbtRed +
                                    (float) image[i][width - 2].rgbtRed + (float) image[i - 1][width - 2].rgbtRed) / 6);

        avg[i][width - 1].rgbtBlue = (BYTE) round(((float) image[i][width - 1].rgbtBlue +
                                     (float) image[i + 1][width - 1].rgbtBlue +
                                     (float) image[i - 1][width - 1].rgbtBlue + (float) image[i + 1][width - 2].rgbtBlue +
                                     (float) image[i][width - 2].rgbtBlue + (float) image[i - 1][width - 2].rgbtBlue) / 6);

        avg[i][width - 1].rgbtGreen = (BYTE) round(((float) image[i][width - 1].rgbtGreen +
                                      (float) image[i + 1][width - 1].rgbtGreen + (float) image[i - 1][width - 1].rgbtGreen +
                                      (float) image[i + 1][width - 2].rgbtGreen + (float) image[i][width - 2].rgbtGreen +
                                      (float) image[i - 1][width - 2].rgbtGreen) / 6);
    }

    //blurring the pixel at (0,0)
    avg[0][0].rgbtRed = (BYTE) round(((float) image[0][0].rgbtRed + (float) image[0][0 + 1].rgbtRed +
                                      (float) image[0 + 1][0].rgbtRed + (float) image[1][0 + 1].rgbtRed) / 4);

    avg[0][0].rgbtBlue = (BYTE) round(((float) image[0][0].rgbtBlue + (float) image[0][0 + 1].rgbtBlue +
                                       (float) image[0 + 1][0].rgbtBlue + (float) image[1][0 + 1].rgbtBlue) / 4);

    avg[0][0].rgbtGreen = (BYTE) round(((float) image[0][0].rgbtGreen + (float) image[0][0 + 1].rgbtGreen +
                                        (float) image[0 + 1][0].rgbtGreen + (float) image[1][0 + 1].rgbtGreen) / 4);

    //blurring (0, width - 1)
    avg[0][width - 1].rgbtRed = (BYTE) round(((float) image[0][width - 1].rgbtRed + (float) image[0][width - 2].rgbtRed +
                                (float) image[1][width - 1].rgbtRed + (float) image[1][width - 2].rgbtRed) / 4);

    avg[0][width - 1].rgbtBlue = (BYTE) round(((float) image[0][width - 1].rgbtBlue + (float) image[0][width - 2].rgbtBlue +
                                 (float) image[1][width - 1].rgbtBlue + (float) image[1][width - 2].rgbtBlue) / 4);

    avg[0][width - 1].rgbtGreen = (BYTE) round(((float) image[0][width - 1].rgbtGreen + (float) image[0][width - 2].rgbtGreen +
                                  (float) image[1][width - 1].rgbtGreen + (float) image[1][width - 2].rgbtGreen) / 4);

    //blurring (height - 1, 0)
    avg[height - 1][0].rgbtRed = (BYTE) round(((float) image[height - 1][0].rgbtRed + (float) image[height - 2][0].rgbtRed +
                                 (float) image[height - 1][1].rgbtRed + (float) image[height - 2][1].rgbtRed) / 4);

    avg[height - 1][0].rgbtBlue = (BYTE) round(((float) image[height - 1][0].rgbtBlue + (float) image[height - 2][0].rgbtBlue +
                                  (float) image[height - 1][1].rgbtBlue + (float) image[height - 2][1].rgbtBlue) / 4);
    avg[height - 1][0].rgbtGreen = (BYTE) round(((float) image[height - 1][0].rgbtGreen + (float) image[height - 2][0].rgbtGreen +
                                   (float) image[height - 1][1].rgbtGreen + (float) image[height - 2][1].rgbtGreen) / 4);

    //blurring (heigh - 1, width - 1)
    avg[height - 1][width - 1].rgbtRed = (BYTE) round(((float) image[height - 1][width - 1].rgbtRed +
                                         (float) image[height - 1][width - 2].rgbtRed +
                                         (float) image[height - 2][width - 1].rgbtRed + (float) image[height - 2][width - 2].rgbtRed) / 4);

    avg[height - 1][width - 1].rgbtBlue = (BYTE) round(((float) image[height - 1][width - 1].rgbtBlue +
                                          (float) image[height - 1][width - 2].rgbtBlue +
                                          (float) image[height - 2][width - 1].rgbtBlue + (float) image[height - 2][width - 2].rgbtBlue) / 4);
    avg[height - 1][width - 1].rgbtGreen = (BYTE) round(((float) image[height - 1][width - 1].rgbtGreen +
                                           (float) image[height - 1][width - 2].rgbtGreen +
                                           (float) image[height - 2][width - 1].rgbtGreen + (float) image[height - 2][width - 2].rgbtGreen) / 4);

    //updating the image with the blurred values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = avg[i][j];
        }
    }

    return;
}