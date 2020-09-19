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
bool follow_paths(int starts[], int num_starts, int target);

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
    //Check valid name
    for (int i = 0; i < candidate_count; i++)
    {
        //If a match found, update ranks with the candidate name (we assume
        //no two candidates have the same name)
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    //If no valid name found, return false
    return false;
}


// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //Fill the preferences - ranks[] is an array of the candidates positions
    //where their index indicates their preference e.g. the candidate in ranks[0] is
    //prefered over ranks[k] for k > 0
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //Initialise pair_count value
    pair_count = 0;

    //Loop over preferences and compare the votes
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Introduce some dummy variables
    bool swaps;
    int votesA;
    int votesB;
    pair dummy_pair;

    //We only need to compare the winning values and bubble sort accordingly
    for (int i = 0; i < pair_count - 1; i++)
    {
        swaps = false;
        for (int j = 0; j < pair_count - 1; j++)
        {
            //Determine winning # votes for pair j and pair j+1
            votesA = preferences[pairs[j].winner][pairs[j].loser];
            votesB = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            //If pair j has lesser margin that pair j+1, swap
            if (votesA < votesB)
            {
                dummy_pair = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = dummy_pair;
                swaps = true;
            }
        }

        //If there are no swaps made, sorting is finished and we can break out of the loop
        if (swaps == false)
        {
            break;
        }

    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int starts[1];;
        starts[0] = pairs[i].loser;

        if (!follow_paths(starts, 1, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}


//There will be a created by adding path i->j if there is a path
//from j to i, so we must follow all paths from j to make sure none such exist
bool follow_paths(int starts[], int num_starts, int target)
{
    //We collect an array of all the indexes for which there is a path from one of the start indexes
    //to another index, and we count how many we have - make path_indexes the square of the candidate count
    //to be sure there is enough space - this is obviously not the most efficient method
    int path_indexes[candidate_count * candidate_count];
    int paths_out = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < num_starts; j++)
        {
            //If there is a path, add the end index to the array
            if (locked[starts[j]][i] == true)
            {
                path_indexes[paths_out] = i;
                paths_out += 1;
            }
        }
    }

    //If no paths out, terminate - there is no cycle
    if (paths_out == 0)
    {
        return false;
    }

    else
    {
        for (int k = 0; k < paths_out; k++)
        {
            //Check if our target index was reached
            if (path_indexes[k] == target)
            {
                return true;
            }
        }

        //If target node not reached, repeat with the new set of starting indexes
        return follow_paths(path_indexes, paths_out, target);
    }
}


// Print the winner of the election
void print_winner(void)
{
    //The winner is the (assume unique) source of the graph
    //locked[i][j] = true means there is a path from i to j so
    //for j to be a source, we must have locked[k][j] = false for
    //all 0 <= k <= candidate_count
    for (int i = 0; i < candidate_count; i++)
    {
        bool not_source = false;
        int j = 0;

        while (not_source == false && j < candidate_count)
        {
            if (locked[j][i] == true)
            {
                not_source = true;
            }

            j += 1;
        }

        //Not_source will only still be false here if whole column was false
        if (not_source == false)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

