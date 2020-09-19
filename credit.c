#include <stdio.h>
#include <cs50.h>
#include <math.h>

int find_length(long num);
int extract_digit(long num, int digit_position);
bool luhns_alg(long n);
void check_type(long n);
void test(long n);



int main(void)
{
    long number = get_long("Number: ");
    test(number);
}



void test(long n)
{
    //First check the length - must be between 13 and 16
    if (n > pow(10, 16))
    {
        printf("INVALID\n");
    }
    else if (n < pow(10, 12))
    {
        printf("INVALID\n");
    }
    else
    {
        //Now check validity
        bool validity = luhns_alg(n);

        if (validity == false)
        {
            printf("INVALID\n");
        }
        else
        {
            //Finally check type
            check_type(n);
        }
    }
}


//Check the beginning digits to determine type and double check
//length is correct
void check_type(long n)
{
    int len = find_length(n);
    int first_digit = extract_digit(n, len - 1);
    int second_digit = extract_digit(n, len - 2);

    //Amex start with 34 or 37 and are 15 digits long
    if (first_digit == 3 && len == 15)
    {
        if (second_digit == 4 || second_digit == 7)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //Mastercard start with 51, 52, 53, 54, 55 and are 16 digits long
    else if (first_digit == 5 && len == 16)
    {
        if (second_digit < 6)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //Visa start with 4 andare 13 or 16 digits long
    else if (first_digit == 4)
    {
        if (len == 13 || len == 16)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


//Implement Luhn's Algorithm
bool luhns_alg(long n)
{
    int len = find_length(n);

    //Multiply even digits by two and sum the digits of these
    int even_sum = 0;

    for (int i = 1; i < len; i = i + 2)
    {
        int dig = extract_digit(n, i) * 2;

        //Check if single or double digits (must be <20)
        if (dig < 10)
        {
            even_sum = even_sum + dig;
        }
        else
        {
            int dig_sum = (dig % 10) + ((dig / 10) % 10);
            even_sum = even_sum + dig_sum;
        }
    }

    //Sum the odd digits
    int odd_sum = 0;

    for (int j = 0; j < len; j = j + 2)
    {
        int dig = extract_digit(n, j);
        odd_sum = odd_sum + dig;
    }

    //Add these two together
    int sum = even_sum + odd_sum;

    //Modulo 10
    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Extract specific digits
int extract_digit(long num, int digit_position)
{
    long divisor = pow(10, digit_position);
    int digit = (num / divisor) % 10;
    return digit;
}


//Find length between 13 and 16 digits; input is a number with between 13 and 16 digits
int find_length(long num)
{
    if (num < pow(10, 13))
    {
        return 13;
    }
    else if (num < pow(10, 14))
    {
        return 14;
    }
    else if (num < pow(10, 15))
    {
        return 15;
    }
    else
    {
        return 16;
    }
}