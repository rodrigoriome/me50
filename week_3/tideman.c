#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

bool is_cyclic();
bool dfs(int node, bool visited[], bool r_stack[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;

            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count - 1; j++)
        {
            preferences[ranks[i]][ranks[j + 1]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int unsorted = pair_count;

    // Bubble sort for quickness
    // TODO: Change for Merge Sort
    while (unsorted > 0)
    {
        for (int i = 0; i < unsorted; i++)
        {
            int curr_diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int next_diff = preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner];

            if (curr_diff < next_diff)
            {
                pair temp_pair;

                temp_pair.winner = pairs[i].winner;
                temp_pair.loser = pairs[i].loser;

                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser = pairs[i + 1].loser;

                pairs[i + 1].winner = temp_pair.winner;
                pairs[i + 1].loser = temp_pair.loser;
            }
        }

        unsorted--;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Lock each pair
        locked[pairs[i].winner][pairs[i].loser] = true;

        // If a cycle has been detected, unlock last pair
        if (is_cyclic())
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

bool is_cyclic()
{
    bool visited[candidate_count];
    bool r_stack[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        visited[i] = false;
        r_stack[i] = false;
    }

    // Navigate through graph to look over cycles
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                if (!visited[i] && dfs(i, visited, r_stack))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// The locked array is an Adjacent Matrix to represent a Graph. In this case, we use an alternative version of
// Depth-First-Search algorithm to detect cycles between the graph nodes
bool dfs(int node, bool visited[], bool r_stack[])
{
    // If the current node is found on the recursion stack, it means we found a cycle
    if (r_stack[node])
    {
        return true;
    }

    // Mark node as visited and add to recursion stack
    visited[node] = true;
    r_stack[node] = true;

    // Visit adjacent candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If found, recur on it
        if (locked[node][i] && dfs(i, visited, r_stack))
        {
            return true;
        }
    }

    // If no cycle were found for this this, remove it from current navigation stack
    r_stack[node] = false;

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    int source;
    bool is_source;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                // Mark each candidate as source
                is_source = true;
            }
        }

        for (int j = 0; j < candidate_count; j++)
        {
            // If the current source was beat in any other locked pair
            if (locked[j][i])
            {
                // Unmark him as source
                is_source = false;
            }
        }

        if (is_source)
        {
            source = i;
        }
    }

    printf("%s\n", candidates[source]);
}
