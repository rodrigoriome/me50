#include <cs50.h>
#include <stdio.h>
#include <math.h>

bool is_letter(char ch);
bool is_punct(char ch);
float calculate_score(int letters, int words, int sentences);
int get_index(string input);

int main(void)
{
    // Require at least a character to increment words and prevent Division by Zero
    string text;
    do
    {
        text = get_string("Text: ");
    }
    while (! is_letter(text[0]));

    int index = get_index(text);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Check if is letter
bool is_letter(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// Check if is a period, exclamation point or question mark to delimit a sentence
bool is_punct(char ch)
{
    return ch == '.' || ch == '!' || ch == '?' || ch == ':';
}

// Calculate the readability score
float calculate_score(int letters, int words, int sentences)
{
    float L = (float)letters * 100 / words;
    float S = (float)sentences * 100 / words;

    // Coleman-Liau Index formula
    return 0.0588 * L - 0.296 * S - 15.8;
}

// Return the actual readability grade index
int get_index(string input)
{
    int letters = 0;
    int words = 0;
    int sentences = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        // Calculate sentences
        if (is_punct(input[i]))
        {
            sentences++;
        }

        // Calculate letters and words
        else if (is_letter(input[i]))
        {
            letters++;

            if (input[i - 1] == ' ' || input[i - 1] == '\0')
            {
                words++;
            }
        }
    }

    return rintf(calculate_score(letters, words, sentences));
}
