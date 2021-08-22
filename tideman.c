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
bool visit(pair *visitedaddress, int i);
bool newvisit(pair start, int i, int j);
void record_preferences(int ranks[]);
void add_pairs(void);
void swapi(int *a, int *b);
void swapp(pair *a, pair *b);
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
    int preferences_len = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            // Checking if i + j is out of bounds
            if (i + j < candidate_count)
            {
                preferences[ranks[i]][ranks[i + j]]++;
                preferences_len++;
            }
            else
            {
                break;
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
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
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
            else if (ji > ij)
            {
                pairs[pair_count] = make_pair(j, i);
                pair_count++;
            }
            else if (ij == ji)
            {
                // pairs[pair_count] = make_pair(i, j);
                // pair_count++;
                // pairs[pair_count] = make_pair(j, i);
                // pair_count++;
            }
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("%s %s %i\n", candidates[pairs[i].winner], candidates[pairs[i].loser], preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
    }
    printf("\n");
    return;
}

// Function to swap the two values at the two given pointers
void swapi(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}

// Same thing but for pairs
void swapp(pair *a, pair *b)
{
    pair temp = *a;
    *a = *b;
    *b = temp;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Basically i'm gonna get the strengths of win/lose pairs in an array and then sort that array but making sure to keep that array and the "pairs" array synced.
    int strengths[MAX * (MAX - 1) / 2];
    for (int i = 0; i < pair_count; i++)
    {
        pair this_pair = pairs[i];
        int strength = preferences[this_pair.winner][this_pair.loser] - preferences[this_pair.loser][this_pair.winner];
        strengths[i] = strength;
    }
    // Strengths array is done. I'm gonna sort that, while updating the pairs array accordingly.
    for (int i = 0; i < pair_count - 1; i++)
    {
        bool swapped_this_pass = false;
        for (int j = 0; j < pair_count - (i + 1); j++)
        {
            // printf("%i %i\n", i, j);
            if (strengths[j] < strengths[j + 1])
            {
                // for (int n = 0; n < pair_count; n++)
                // {
                    // printf("%s %s %i\n", candidates[pairs[n].winner], candidates[pairs[n].loser], strengths[n]);
                // }
                swapi(&strengths[j], &strengths[j + 1]);
                swapp(&pairs[j], &pairs[j + 1]);
                // printf("Swapped %s %s, %s %s and %i, %i at %i, %i\n", candidates[pairs[j].winner], candidates[pairs[j].loser], candidates[pairs[j + 1].winner], candidates[pairs[j + 1].loser], strengths[j], strengths[j + 1], j, j + 1);
                swapped_this_pass = true;
            }
        }
        // Exit if no swaps were made in a certain pass
        if (!swapped_this_pass)
        {
            break;
        }
    }
    for (int i = 0; i < pair_count; i++)
    {
        printf("%s %s %i\n", candidates[pairs[i].winner], candidates[pairs[i].loser], strengths[i]);
    }
    printf("\n");
    return;
}

// Explore one candidate's locked[][] entries to determine whether a cycle is created
bool visit(pair *visitedaddress, int i)
{
    for (int j = 0; j < MAX; j++)
    {
        // If there is an arrow from i to j
        if (locked[j][i])
        {
            // Explore it if we haven't already
            for (int n = 0; n < MAX; n++)
            {
                if (visitedaddress[n].loser == i && visitedaddress[n].winner == j)
                {
                    // We have gotten to someone already visited! Done!
                    printf("%s %s was looped back to\n", candidates[visitedaddress[n].loser], candidates[visitedaddress[n].winner]);
                    return true;
                }
            }
            visitedaddress[i] = make_pair(j, i);
            return visit(visitedaddress, j);
        }
    }
    // Investigated every partner. None were locked. No loop detected.
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Add each pair, one at a time, using check_for_cycle as a checker
    for (int i = 0; i < pair_count; i++)
    {
        pair visited[MAX] = {make_pair(0, 0)};
        pair this_pair = pairs[i];
        locked[this_pair.winner][this_pair.loser] = true;
        if (visit(visited, this_pair.winner))
        {
            locked[this_pair.winner][this_pair.loser] = false;
            printf("Found cycle: \n");
            for (int j = 0; j < candidate_count; j++)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    printf("%i ", locked[j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
        else
        {
            printf("Locked in %s %s\n", candidates[this_pair.winner], candidates[this_pair.loser]);
            for (int j = 0; j < candidate_count; j++)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    printf("%i ", locked[j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Basically loop through all the possible winners in the locked array and then loop through and check if anyone is locked against them. If nobody is, then they win! Otherwise, go to the next candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        bool someone_beat_them = false;
        for (int j = 0; j < candidate_count; j++)
        {
            // If j beats i
            if (locked[j][i])
            {
                someone_beat_them = true;
                break;
            }
        }
        // Nobody beat them!
        if (!someone_beat_them)
        {
            // They win!
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}
