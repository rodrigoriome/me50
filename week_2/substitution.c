#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string encipher(string plain, string key);
int validate_key(string value);
string get_err_msg(int err_code);

int main(int argc, string argv[])
{
    // Validate arguments count
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Get the key
    string key = argv[1];

    // Validate the key
    int err_code = validate_key(key);
    if (err_code != 0)
    {
        printf("%s\n", get_err_msg(err_code));
        return 1;
    }

    // Get plaintext
    string plaintext = get_string("plaintext:  ");

    // Encipher
    string cipher = encipher(plaintext, key);

    // Print ciphertext
    printf("ciphertext: %s\n", cipher);
}

string encipher(string plain, string key)
{
    // By default, the ciphertext is a duplicate of the plaintext.
    string ciphertext = strdup(plain);

    for (int i = 0, n = strlen(ciphertext); i < n; i++)
    {
        if (isalpha(ciphertext[i]))
        {
            // For sake of comparison, convert character to lowercase.
            char current_char = tolower(ciphertext[i]);

            // Convert ASCII to alphabetical index to map the cipher key correctly.
            char shifted_char = key[current_char - 'a'];
            ciphertext[i] = shifted_char;

            // Recover original letter casing if necessary.
            if (isupper(plain[i]))
            {
                ciphertext[i] = toupper(ciphertext[i]);
            }
        }
    }

    return ciphertext;
}

int validate_key(string key)
{
    // Track characters that has already been used.
    bool used_positions[26] = { false };

    // Check if the key has the right length.
    if (strlen(key) != 26)
    {
        return 1;
    }

    // Loop through the key only if it has 26 characters.
    for (int i = 0; i < 26; i++)
    {
        // For sake of validation, convert key to lowercase.
        key[i] = tolower(key[i]);

        // Check if character is non-alphabetical.
        if (! isalpha(key[i]))
        {
            return 2;
        }

        // Character's alphabetic index
        int alpha_index = key[i] - 'a';

        // Check if character has already been used before.
        if (used_positions[alpha_index] == true)
        {
            return 3;
        }

        // Mark character as used.
        used_positions[alpha_index] = true;
    }

    // If no errors were found, return 0.
    return 0;
}

// Get the respective message for an error code.
string get_err_msg(int err_code)
{
    if (err_code == 1)
    {
        return "Key must contain 26 characters.";
    }
    else if (err_code == 2)
    {
        return "Key must only contain alphabetic characters.";
    }
    else if (err_code == 3)
    {
        return "Key must not contain repeated characters.";
    }
    else
    {
        return "No errors where found.";
    }
}
