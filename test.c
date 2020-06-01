//checking if can change char set to lower:
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
int main(void)
{
    string input = get_string("write input: ");
    int length = strlen(input);
    for(int i = 0; i < length; i++)
    {
        input[i] = tolower(input[i]);
    }
    printf("%s", input);
}