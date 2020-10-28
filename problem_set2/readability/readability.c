#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Implementation of a program that computes the approximate grade level needed to comprehend some text

int letters_amount(char* text);
int words_amount(char* text);
int sentences_amount(char* text);
void print_grade(int letters, int words, int sentences);

int main(void)
{
    char *text = get_string("Enter the text you wish to evaluate: ");
    printf("%s\n", text);
    int letters = letters_amount(text);
    // printf("%i\n", letters);
    int words = words_amount(text);
    // printf("%i\n", words);
    int sentences = sentences_amount(text);
    // printf("%i\n", sentences);
    print_grade(letters, words, sentences);
}

// Returns the amount of letters in a string
int letters_amount(char* text)
{
    int letters = 0;
    for (int i = 0 ; i < strlen(text) ; i++)
    {
        if (isalpha(text[i]) != 0)
        {
            letters++;
        }
    }
    return letters;
}

// Returns the ammount of words in a string
int words_amount(char* text)
{
    int words = 0;
    for (int i = 0 ; i < strlen(text) ; i++)
    {
        if (isspace(text[i]) != 0)
        {
            words++;
        }
    }
    return ++words;
}

// Returns the ammount of sentences in a string (only accounting for . ? or ! as the end of a sentence)
int sentences_amount(char* text)
{
    int sentences = 0;
    for (int i = 0 ; i < strlen(text) ; i++)
    {
        if ((text[i]) == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }
    return sentences;
}

// Prints out the grade level of the provided text
void print_grade(int letters, int words, int sentences)
{
    // Coleman Liau's index: index = 0.0588 * L - 0.296 * S - 15.8
    float L = (float)letters * (100 / (float)words);
    float S = (float)sentences * (100 / (float)words);
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}