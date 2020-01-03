#include "helpers.h"
#include "math.h"

int ccap(float color_value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get average value between all color channels
            int avg = ccap((float)(image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_red = image[i][j].rgbtRed;
            int original_green = image[i][j].rgbtGreen;
            int original_blue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = ccap(0.393 * original_red + 0.769 * original_green + 0.189 * original_blue);
            image[i][j].rgbtGreen = ccap(0.349 * original_red + 0.686 * original_green + 0.168 * original_blue);
            image[i][j].rgbtBlue = ccap(0.272 * original_red + 0.534 * original_green + 0.131 * original_blue);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE duplicate[height][width];

    // Duplicate the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            duplicate[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Swap the j'th column with its "cross-axis" equivalent
            image[i][j] = duplicate[i][width - 1 - j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE duplicate[height][width], adjacent;
    float avg_red, avg_green, avg_blue, pixels_around;

    // Duplicate the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            duplicate[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg_red = 0;
            avg_green = 0;
            avg_blue = 0;
            pixels_around = 0;

            adjacent.rgbtRed = 0;
            adjacent.rgbtGreen = 0;
            adjacent.rgbtBlue = 0;

            int x_min = j == 0          ? 0 : -1;
            int x_max = j == width - 1  ? 0 : 1;
            int y_min = i == 0          ? 0 : -1;
            int y_max = i == height - 1 ? 0 : 1;

            for (int x = x_min; x <= x_max; x++)
            {
                for (int y = y_min; y <= y_max; y++)
                {
                    // Sum all the values of each color around
                    avg_red += duplicate[i + y][j + x].rgbtRed;
                    avg_green += duplicate[i + y][j + x].rgbtGreen;
                    avg_blue += duplicate[i + y][j + x].rgbtBlue;

                    // Count number of close pixels
                    pixels_around++;
                }
            }

            // Get the average of each color around
            avg_red = ccap(avg_red / pixels_around);
            avg_green = ccap(avg_green / pixels_around);
            avg_blue = ccap(avg_blue / pixels_around);

            // Assign new values
            image[i][j].rgbtRed = avg_red;
            image[i][j].rgbtGreen = avg_green;
            image[i][j].rgbtBlue = avg_blue;
        }
    }
}

// Round color value and prevent exceding limit
int ccap(float color_value)
{
    if (color_value > 255)
    {
        return 255;
    }

    return round(color_value + 0.01);
}
