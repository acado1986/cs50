/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// define size of a BYTE 
typedef uint8_t BYTE;

// define cluster block size
#define BLOCK 512 

int main()
{
    // open memory card file for reading
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open the file card.raw for read\n");
        return 1;
    }
    
    // create a buffer of 512 bytes to store a cluster block (FAT file system)
    BYTE buff[BLOCK];
    
    // variables for titles of the new files, ex. "###.jpg"
    char title[8];
    int file_num = -1;
    
    // define output file pointer outside the loop to close the last file
    FILE* outptr = NULL;
    
    // read until end of the file
    while (fread(&buff, sizeof(buff), 1, inptr) == 1)
    {
        // flag if a new jpeg is found
        bool newJpg = false;
        
        // check if a new jpg is found
        // check the first 3 bytes for pattern 0xFFD8FF and
        // the fourth value between 0xE0 and 0xEF
        if (buff[0] == 0xFF && buff[1] == 0xD8 && buff[2] == 0xFF && (buff[3] >= 0xE0 && buff[3] <= 0xEF))
        {
            newJpg = true;
            file_num++;
        }
            
        // open a new jpg for writing
        if (newJpg == true)
        {
            // close previous file if exists
            if ( outptr != NULL)
                fclose(outptr);
        
            // name of new file
            sprintf(title, "%03d.jpg", file_num); 
            
            // open file for append a new data
            outptr = fopen(title, "a");
            if (outptr == NULL)
            {
                printf("Could not open the file %s for write\n", title);
                return 2;
            }
        }
       
        // check to only write on a valid jpg file
        if (outptr != NULL)
            // write data to file
            fwrite(&buff, sizeof(buff), 1, outptr);
            
    }
        
    // close the memory card file
    fclose(inptr);
        
    // close remaining files
    if (outptr != NULL)
        fclose(outptr);
        
    // exit succesfully
    return 0;
}
