#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float avg_per_100_words(int words, int count);

int main(void)
{
    string text = get_string("Text: ");

    //The Coleman-Liau index is index = 0.0588 * L - 0.296 * S - 15.8 where
    //L is avg letters per 100 words, and S is avg sentences per 100 words
    int words = count_words(text);
    float L = avg_per_100_words(words, count_letters(text));
    float S = avg_per_100_words(words, count_sentences(text));
    float index = 0.0588 * L - 0.296 * S - 15.8;

    //Round to determine the grade
    int grade = round(index);

    //Print the grade of the text
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}


//Count the number of letters in a string
int count_letters(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //Count lowercase
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            count += 1;
        }
        //Count uppercase
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            count += 1;
        }
    }

    return count;
}


//Count the number of words in a string
int count_words(string text)
{
    //Count the number of spaces in the text
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //The ascii for space is 32
        if (text[i] == 32)
        {
            count += 1;
        }
    }
    //Word count is spaces + 1 (no space at end of text)
    return count + 1;
}


//Count sentences
int count_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //We ignore the nuances and simply count anything that ends with . ! or ?
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count += 1;
        }
    }

    return count;
}


//Average per hundred words
float avg_per_100_words(int words, int count)
{
    //Avg is count * (100 / words)
    return (float) count * (100 / (float) words);
}