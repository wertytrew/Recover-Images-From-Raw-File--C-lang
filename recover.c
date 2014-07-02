/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
typedef uint8_t  BYTE;
 
// prototype
int first4bytes (BYTE bufBytes[512]);

int main(void)
{
    // open card file
    FILE* inptr = fopen("card.raw", "r");
    
    if (inptr == NULL)
    {
        fclose(inptr);
        printf("Could not open file");
        return 1;
    }
    
    // sizes of
    BYTE block[512];
    char filename[8];
    
    // keep track of jpg starts
    int counter = 0;
    
    // create first new file to write to
    FILE* newImage = NULL;
    
    // read 512 bytes into a buffer - repeat until end of card
    while (fread(block, sizeof(BYTE), 512, inptr) == 512)
    {        
        // start of a new jpg
        if (first4bytes(block) == 1)
        {
            if (counter > 0)
            {
                // close any open file
                fclose(newImage);
            }
            
            // create a new file for next new jpg file
            sprintf(filename, "%03i.jpg", counter);
            newImage = fopen(filename, "w");
            fwrite(block, sizeof(BYTE), 512, newImage);
            counter ++;
        }
        
        // handle non jpg data from beginning of raw file and 1st jpg not yet found
        else if (first4bytes(block) == 0)
        {
            // if raw file contains non-jpg data before the first jpg has been found
            if (counter == 0)
            {
                continue;
            }
            
            else
            {
                fwrite(block, sizeof(BYTE), 512, newImage);
            }
        }
    }
    
    // close any remaining files
    fclose(newImage);
    fclose(inptr);
    
    // and there we have it
    return 0;
        
}

// test first 4 bytes to see if they match 1 of 2 jpg magic numbers
int first4bytes (BYTE blockBytes[512])
{
    if((blockBytes[0] == 255) && (blockBytes[1] == 216) && (blockBytes[2] == 255) && (blockBytes[3] == 224))
    {
        return 1;
    }
    
    else if((blockBytes[0] == 255) && (blockBytes[1] == 216) && (blockBytes[2] == 255) && (blockBytes[3] == 225))
    {
        return 1;
    }
    
    else
    {
        return 0;
    }
}
