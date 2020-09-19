#include <stdio.h>
#include <cs50.h>

void print_spaces(int k);
void print_bricks(int m);
void build_pyramid(int n);


int main(void)
{
    int n;

    do
    {
        n = get_int("Height: \n");
    }
    while (n < 1 || n > 8);

    build_pyramid(n);
}


//Function for drawing pyramid
void build_pyramid(int n)
{
    //i indexes the rows
    for (int i = 0; i < n; i++)
    {
        //Print preceeding spaces
        print_spaces(n - i - 1);

        //Print left hand bricks
        print_bricks(i + 1);

        //Print space in between
        print_spaces(2);

        //Print right hand bricks
        print_bricks(i + 1);

        printf("\n");

    }
}


//Function for printing m bricks
void print_bricks(int m)
{
    for (int j = 0; j < m; j++)
    {
        printf("#");
    }
}


//Function for printing k spaces in a row
void print_spaces(int k)
{
    for (int i = 0; i < k; i++)
    {
        printf(" ");
    }
}
