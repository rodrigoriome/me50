#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float change;

    // Get a positive change
    do
    {
        change = get_float("What's the change?\n");
    } while (change < 0);

    // Convert dollars to cents
    int owed = round(change * 100);

    // Check what is the biggest coin possible to remove from what is owed until it reaches zero
    int coins = 0;
    while (owed > 0)
    {
        // Checks for quarters
        if (owed >= 25)
        {
            owed -= 25;
        }
        // Checks for dimes
        else if (owed >= 10)
        {
            owed -= 10;
        }
        // Checks for nickels
        else if (owed >= 5)
        {
            owed -= 5;
        }
        // Checks for remaining pennies
        else
        {
            owed -= 1;
        }

        // While this loop is running, it means the owed change hasn't reached 0 yet,
        // so it should add a new coin for each iteration
        coins++;
    }

    printf("%i\n", coins);
}
