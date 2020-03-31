#include <stdio.h> //Main library
#include <cs50.h> //CS50 Library

int main(void)
{
    string name = get_string("What is your name?\n");//Prompt user for name
    printf("hello, %s\n", name);//output name
}
