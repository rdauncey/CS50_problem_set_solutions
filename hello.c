#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get user name
    string name = get_string("What is your name?\n");

    //Print personalised greeting
    printf("hello, %s\n", name);
}