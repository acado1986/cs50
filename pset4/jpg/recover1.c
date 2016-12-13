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
#include "jpg.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover [filename]\n");
        return 1;
    }
    
    // remember filename
    char* infile = argv[1];
    
    /*close*/// open memory card file for reading
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open the file %s for read\n", infile);
        return 2;
    }
    
    // create a buffer of 512 bytes to store a cluster block (FAT file system) 
    BYTE buff[512];
    
    /*free*/// variables for titles of the new files, ex. "###.jpg"
    char title[8];
    int file_num = -1;
    
    // define output file pointer that will be opened later
    FILE* outptr = NULL;
    
    // read until end of the file
    while (fread(&buff, sizeof(buff), 1, inptr) == 1)
    {
        // flag if a new jpeg is found
        bool newJpg = false;
        
        // check if a new jpg is found
        for (int j = 0; j <= 508; j += 4)
        {
            if (buff[j] == 0xFF && buff[j+1] == 0xD8 && buff[j+2] == 0xFF)
            {
                // check if the fourth byte has the values between 0xe0 and 0xef
                if (buff[j+3] >= 0xE0 && buff[j+3] <= 0xEF)
                {
                    newJpg = true;
                    file_num++;
                    break;
                }
            }
        }
        
        // open a new jpg for writing
        if (newJpg == true)
        {
            // close previous file if exists
            if ( outptr != NULL)
                fclose(outptr);
        
            // name of new file
            sprintf(title, "%03d.jpg", file_num); 
            
            /*close*/// open file for append a new data
            outptr = fopen(title, "a");
            if (outptr == NULL)
            {
                printf("Could not open the file %s for write\n", title);
                return 3;
            }
        }
       
        // check if jpg exists
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
