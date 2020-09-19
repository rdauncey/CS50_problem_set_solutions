#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool check_valid_key(string key);
string string_to_lower(string key);
string to_ciphertext(string key, string plain);


int main(int argc, string argv[])
{
    //User inputs the key when calling program

    //Return error if one arg not given
    if (argc != 2)
    {
        printf("Give one command line argument\n");
        return 1;
    }

    else
    {
        //Return error if invalid key given
        if (check_valid_key(argv[1]) == false)
        {
            printf("Key invalid\n");
            return 1;
        }

        //If key valid, proceed
        else
        {
            string plain = get_string("plaintext: ");
            printf("ciphertext: %s\n", to_ciphertext(argv[1], plain));
        }

    }
}


bool check_valid_key(string key)
{
    int n = strlen(key);

    //Check has 26 chars
    if (n != 26)
    {
        return false;
    }
    else
    {
        //Convert to lower case chars for ease
        string uniform_key = string_to_lower(key);

        for (int i = 0; i < n; i++)
        {
            //Check does not contain any other chars
            if (key[i] < 'a' || key[i] > 'z')
            {
                return false;
            }

            else
            {
                //Check no repeats further down string
                for (int j = i + 1; j < n; j++)
                {
                    if (key[i] == key[j])
                    {
                        return false;
                    }
                }
            }
        }
    }

    //If the key has passed through all of these without returning false then it is valid
    return true;
}



//Convert key to lowercase
string string_to_lower(string key)
{
    int n = strlen(key);
    for (int i = 0; i < n; i++)
    {
        key[i] = tolower(key[i]);
    }

    return key;
}


//Convert to cipher text
string to_ciphertext(string key, string plain)
{
    //Convert key to all lower case for convenience
    string uniform_key = string_to_lower(key);
    int k = strlen(plain);

    for (int i = 0; i < k; i++)
    {
        //Substitute lower case
        if (plain[i] >= 'a' && plain[i] <= 'z')
        {
            //Index of char in key given by (int) plain[i] - 97
            plain[i] = uniform_key[(int) plain[i] - 97];
        }

        //Substitute upper case
        else if (plain[i] >= 'A' && plain[i] <= 'Z')
        {
            //Index of char in key given by (int plain[i] - 65)
            char sub_letter = uniform_key[(int) plain[i] - 65];
            plain[i] = toupper(sub_letter);
        }

        //Other chars left alone
    }

    return plain;
}