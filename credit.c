#include <stdio.h>
#include <cs50.h>

long luhn_chk(void);

int main (void)
{
    long credit = luhn_chk();
    string card = "INVALID";

    if(credit < 4000000000000 || credit >= 5600000000000000) //eliminate out of scope
    {
        card = "INVALID";
    }
    else if(credit < 5000000000000000 && credit >= 4000000000000000) //VISA check 1
    {
        card = "VISA";
    }
    else if(credit < 5000000000000 && credit >= 4000000000000) //VISA check 2
    {
        card = "VISA";
    }
    else if(credit < 380000000000000 && credit >= 370000000000000) //AMEX check 1
    {
        card = "AMEX";
    }
    else if(credit < 350000000000000 && credit >= 340000000000000) //AMEX check 2
    {
        card = "AMEX";
    }
    else if(credit < 5600000000000000 && credit >= 5100000000000000) //MASTER check
    {
        card = "MASTERCARD";
    }

    printf("%s\n", card);
}

long luhn_chk(void)
{
    long credit, luhn;
    int sum, num1, num2;

    do //preliminary filter to see if specific to either credit cards
    {
        credit = get_long("Number: "); //user prompt
    }
    while (credit < 0 || credit > 9999999999999999); //prelim bounds

    luhn = credit;
    sum = 0;

    do //checking Luhn's principle
    {
        num1 = luhn % 10;
        luhn = luhn / 10;
        num2 = luhn % 10;
        luhn = luhn / 10;
        sum = sum + num1 + 2 * num2;
    }
    while(luhn > 0);

    sum %= 10;

    if(sum != 0)
    {
        credit = 0;
    }
    printf("%li", credit);
    return credit;
}
