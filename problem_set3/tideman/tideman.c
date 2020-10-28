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
bool creates_cycle(int index);
bool have_passed_candidate(int index, bool passed_candidate[]);
bool check_source(int column);

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
    for (int i = 0 ; i < candidate_count ; i++) // Loop through candidates string array
    {
        if (strcmp(name, candidates[i]) == 0) // if a match is found between user input's vote and candidate name, update ranks array
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
    for (int i = 0 ; i < candidate_count ; i++)
    {
        for (int j = i + 1 ; j < candidate_count ; j++)
        {
            preferences[ranks[i]][ranks[j]]++; // Loop through ranks array 2 votes a time and update preferences
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Note: when no winner => don't update pairs array
    // Note: Update global variable pair count after adding a pair
    for (int i = 0 ; i < candidate_count ; i++)
    {
        for (int j = 0 ; j < candidate_count ; j++)
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
    // Create an array which calculates the strengths of the victories per pair
    int strengths[pair_count];
    for (int i = 0 ; i < pair_count ; i++)
    {
        strengths[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    // Using insertion sort to sort the strengths array and link the algorithm to the pairs array
    for (int i = 1 ; i < pair_count ; i++)
    {
        int strongest_vict = strengths[i];
        pair strongest_pair = pairs[i];
        int j = i - 1;

        while (strongest_vict > strengths[j] && j >= 0)
        {
            strengths[j + 1] = strengths[j];
            pairs[j + 1] = pairs[j];
            j--;
        }
        strengths[j + 1] = strongest_vict;
        pairs[j + 1] = strongest_pair;
    }
}

bool have_passed_candidate(int index, bool passed_candidate[])
{
    // Base Case
    if (passed_candidate[index])
    {
        return true;
    }
    // Set the value for the candidate we passed to true
    passed_candidate[index] = true;
    // Loop through locked array to check neighbours and if there is a cycle starting with the following neighbour(s)
    for (int i = 0 ; i < candidate_count ; i++)
    {
        if (locked[index][i] && have_passed_candidate(i, passed_candidate))
        {
            return true;
        }
    }
    return false;
}

// Function to check whether locking in the pair creates a cycle in the graph
bool creates_cycle(int index)
{
    //First locked pair can't create a cycle
    if (index == 0)
    {
        return false;
    }

    // Create a bool array to check if we passed a candidate already in the loop
    bool passed_candidate[candidate_count];
    for (int i = 0 ; i < candidate_count ; i++)
    {
        passed_candidate[i] = false;
    }
    return have_passed_candidate(index, passed_candidate);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0 ; i < pair_count ; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        locked[winner][loser] = true;
        // Call function to check if setting this value in locked array to true creates a cycle
        if (creates_cycle(i))
        {
            locked[winner][loser] = false; // set back to false to prevent creating a cycle
        }
    }
}

// Checks if a certain candidate is the source (= has no incoming edges)
bool check_source(int column)
{
    for (int i = 0 ; i < candidate_count ; i++)
    {
        if (locked[i][column]) // if the candidate has an incoming edge => not the source
        {
            return false;
        }
    }
    return true;
}
// Print the winner of the election
void print_winner(void)
{
    string output = "";
    int counter = 0;
    // Create loop and call other other function to make it easier to iterate through a column
    for (int i = 0 ; i < candidate_count ; i++)
    {
        if (check_source(i))
        {
            printf("%s\n", candidates[i]);
        }
    }
}