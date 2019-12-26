#include <cs50.h>
#include <stdio.h>

bool validate_amex(long number);
bool validate_mastercard(long number);
bool validate_visa(long number);

int main(void)
{
    long card_number;

    // Ensure the user enter a positive card number
    do
    {
        card_number = get_long("Type a credit card number: ");
    }
    while (card_number <= 0);

    if (validate_amex(card_number))
    {
        printf("AMEX\n");
    }
    else if (validate_mastercard(card_number))
    {
        printf("MASTERCARD\n");
    }
    else if (validate_visa(card_number))
    {
        printf("VISA\n");
    }

    // If no format is matched, print invalid
    else
    {
        printf("INVALID\n");
    }
}

// Count digits in a number
int get_long_length(long value)
{
    int length = 1;

    // For each house in a number, increase the digits
    // Each house goes to a maximum of 9
    while (value > 9)
    {
        length++;
        value /= 10;
    }

    return length;
}

// If position is 0, it will get last digit
int get_digit_reverse(long number, int position)
{
    // Move pointer from right to left until it points to the asked position
    for (int i = 0; i < position; i++)
    {
        number /= 10;
    }

    return number % 10;
}

// If position is 0, it will get first digit
int get_digit(long number, int position)
{
    return get_digit_reverse(number, get_long_length(number) - 1 - position);
}

int sum_number_digits(int number)
{
    int length = get_long_length(number);
    int accumulator = 0;

    // Sum each digit with the accumulator variable
    for (int i = 0; i < length; i++)
    {
        accumulator += get_digit_reverse(number, i);
    }

    return accumulator;
}

bool number_begins_with(long number, int begin)
{
    // Compare the first digits of a number with those of another number
    for (int i = 0; i < get_long_length(begin); i++)
    {
        if (get_digit(number, i) != get_digit(begin, i))
        {
            return false;
        }
    }

    return true;
}

bool validate_luhn(long card_number)
{
    int length = get_long_length(card_number);
    int even_sum = 0;
    int odd_sum = 0;
    long number_chunk = card_number;

    // Read every digit starting from last
    for (int i = 0; i < length; i++)
    {
        int n = number_chunk % 10;
        if (i % 2 == 0)
        {
            even_sum += n;
        }
        else
        {
            // Multiply and sum the digits of every 2 digits starting from second-to-last
            odd_sum += sum_number_digits(n * 2);
        }

        // Remove the last digit of the given number so it can be read on next iteration
        number_chunk /= 10;
    }

    int total_sum = even_sum + odd_sum;
    int last_sum_digit = get_digit_reverse(total_sum, 0);
    if (last_sum_digit == 0)
    {
        return true;
    }

    return false;
}

bool validate_amex(long number)
{
    // Check if it matches American Express conditions
    if (
        get_long_length(number) == 15
        && (number_begins_with(number, 34) || number_begins_with(number, 37))
        && validate_luhn(number)
    )
    {
        return true;
    }

    return false;
}

bool validate_mastercard(long number)
{
    // Check if it matches MasterCard conditions
    if (
        get_long_length(number) == 16
        && (
            number_begins_with(number, 51)
            || number_begins_with(number, 52)
            || number_begins_with(number, 53)
            || number_begins_with(number, 54)
            || number_begins_with(number, 55)
        )
        && validate_luhn(number)
    )
    {
        return true;
    }

    return false;
}

bool validate_visa(long number)
{
    // Check if it matches Visa conditions
    if (
        (get_long_length(number) == 13 || get_long_length(number) == 16)
        && number_begins_with(number, 4)
        && validate_luhn(number)
    )
    {
        return true;
    }

    return false;
}
