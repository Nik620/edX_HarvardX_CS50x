#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, i, j, k, l;
    do
    {
        height = get_int("Height: "); //Initial Prompt
    }
    while (height <= 0 || height > 8); //bounds conditions

    for (i = 1; i <= height; i++) //block builder
    {
        for (j = height - i; j > 0; j--) //Spaces
        {
            printf(" ");
        }
        for (k = 1; k <= i; k++) //Left Hashes
        {
            printf("#");
        }
        printf("  ");//Gap
        for (l = 1; l <= i; l++) //Right Hashes
        {
            printf("#");
        }
        printf("\n");//new line
    }
}
