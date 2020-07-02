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
    printf("%s\n", input);

    int value = sizeof(char);
    printf("%i\n",value);
    
    
    string word = input;
    int length2 = strlen(word);
    char word_cpy[length2];
    char copy[strlen(word)];
    printf("method1 %s \n", word_cpy);
    printf("method2 %s ", copy);
}