// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26*26; //check thru first 2 chars

// Hash table
node *table[N];

// Counter for number of words in dictionary
int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //case sensitive
    int word_len = strlen(word);
    char word_copy[word_len];
    for (int i = 0; i < word_len; i++)
    {
        word_copy[i] = tolower(word[i]);
    }

    //Hash word to obtain a hash value
    int index = hash(word);

    //Access linked list at index in hash table
    //check that list exists at index
    //node *cursor = malloc(sizeof(node));
    node *cursor = table[index];
    //if(cursor == NULL)
    //{
        //return false;
    //}
    //if(table[index] == NULL)
    //{
        //return false;
    //}
    //Traverse linked list
    //look for word using strcasecmp
    //keep moving cursor until you get to null
    while (cursor-> next != NULL) //cursor -> next != NULL
    {
        if (strcasecmp(word_copy, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor -> next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //takes in a word and returns an unsigned int
    //input: word with alpha chars & apostrophes
    //output: numerical indec between 0 and N-1, inclusive
    //need to decide on value of N
    //if value >N, can take %N to get val in range

    //assessing first three chars for bucket aaa, aab, etc.
    char first = tolower(word[0]);
    char second = tolower(word[1]);
    //char third = tolower(word[2]);

    //assigning vals for each char
    //based on ascii table where a=97
    int ind_first = first - 97;
    int ind_second = second - 97;
    //int ind_third = third - 97;

    //index function, aaa = 000000, zzz = 252525, etc
    //thus no. buckets is 26 * 26 * 26
    //int index = ind_first*10000 + ind_second*100 + ind_third*1;
    int index = ind_first*100 + ind_second;

    //return index
    return index % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open up dictionary for you to read from
    //to load this data onto your hash table.
    // Returns true if you were successful in loading
    //data into hash table, and false if memory error.
    // Have to store data into hash table, which is
    //an array of linked lists, and assigns a number
    //to every input.
    // Hash
    // Takes a word as input, and outputs a number
    //corresponding to which bucket to store word in.
    // Every node will have a value and pointer.
    // Allocate memory into node, and put data in
    //node *n = malloc(sizeof(node));
    //strcpy(n->word, "Hello");
    //n->next = NULL;

    //Open dictionary file
    //use fopen, remember to check if return is NULL
    FILE *dictionary_file = fopen(dictionary, "r");
    if(dictionary_file == NULL)
    {
        printf("ERROR: file not found\n");
        return false;
    }
    //Read strings from file one at a time
    //use fscanf(file, "%s", word), will return EOF at end
    char new_word[45];
    int index;

    while(fscanf(dictionary_file, "%s", new_word) != EOF)
    {
        //Create a new node for each word
        //use malloc, check if return is NULL, copy w/ strcpy
        node *n = malloc(sizeof(node));

        if(n == NULL)
        {
            printf("ERROR: no memory\n");
            return false;
        }

        strcpy(n->word, new_word);
        n->next = NULL;

        //Hash word to obtain a hash value
        //use hash function, function takes str & ret index
        index = hash(new_word);

        //Insert node into hash table at that location
        //recall hash table is array of linked lists
        //pointers in correct order


        //pointer to go through list
        if(table[index] != NULL)
        {
            node *ptr = table[index];

            while(ptr->next != NULL)
            {
                ptr = ptr->next;
            }

            ptr->next = n;
        }
        //check for start of list
        else
        {
            table[index] = n;
        }
        word_count++;
    }

    //closing dictionary to save memory
    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Returns total no. words in dictionary
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // frees memory
    //be sure to use free to unload any memory in load. Recall valgrind is best friend.
    //$ valgrind .speller texts/cat.txt
    //Remember that your table is just an array of linked lists.

        //Create a variable called cursor, and while set use syntax free(cursor); First tho
    //you need to check where you are. Make another variable called tmp (equal to cursor)
    //move the cursor to the next node, and then free(tmp);. When cursor reaches NULL,
    //you reached the end of the linked llist and now are done.
    for (int i = 0; i < N; i++)
    {
        node *start = table[i];
        node *cursor = start;
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor -> next;
            free(tmp->word); //free memory
            free(tmp); //free node
        }
    }
    return true;
}
