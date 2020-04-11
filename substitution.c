#include <stdio.h>
#include <cs50.h>
//#include <math.h>
#include <string.h>
#include <ctype.h>

//arg is # of arguments, key is to check
int validation(int arg, string key);


//keyc is # of cmd line args
//keyv is all of the inputs provided, [] is array of strings
int main(int keyc, string keyv[])
{
    //input of 26 character key, if not, mention 26 char
    //case sensitive, no impact to case of inp to outp
    //non letters do not change
    //if program is executed without command-line arg
        //or more than one, then print error message
        //to remind the user to input key
        //from main, return a value of 1
    //if key is invalid (not 26 char, char is not alpha,)
        //(char only once), print error message
        //to remind user to input 26 diff alpha chars
        //from main, return a value of 1
    //if key is valid, ouput "plaintext:" without newline
        //then ouput converted "ciphertext: " without nl
        //from main, return 0

    //Get the key
    int msg = validation(keyc, keyv[1]); //message from validation

    if (msg == 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (msg == 2)
    {
        printf("Invalid key, must contain 26 characters.\n");
        return 1;
    }
    else if (msg == 3)
    {
        printf("Invalid key, must contain characters within alphabet.\n");
        return 1;
    }
    else
    {
        printf("working\n");
    }

    //Validate the key
        //check key length
        //check for non-alpha chars
        //check for repeated chars (case-sensitive)
    //Get plaintext
        //use get_string
    //Encipher
        //determine what letter it maps to
        //preserve case
        //leave non-alpha char as is
    //Print ciphertext

    //Output 1, invalid key: no key
    //Output 2, invalid key: more than one key
    //Output 3, invalid key: not 26 chars
    //Output 4, invalid key: not alpha char
    //Output 5, invalid key: char used more than once
    //Output 0, valid key: conversion
}

int validation(int arg, string key)
{
    int msg = 0; //message output
    int len = strlen(key); //string length
    int alp = 0; //check if all chars are in alphabet
    int keyc = 0; //individual characters

    for (int i = 0; i < 26; i++) //alphabet checker
    {
        keyc = isalpha((int)key[i]);
    }

    if (arg != 2) //not one input
    {
        msg = 1;
        return msg;
    }
    else if (len != 26) //not 26 characters
    {
        msg = 2;
        return msg;
    }
    else if (alp > 0) //not alpha char
    {
        msg = 3;
        return msg;
    }
    else
    {
        return msg;
    }

}