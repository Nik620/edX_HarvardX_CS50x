#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: "); //user input
    float l = count_letters(text); //number of letters
    float w = count_words(text); //number of words
    float s = count_sentences(text); //number of sentences
    float L = (l / w) * 100;
    float S = (s / w) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8; //Coleman-Liau index

    if (index <= 16 && index >= 1)
    {
        printf("Grade %.0f\n", index);
    }
    else if ( index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text) //subroutine to detect characters
{
    int ctr = 0; //counter for letters
    int txt = 0; //conversion from text to decimal

    for (int i = 0; text[i] != '\0'; i++ )
    {
        txt = (int)text[i];
        if ((txt >=65 && txt <= 90) || (txt >= 97 && txt <= 122))//capital letter
        {
            ctr++;
        }
    }

    return ctr;
}

int count_words(string text) //subroutine to detect words
{
    int ctr = 0;
    int txtp = 0; //first position
    int txtn = 0; //second position

    for (int i = 0; text[i] != '\0'; i++) //check first and second char value
    {
        txtp = (int)text[i];
        txtn = (int)text[i+1];
        if ((txtp >=65 && txtp <= 90) || (txtp >=97 && txtp <= 122)) //letter chk
        {
            if (txtn < 65 || txtn > 122)
            {
                ctr++;
            }
            else if (txtn >= 90 && txtn <= 96)
            {
                ctr++;
            }
        }
    }

    return ctr;
}

int count_sentences(string text) //subroutine to detect sentences
{
    int ctr = 0;
    int txt = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        txt = (int)text[i];
        if(txt == 33 || txt == 46 || txt == 63)
        {
            ctr++;
        }

    }

    return ctr;
}