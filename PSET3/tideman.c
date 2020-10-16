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

//Margin Calculator *new, used to check strength margine
int margin[MAX];

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
    for (int i = 0; i != candidate_count; i++) //sort through to give ranks[] at position of rank
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
        for (int j = i + 1; j != candidate_count; j++) //find position j within ranks
        {
            preferences[ranks[i]][ranks[j]]++; //each iteration will fill out
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int temp = 0;

    for (int i = 0; i != candidate_count; i++) //navigate through preferences array
    {
        for (int j = i + 1; j != candidate_count; j++)
        {
            if (preferences[i][j] == preferences[j][i])
            {
                temp = 0; //null value, for some reason doesn't work without
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                margin[pair_count] = preferences[i][j];
                pair_count++;
            }
            else
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                margin[pair_count] = preferences[j][i];
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int maxi;
    int tmp_str;
    pair tmp_pair;

    for (int i = 0; i != pair_count; i++)
    {
        tmp_str = margin[i]; //placeholders
        tmp_pair.winner = pairs[i].winner;
        tmp_pair.loser = pairs[i].loser;
        maxi = i;
        for (int j = i + 1; j != pair_count; j++)
        {
            if (margin[maxi] < margin [j]) //checking strength margin
            {
                maxi = j;
            }
        }
        //Pullin the ol switcharoo, linear search
        margin[i] = margin[maxi];
        pairs[i].winner = pairs[maxi].winner;
        pairs[i].loser = pairs[maxi].loser;
        margin[maxi] = tmp_str;
        pairs[maxi].winner = tmp_pair.winner;
        pairs[maxi].loser = tmp_pair.loser;
    }
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
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int counter;
    bool nocycle = false;
    int max = 0;
    int tie[candidate_count];

    for (int i = 0; i != candidate_count; i++)
    {
        counter = 0;
        for (int j = 0; j != candidate_count; j++) //check matrix if 2x trues found
        {

            if (locked[i][j] == true)
            {
                counter++;
            }
        }
        tie[i] = counter;
        if (counter == 2)
        {
            nocycle = true;
        }
        else if (max < counter)
        {
            max = counter;
        }
    }
    for (int i = 0; i != candidate_count; i++)
    {
        if (nocycle == true && tie[i] == 2)
        {
            printf("%s\n", candidates[i]);
            break;
        }
        else if (nocycle == false && pairs[0].winner == i)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}
