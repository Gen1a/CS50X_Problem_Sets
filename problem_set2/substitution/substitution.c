#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool contains_only_alphabetic(string text);
bool contains_duplicate_letter(string text);
void print_ciphertext(string key, string plaintext);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\nTry again.\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!contains_only_alphabetic(argv[1]))
    {
        printf("Key must only contain 26 alphabetic characters.\n");
        return 1;
    }
    else if (contains_duplicate_letter(argv[1]))
    {
        printf("Key must only contain 26 UNIQUE alphabetic characters.\n");
        return 1;
    }
    else
    {
        string key = argv[1];
        string plaintext = get_string("Enter the plain-text: ");
        print_ciphertext(key, plaintext);
        return 0;
    }
}

// Encyphers a plaintext into a ciphertext
void print_ciphertext(string key, string plaintext)
{
    char ciphertext[strlen(plaintext)];
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0 ; i < strlen(plaintext) ; i++)
    {
        if (isupper(plaintext[i]) != 0) // if the plaintext character is uppercase
        {
            for (int j = 0 ; j < strlen(alphabet) ; j++) // Loop through the alphabet looking for key character
            {
                if (tolower(plaintext[i]) == alphabet[j]) // if letter is found in alphabet
                {
                    ciphertext[i] = toupper(key[j]); // add key character to ciphertext and break out of for loop
                    break;
                }
            }
        }
        else if (islower(plaintext[i]) != 0) // if the plaintext character is lowercase
        {
            for (int j = 0 ; j < strlen(alphabet) ; j++) // Loop through the alphabet looking for key character
            {
                if (plaintext[i] == alphabet[j]) // if letter is found in alphabet
                {
                    ciphertext[i] = tolower(key[j]); // add key character to ciphertext and break out of for loop
                    break;
                }
            }
        }
        else // if the plaintext character is not an alphabetic character add it to ciphertext without encryption
        {
            ciphertext[i] = plaintext[i];
        }
    }
    printf("ciphertext: %s\n", ciphertext);
}

// Checks if key only contains alphabetic characters
bool contains_only_alphabetic(string text)
{
    for (int i = 0 ; i < strlen(text); i++)
    {
        if (!isalpha(text[i]))
        {
            return false;
        }
    }
    return true;
}

// Compares the i'th character to all the following characters and returns true if found duplicate letter
bool contains_duplicate_letter(string text)
{
    for (int i = 0 ; i < strlen(text) - 1 ; i++) // strlen(text) - 1 to stay inside array's bounds
    {
        int counter = 1;
        for (int j = i + 1 ; j < strlen(text); j++)
        {
            if (text[i] == text[j])
            {
                return true;
            }
        }
    }
    return false;
}