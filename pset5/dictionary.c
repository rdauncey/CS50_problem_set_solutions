// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table - start with 27 (letters of the alphabet plus apostrophe
// and space for storing word count)
const unsigned int N = 26 + 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Create temp node
    node *current_node = table[hash(word)];

    //Systematically check through list
    while (current_node != NULL)
    {
        if (strcasecmp(current_node->word, word) == 0)
        {
            return true;
        }

        current_node = current_node->next;
    }
    //If not found, loop ends and we return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //We may assume all words in dictionary are lower case or contain apostrophes
    //Sort into buckets in alphabetical order, with the last one for words beginning with '
    //i.e. words beginning with a are put in a linked list in table[0]
    if (islower(word[0]))
    {
        return (unsigned int) word[0] - 97;
    }

    //Must include this for check function
    else if (isupper(word[0]))
    {
        return (unsigned int) word[0] - 65;
    }

    //By assumption if not lower case, must be an apostrophe
    else
    {
        return N - 2;
    }
    //We reserve the final bucket in the hash table for the word count
}


// Loads dictionary into a data structure in memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Open dictionary as a file & check it has opened
    FILE *dict = fopen(dictionary, "r");
    if (!dict)
    {
        fclose(dict);
        return false;
    }

    //For each word found, create a node
    //Use hash function to determine the bucket, and then insert at the beginning of the linked list

    char word[LENGTH + 1];
    int word_count = 0;
    int scan_value = 0;

    while (fscanf(dict, "%s", word) != EOF)
    {
        //Create a node for the word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            unload();
            fclose(dict);
            return false;
        }

        //Copy the current word into the node
        strcpy(n->word, word);

        //Determine hash number
        unsigned int hash_no = hash(word);

        //Insert the node at the beginning of the relevant linked list
        n->next = table[hash_no];
        table[hash_no] = n;

        //Update word count
        word_count++;
    }

    //Include word count in hash table
    node *no_words = malloc(sizeof(node));
    if (no_words == NULL)
    {
        unload();
        fclose(dict);
        return false;
    }

    //Use sprintf to convert word count to string and store in no_words
    char num_words[20];
    sprintf(num_words, "%i", word_count);
    strcpy(no_words->word, num_words);

    //Insert the word count at the beginning of the table[N-1] list
    no_words->next = table[N - 1];
    table[N - 1] = no_words;

    fclose(dict);
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (table[N - 1] != NULL)
    {
        //Get the string giving the word count from the end of the hash table, and convert to an int
        char *str_count = table[N - 1]->word;
        int int_count = atoi(str_count);
        return int_count;
    }

    else
    {
        return 0;
    }
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //For each bucket in table, sequentially free the space
    for (int i = 0; i < N; i++)
    {
        //Check that a linked list exists in bucket
        if (table[i] != NULL)
        {
            node *current_node = table[i];
            node *tmp = current_node;
            current_node = current_node->next;

            //Loop through to end of linked list, freeing nodes pointed at by tmp
            //and using current_node to ensure we don't lose the end of the list
            while (current_node != NULL)
            {
                free(tmp);
                tmp = current_node;
                current_node = current_node->next;
            }

            free(current_node);
            free(tmp);
        }
    }

    return true;
}
