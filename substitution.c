#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int check(string key); //check if key contents are valid

int main(int keyc, string keyv[]) //keyc is cmd lines, keyv is array of strings
{
    //STEP 1: KEY VALIDATION
    if (keyc != 2) //check for correct # string inputs
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = keyv[1];
    int msg = check(key); //key contents
    //0 = valid, 1 = #chars or chartype incorrect, 2 = chars used more than once

    if (msg == 1)
    {
        printf("Invalid key, must contain 26 alpha characters.\n");
        return 1;
    }
    else if (msg == 2)
    {
        printf("Invalid key, characters must be different.\n");
        return 1;
    }

    //STEP 2: INPUT & CONVERSION
    string text = get_string("plaintext: "); //user input if all ok
    printf("ciphertext: ");
    int len = strlen(text); //text length
    int pos = 0; //alphabetical position

    for (int i = 0; i < len; i++ )
    {
        if ((int)text[i] >= 65 && (int)text[i] <= 90) //cap condition
        {
            pos = (int)text[i] - 65;
            printf("%c", (char)toupper(key[pos]));
        }
        else if ((int)text[i] >= 97 && (int)text[i] <= 122) //lwr condition
        {
            pos = (int)text[i] - 97;
            printf("%c", (char)tolower(key[pos]));
        }
        else //all other chars
        {
            printf("%c", text[i]);
        }

        if(i == len - 1)
        {
            printf("\n");
            return 0; //end program
        }
    }


}

int check(string key)
{
    int len = strlen(key); //calculate length of stirng
    int out = 0; //output val
    int noalp = 0; //no. chars not in alphabet
    int keyc = 0; //individual characters
    bool dup = false; //duplicate check
    //int capchar = 0; //convert char to cap
    //int capchartot = 0; //summation of asci value of cap
    //int capcharcap = 0; //check to see if chars aren't used more than once

    for (int i = 0; i < 26; i++) //alphabet checker
    {
        keyc = isalpha((int)key[i]);
        for(int j = 0; j < 26; j++)
        {
            if(toupper((int)key[i]) == toupper((int)key[j]))
            {
                dup = true;
                break;
            }
        }
        //capchar = toupper((int)key[i]);
        //capchartot += capchar;
        //capcharcap += (65 + i); //initialize at letter A
        if(keyc == 0)
        {
            noalp++;
        }
    }

    if (len != 26 || noalp > 0) //not 26 characters or invalid chars
    {
        out = 1;
    }
    else if (dup == true)
    {
        out = 2;
    }
    //else if (capchartot != capcharcap) //check for multiple use of chars
    //{
        //out = 2;
    //}

    return out;
}