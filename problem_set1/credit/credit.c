#include <stdio.h>
#include <cs50.h>

int get_length(unsigned long x);
bool is_valid_checksum(unsigned long ccn);
void print_cc_brand(unsigned long ccn);

int main(void)
{
    long ccn = 0;
    ccn = get_long("Enter your Credit Card number (without - or spaces): ");

    if (is_valid_checksum(ccn))
    {
        print_cc_brand(ccn);
    }
    else
    {
        printf("INVALID\n");
    }
}

// Returns length of the CC number
int get_length(unsigned long x)
{
    int length = 0;
    for (int i = 0 ; x != 0 ; x /= 10)
    {
        length++;
    }
    return length;
}

// Verifies the validity of the CC number with Luhn's algorithm
bool is_valid_checksum(unsigned long ccn)
{

    int length = get_length(ccn);

    if (length == 13 || length == 15 || length == 16)
    {
        int temp = 0;
        int checksum = 0;

        for (int i = 0 ; ccn != 0 ; i++)
        {
            if (i % 2 == 0)
            {
                checksum += ccn % 10;
                ccn = ccn / 10;
            }
            else
            {
                temp = 2 * (ccn % 10);
                if (temp >= 10)
                {
                    checksum += temp % 10;
                    checksum++;
                }
                else
                {
                    checksum += temp;
                }
                ccn = ccn / 10;
            }
        }
        return checksum % 10 == 0;
    }
    else
    {
        return false;
    }
}

// Prints the CC's brand or prints invalid if it starts with the wrong numbers
void print_cc_brand(unsigned long ccn)
{
    if ((ccn >= 34e13 && ccn < 35e13) || (ccn >= 37e13 && ccn < 38e13))
    {
        printf("AMEX\n");
    }
    else if ((ccn >= 4e12 && ccn < 5e12) || (ccn >= 4e15 && ccn < 5e15))
    {
        printf("VISA\n");
    }
    else if (ccn >= 51e14 && ccn < 56e14)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}