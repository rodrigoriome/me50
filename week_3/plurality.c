#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // In this example, the candidates array is unsorted. Therefore, BINARY SEARCH IS NOT POSSIBLE.

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;

            return true;
        }
    }

    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Count winners and their indexes. It should allow no winners as all votes could be invalid.
    int winners = 0;

    // It should allow everybody winning if all candidates have same amount of votes.
    int winners_indexes[MAX] = { 0 };

    // Loop through candidates.
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate doesn't have any votes, no need to update totals.
        if (candidates[i].votes > 0)
        {
            if (candidates[i].votes == candidates[winners_indexes[0]].votes)
            {
                winners++;
                winners_indexes[winners - 1] = i;
            }
            else if (candidates[i].votes > candidates[winners_indexes[0]].votes)
            {
                winners = 1;
                winners_indexes[winners - 1] = i;
            }
        }
    }

    if (winners > 0)
    {
        for (int i = 0; i < winners; i++)
        {
            printf("%s\n", candidates[winners_indexes[i]].name);
        }

        return;
    }

    // If all votes are invalid, there is no winner.
    printf("0 valid votes. There is no winner.\n");
}
