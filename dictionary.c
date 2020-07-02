// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "dictionary.h"
#include <strings.h>//not clear in programmers manual, but found
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1; 

//counter for word in analysis. Not sure if int capable, using long
long count_word = 0;

// Hash table
node *table[N]; //array of node pointers

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Must be case sensitive, can use tolower() #include <ctype.h>, this should
    //probably be in a while loop as works only with chars.
    //Return true for words only if in dictionary. In the dictionary, each line
    //ends with \n, the dictionary contains at least 1 word, none >45 chars.
    //Each word will be lowercase, possible with apostrophes. Words will only contain
    //alphas and apostrophes. Only takes in text, and optionally dictionary as inpt,
    //must not leak, check leaks with valgrind. Use strcasecmp to compare two strings
    //case-insensitively
    int length = strlen(word);
    char word_cpy[length]; //discovered that word is not writeable
    
    for (int i = 0; i < length; i++)
    {
        word_cpy[i] = tolower(word[i]);
    }
    //Hash word to obtain a hash value. Access linked list at the index of hash tble.
    //Traverse linked list, looking for the word (use strcasecmp).
    int hash_value = hash(word_cpy);
    //Use cursor = cursor->next; to traverse through until you reach NULL if word not
    //inside the list.
    node *cursor = malloc(sizeof(node));
    if (cursor == NULL)
    {
        return false;
    }

    //Start with cursor set to first item in linked list. Keep moving cursor until you
    //get to NULL, checking each node for the word.
    cursor = table[hash_value];
    while (cursor -> next != NULL)
    {
        if (strcasecmp(word_cpy, cursor->word) == 0)
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
unsigned long hash(const char *word)
{
    //takes a string of input of chars and possibly ' and returns an unsigned integer.
    //output is numerical index between 0 and N-1 inclusive. Larger N means more bucket
    //If function ends up with value greater than N, you can take the value % N to get
    //a value in the appropriate range.

    //hash table from https://cp-algorithms.com/string/string-hashing.html
    //const int p = 31;
    //const int m = 1e9 + 9;
    //long long hash_value = 0;
    //long long p_pow = 1;
    //int length = strlen(word);
    //for (int i = 0; i < length; i++)//originally char c : word
    //{
        //char c = word[i];
        //hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        //p_pow = (p_pow * p) % m;
    //}

    //return hash_value;

    //https://stackoverflow.com/questions/7666509/hash-function-for-string
    unsigned long hash_value = 5381;
    int c = 0;

    while (c == *word++)
    {
        hash_value = ((hash_value << 5) + hash_value) + c; /* hash * 33 + c */
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // loads word from dictionary into function. Input is a string dictionary file you
    //read from. Returns true if you were successfully able to load data onto hash table,
    //false if memory error like not enough or overflow.
    //Takes a word as input, outputs a number corresponding to which bucket to store
    //the word in. Use the node table to store the word.
    //Use node *n = malloc(sizeof(node)); to put data into the node
    //use:strcpy(n->word, "Hello"); if you know what the next pointer is,set
    //right away, but if you don't, use a null ptr n->next = NULL;

    //process
    //open dictionary file, use fopen to gain access to file
    //remember to check if return val is NULL to see if successfuly able to open file
    //should return null, not false
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }

    //read strings from file one at a time with fscanf(file, "%s", word)
    //fscanf will return EOF once reaches end. %s is you want to read string
    //word will be where you'll read word into
    char dictionary_word[45];
    int hash_index;
    while (fscanf(file, "%s", dictionary_word) != EOF)
    {
        fscanf(file, "%s", dictionary_word);
        //create a new node for each word, use malloc (check if return val is null)
        //copy word into node using strcpy
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("no memory\n");
            //unload();
            return false;
        }
        //n->word = malloc((strlen(dictionary_word) + 1) * sizeof(char));
        //if (n->word == NULL)
        //{
            //printf("no memory\n");
            //return false;
        //}
        strcpy(n->word, dictionary_word);
        n->next = NULL;
        //hash word to obtain a hash value, use hash function (dictionary.c defined)
        //function takes a string and returns an index
        hash_index = hash(dictionary_word);

        //insert node into hash table at that location, recall hash table is an
        //array of linked lists. set pointers in correct order set new word pointer
        //to first element in linked list move head of linked list ptr to new word
        //repeat for every word in dictionary
        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
        }
        else
        {
            node* cursor = table[hash_index];
            while (cursor->next != NULL)
            {
                cursor = cursor->next;
            }
            //n->next = table[hash_index];
            //table[hash_index] = n;
            cursor->next = n;
        }
        count_word++;
    }
    //fclose(dictionary); //close dictionary
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //Return the number of words in the dictionary. Iterate over every linked lists
    //or as loading the hash table, keep track of each word being assessed.

    //keeping track using count_word
    return count_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    //be sure to use free to unload any memory in load. Recall valgrind is best friend.
    //$ valgrind .speller texts/cat.txt
    //Remember that your table is just an array of linked lists.

    //Create a variable called cursor, and while set use syntax free(cursor); First tho
    //you need to check where you are. Make another variable called tmp (equal to cursor)
    //move the cursor to the next node, and then free(tmp);. When cursor reaches NULL,
    //you reached the end of the linked llist and now are done.
    for (int i = 0; i < N; i++)
    {
        //node *tmp = table[i];
        node *cursor = table[i];

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
