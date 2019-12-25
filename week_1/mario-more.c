#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Prevent users from entering any number except the ones between 1 and 8, inclusive
    do
    {
        height = get_int("Height: ");
    } while (height < 1 || height > 8);

    // The loops builds the two-dimensional artboard
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // This condition checks to see what character it should place at this given "j"
            // The -1 is there because the height starts from 1 and loops starts from 0
            if (j >= height - i - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }

        // Prints the space between pyramids
        printf("  ");

        // Prints the another pyramid
        for (int j = 0; j < height; j++)
        {
            // In this pyramid, the number of blocks in a row is equal to the position of the column
            // As it's aligned on the left, there is no need to insert whitespaces after the blocks
            if (j <= i)
            {
                printf("#");
            }
        }

        printf("\n");
    }
}
