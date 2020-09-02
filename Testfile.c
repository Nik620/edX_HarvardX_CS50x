#include <ctype.h>
#include <stdio.h>
#include <cs50.h>
int main(void)
{
    const char *word = "Word";
    char tester = word[0];
    tester = tolower(word[0]);
    printf("%c\n", tester);
    int dex = tester - 97;
    printf("%i\n", dex);
    return 0;
}