// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 20000; // produces output at same rate as CS50 Staff

// Hash table
node *table[N];

// Variable to store amount of words in dictionary
unsigned int s;

// Variable to store whether dictionary is loaded
bool loaded = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // check should be case-insensitive
    // word-argument is constant => make copy
    char copy[strlen(word)];
    strcpy(copy, word);

    // Use hash function to obtain hashvalue
    for (int i = 0 ; copy[i] != '\0' ; i++)
    {
        copy[i] = tolower(copy[i]);
    }
    int hashcode = hash(copy);

    // Access linked list at retrieved index in hash table
    // Look through linked list for the word (strcasecmp)
    for (node *cursor = table[hashcode]; cursor != NULL ; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, copy ) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
// Hashfunction reference: https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
         hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % N; //N is size of hashtable
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file from function argument
    // fopen() + check if null
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        printf("Could not load dictionary.");
        return false;
    }

    // Start with setting hashtable pointers to NULL
    for (int i = 0 ; i < N ; i++)
    {
        table[i] = NULL;
    }
    // Read strings from dictionary file
    // fscanf(file, %s, word)
    // fscanf returns EOF at the end of the file
    char w[LENGTH + 1]; // LENGTH + 1 for \0
    while (fscanf(file, "%s", w) != EOF)
    {
        // Create new node with malloc + check return value
        // Copy word into node with eg. strcopy(n->word, "Hello");
        node *newWord = malloc(sizeof(node));
        if(!newWord)
        {
            printf("Error allocating memory (ran out of memory).\n");
            return false;
        }
        strcpy(newWord->word, w);
        //newWord->next = NULL;

        // Hash word with Hash function (argument = word, output = number/index)
        int hashcode = hash(w);

        // Insert node in hashtable
        newWord->next = table[hashcode];
        table[hashcode] = newWord;

        // Add 1 to dictionary size counter
        s++;
    }
    // Set global variable loaded to true;
    loaded = true;
    // Close the dictionary file after entirely reading it
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return loaded ? s : 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if(!loaded)
    {
        return false;
    }

    // Loop to iterate over array containing linked lists
    for(int i = 0 ; i < N ; i++)
    {
        // Initialize cursor to iterate over linked list
        node *cursor = table[i];
        while(cursor != NULL)
        {
            node *temp = cursor; // Pointer to save cursor's pointer
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
