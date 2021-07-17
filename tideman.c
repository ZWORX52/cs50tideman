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
pair make_pair(int winner, int loser);
int index(string str, string arr[], int arr_size);
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

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

// Get index of string in an array of strings
int index(string str, string arr[], int arr_size)
{
    for (int i = 0; i < arr_size; i++)
    {
        if (strcmp(str, arr[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // candidates[i] == name
            ranks[rank] = index(name, candidates, candidate_count);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; i < candidate_count; j++)
        {
            if (i + j < candidate_count)
            {
                preferences[ranks[i]][ranks[i + j]]++;
            }
        }
    }
    return;
}

// Takes a winner and a loser and makes a pair out of them
pair make_pair(int winner, int loser)
{
    pair this_pair;
    this_pair.winner = winner;
    this_pair.loser = loser;
    return this_pair;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Does "i" win or does "j" win?
            int ij = preferences[i][j], ji = preferences[j][i];
            // More people prefer i over j than people who prefer j over i, therefore i beats j
            if (ij > ji)
            {
                pairs[pair_count] = make_pair(i, j);
                pair_count++;
            }
            // Otherwise, j beats i
            else
            {
                pairs[pair_count] = make_pair(j, i);
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Basically i'm gonna get the strengths of win/lose pairs in an array and then sort that arra but making sure to keep that array and the "pairs" array synced.
    int strengths[MAX * (MAX - 1) / 2];
    for (int i = 0; i < pair_count; i++)
    {
        this_pair = pairs[i];
        int strength = preferences[this_pair.winner][this_pair.loser] - preferences[this_pair.loser][this_pair.winner];
        strengths[i] = strength;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}
