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
    for (int i = 0; i != candidate_count; i++) //sort through to give ranks[] at position of rank, the value of candidate
    {
        if (strcmp(name, candidates[i]) == 0)
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
    for (int i = 0; i != candidate_count; i++) //find position i within ranks
    {
        for (int j = i + 1; j != candidate_count; j++) //find position j within ranks, where j starts at next position
        {
            preferences[ranks[i]][ranks[j]]++; //each iteration will fill out matrix, this compares rank position with those thereafter
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i != candidate_count; i++) //navigate through preferences array
    {
        for (int j = i + 1; j != candidate_count; j++) //tried at j = 0, get duplicate pairs. Using i+1 skips pairs already found
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] < preferences[i][j]) //rather than cycling through whole thing, do binary search at once
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    //printf("candidate count %i\n", candidate_count);
    //printf("pair count %i\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //for (int i = 0; i < pair_count; i++){printf("(%i,", pairs[i].winner); printf("%i)\n", pairs[i].loser);}

    int sorter[candidate_count]; //assign strength to each candidate
    //Loop 1 to determine strength of each candidate, and copy to temporary array
    for (int i = 0; i != pair_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (pairs[i].winner == j)
            {
                sorter[j] += 1;
            }

            if (pairs[i].loser == j)
            {
                sorter[j] -= 1;
            }
        }
    }

    int strength[pair_count]; //summarize strength in order of current pairs[]
    //Loop 2 to assign strength to each position
    for (int i = 0; i != pair_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (pairs[i].winner == j || pairs[i].loser == j)
            {
                strength[i] += sorter[j];
            }
        }
    }

    int max; //determine what the max value is
    int temp_str; //placeholder for strength to swap
    pair temp_pair; //placeholder for pair to swap
    //Loop 3 to sort the pairs array
    for (int i = 0; i != pair_count; i++)
    {
        max = i;
        for (int j = i + 1; j != pair_count; j++)
        {
            if (strength[max] < strength[j])
            {
                max = j;
            }
        }
        //Pullin the Ol' Switcharoo, Linear Search Style
        temp_str = strength[max];
        temp_pair.winner = pairs[max].winner;
        temp_pair.loser = pairs[max].loser;
        strength[max] = strength[i];
        pairs[max].winner = pairs[i].winner;
        pairs[max].loser = pairs[i].loser;
        strength[i] = temp_str;
        pairs[i].winner = temp_pair.winner;
        pairs[i].loser = temp_pair.loser;

        //printf("(%i,", pairs[i].winner);
        //printf("%i)\n", pairs[i].loser);
    }

    //for (int i = 0; i < pair_count; i++){printf("(%i,", pairs[i].winner); printf("%i)\n", pairs[i].loser);}
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i != candidate_count; i++)
    {
        for (int j = 0; j != candidate_count; j++)
        {
            if (i == j) //condition of comparison against self
            {
                locked[i][j] = false;
            }
            else
            {
                for (int k = 0; k != pair_count; k++) //search through pairs for match
                {
                    if (pairs[k].winner == i && pairs[k].loser == j)
                    {
                        locked[i][j] = true;
                        break; //break will only come out of the k loop
                    }
                    else
                    {
                        locked[i][j] = false;
                    }
                }
            }
            //if(locked[i][j] == true){printf("1 ");}
            //else{printf("0 ");}
        }
        //printf("\n");
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int source = 0;
    int counter;

    for (int i = 0; i != candidate_count; i++)
    {
        counter = 0;
        //printf("i %i\n", i);
        for (int j = 0; j != candidate_count; j++) //check matrix if 2x trues found
        {
            //printf("j %i\n", j);
            if (locked[i][j] == true)
            {
                counter++;
                //printf("counter %i\n", counter);
            }
        }

        if (counter == 2)
        {
            source = i;
            //printf("source %i\n", source);
            break;
        }
    }

    printf("%s\n", candidates[source]);
    return;
}

