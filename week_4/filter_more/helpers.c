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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE duplicate[height][width];
    float red_gx, red_gy, green_gx, green_gy, blue_gx, blue_gy;
    int sobel_red, sobel_green, sobel_blue, gx_kernel[3][3], gy_kernel[3][3];

    // Sobel X kernel
    gx_kernel[0][0] = -1;
    gx_kernel[0][1] = 0;
    gx_kernel[0][2] = 1;
    gx_kernel[1][0] = -2;
    gx_kernel[1][1] = 0;
    gx_kernel[1][2] = 2;
    gx_kernel[2][0] = -1;
    gx_kernel[2][1] = 0;
    gx_kernel[2][2] = 1;

    // Sobel Y kernel
    gy_kernel[0][0] = -1;
    gy_kernel[0][1] = -2;
    gy_kernel[0][2] = -1;
    gy_kernel[1][0] = 0;
    gy_kernel[1][1] = 0;
    gy_kernel[1][2] = 0;
    gy_kernel[2][0] = 1;
    gy_kernel[2][1] = 2;
    gy_kernel[2][2] = 1;

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
            // Reset kernel values
            red_gx = 0;
            red_gy = 0;
            green_gx = 0;
            green_gy = 0;
            blue_gx = 0;
            blue_gy = 0;

            // Set iteration bounds
            int x_min = j == 0          ? 0 : -1;
            int x_max = j == width - 1  ? 0 : 1;
            int y_min = i == 0          ? 0 : -1;
            int y_max = i == height - 1 ? 0 : 1;

            // Loop through pixels around
            for (int x = x_min; x <= x_max; x++)
            {
                for (int y = y_min; y <= y_max; y++)
                {
                    red_gx += duplicate[i + y][j + x].rgbtRed * gx_kernel[x + 1][y + 1];
                    red_gy += duplicate[i + y][j + x].rgbtRed * gy_kernel[x + 1][y + 1];
                    green_gx += duplicate[i + y][j + x].rgbtGreen * gx_kernel[x + 1][y + 1];
                    green_gy += duplicate[i + y][j + x].rgbtGreen * gy_kernel[x + 1][y + 1];
                    blue_gx += duplicate[i + y][j + x].rgbtBlue * gx_kernel[x + 1][y + 1];
                    blue_gy += duplicate[i + y][j + x].rgbtBlue * gy_kernel[x + 1][y + 1];
                }
            }

            // Calculate sobel value for each color channel
            sobel_red = ccap(sqrt(pow(red_gx, 2) + pow(red_gy, 2)));
            sobel_green = ccap(sqrt(pow(green_gx, 2) + pow(green_gy, 2)));
            sobel_blue = ccap(sqrt(pow(blue_gx, 2) + pow(blue_gy, 2)));

            // Assign new values
            image[i][j].rgbtRed = sobel_red;
            image[i][j].rgbtGreen = sobel_green;
            image[i][j].rgbtBlue = sobel_blue;
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
    if (color_value < 0)
    {
        return 0;
    }

    return round(color_value + 0.01);
}
