//Hash functions

#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Each bucket represents two letters
const unsigned int N_1 = 26 * 26 + 1;

unsigned int hash_1(const char *word)
{
    int first_pos_shift;
    int second_pos_shift;

    //Determine shift of the first position
    if (islower(word[0]))
    {
        first_pos_shift = 97;
    }
    else if (isupper(word[0]))
    {
        first_pos_shift = 65;
    }
    else
    {
        first_pos_shift = N-2;
    }

    //Determine shift of the second position
    if (islower(word[1]))
    {
        second_pos_shift = 97;
    }
    else if (isupper(word[1]))
    {
        second_pos_shift = 65;
    }
    else if (isspace(word[1]))
    {
        second_pos_shift = 32;
    }
    else
    {
        second_pos_shift = N-2;
    }

    //Return hash value
    return (unsigned int) ((word[0] - first_pos_shift) * 26) + (word[1] - second_pos_shift);

    //We reserve the final bucket in the hash table for the word count
}

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
        return N-2;
    }
    //We reserve the final bucket in the hash table for the word count