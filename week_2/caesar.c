#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool is_numeric(string n);
string encipher(string input, int key);

int main(int argc, string argv[])
{
    if (argc != 2 || !is_numeric(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);
    string plaintext = get_string("plaintext:  ");
    printf("ciphertext: %s\n", encipher(plaintext, key));
}

bool is_numeric(string n)
{
    for (int i = 0; n[i] != '\0'; i++)
    {
        if (!isdigit(n[i]))
        {
            return false;
        }
    }

    return true;
}

string encipher(string plain, int key)
{
    // Duplicate original plaintext.
    string ciphertext = strdup(plain);

    // Shift each letter by an rotation amount.
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        if (isalpha(plain[i]))
        {
            // Track if the alphabet range is uppercase or lowercase.
            char alphabet_start;

            if (isupper(plain[i]))
            {
                alphabet_start = 'A';
            }
            else
            {
                alphabet_start = 'a';
            }

            // Convert ASCII to alphabetical index.
            // Ex.: 'A' will be 0 instead of 65, b = 1, c = 2, and so forth...
            int alphabetical_char = plain[i] - alphabet_start;

            // Rotate through alphabet range.
            int shifted_char = (alphabetical_char + key) % 26;

            // Convert result back to ASCII.
            ciphertext[i] = shifted_char + alphabet_start;
        }
    }

    return ciphertext;
}
