#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
typedef uint8_t BYTE;
#define SIZE 512

int main(int argc, char *argv[])
{
    //Initilization & opening card file
    if (argc > 2)
    {
        printf("Usage: ./recover image OR ./recover card.raw\n");
        return 1;
    }
    else if (fopen("card.raw", "r") == NULL)
    {
        printf("Unable to open card.raw.\n");
        return 1;
    }

    FILE *file = fopen("card.raw", "r"); //r stands for read mode

    //Pseudocode:
    //Repeat until end of card:
    //Read 512 bytes into a buffer
    //if start of new jpeg
    //if first jpeg, write first file
    //else, close file
    //Else
    //if already found jpeg
    //check & repeat
    //Close any remaining files


    //Look for beginning of a JPEG
    //each JPEG starts with distinct header:
    //first three bytes 0xff, 0xd8, 0xff
    //last byte anywhere from 0xe0 to 0xef
    //jpegs are stored back-to-back on memory card
    //start to look for JPEG header in each 512byte block
    //use fread(data, size, number, inptr)
    //fread(pointer, 512, tbd, and FILE * to read from)
    //jpeg? check buffer[0] == 0xff, buffer[1] == 0xd8, buffer[2] == 0xff
    //check buffer[3] using boolean:
    //buffer[3] == 0xe0 || ... || buffer[3] == 0xef
    //better trick is the 'bitwise and': (buffer[3] & 0xf0) == 0xe0
    //if a9ll conditions met, the file most likely jpeg
    bool jpeg = false; //condition if file is jpeg
    int i = 0; //counter
    FILE *img;
    char filename[8]; //based on that sprintf 8bit name gen
    BYTE buffer[SIZE];

    //Stop at end of file
    //use fread to figure out if you're at the end of the file
    //make a condition for the file size
    while (fread(buffer, SIZE, 1, file) != 0) //if 0, then no more file
    {
        //Look for beginning of a JPEG
        //each JPEG starts with distinct header:
        //first three bytes 0xff, 0xd8, 0xff
        //last byte anywhere from 0xe0 to 0xef
        //jpegs are stored back-to-back on memory card
        //start to look for JPEG header in each 512byte block
        //use fread(data, size, number, inptr)
        //fread(pointer, 512, tbd, and FILE * to read from)
        //jpeg? check buffer[0] == 0xff, buffer[1] == 0xd8, buffer[2] == 0xff
        //check buffer[3] using boolean:
        //buffer[3] == 0xe0 || ... || buffer[3] == 0xef
        //better trick is the 'bitwise and': (buffer[3] & 0xf0) == 0xe0
        //if a9ll conditions met, the file most likely jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Write 512 bytes until a new JPEG is found
            //FILE *img = fopen(filename, "w"), w for write
            //fwrite(data, size, number, outptr)
            //Open a new JPEG
            //to make it, Filenames: ###.jpg, starting at 000.jpg
            //sprintf(filename, "%03i.jpg", 2)
            //%03i is print 3 digits as integers, 2 is number you print (ie 002)
            jpeg = true;
            if (i == 0) //first image
            {
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                fwrite(buffer, SIZE, 1, img);
                i++;
            }
            else //all other images
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                fwrite(buffer, SIZE, 1, img);
                i++;
            }
        }
        else if (jpeg == true) //if already found jpeg
        {
            fwrite(buffer, SIZE, 1, img);
        }


    }

    fclose(img);
    fclose(file);
    return 0;
}
