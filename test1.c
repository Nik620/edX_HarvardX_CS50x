#include <stdbool.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>
#include <strings.h>
#define DICTIONARY "dictionaries/large"
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
// Number of buckets in hash table
const unsigned int N = 2526; //check thru first 2 chars
// Hash table
node *table[N+1];
// Counter for number of words in dictionary
int word_count = 0;


// Hashes word to a number
unsigned int hash(const char *word)
{
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
    int index = ind_first*100 + ind_second*1;
    //ind_first*10000 + ind_second*100 + ind_third*1;

    //return index
    return index;
}

bool load(const char *dictionary)
{
    
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
        //check for start of list
        if(table[index]->next == NULL)
        {
            table[index] = n;

        }
        //pointer to go through list
        else		//replace w/ n->next = table[index]; table[index] = n;
        {
            //node *ptr = table[index];
            //while(ptr->next != NULL)
            //{
                //ptr = ptr->next;
            //}
            //ptr->next = n;
            n->next = table[index]; 
            table[index] = n;
        }
        word_count++;
    }
    //closing dictionary to save memory
    fclose(dictionary_file);
    return true;
}

int main(void)
{
    string input_val = "Hello";
    int hash_val = hash(input_val);
    printf("%i\n", hash_val);
    char *dictionary = DICTIONARY;
    bool loaded = load(dictionary);
    int val = 0;
    if(loaded == true)
    {
        val = 1;
    }
    printf("%i\n", val);
    return 0;
}