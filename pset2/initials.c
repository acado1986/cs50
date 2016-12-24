/* This program will ask the user for 
 * a full name and will output their initials capitalized*/

#include <stdio.h>
#include "cs50.h"
#include <ctype.h>
int main(void)
{
    // Create a container variable for the user input
    char* full_name;

    // Ask the user for input and save it in the container
    full_name = GetString();
    
    // Itinerate the name and check for the first letter in name
    // Do-while loop because the first letter is always an initial
    int i = 0;
    do
    {
        // If previous position is not a letter and current position is
        // And only in this case it will print the letter
        if (!isalpha(full_name[i - 1]) && isalpha(full_name[i]))
        {
            // Capitalize if necesserary and print it out
            printf("%c", toupper(full_name[i]));
        }
        i++;
    }while (full_name[i] != '\0');
    printf("\n");
}
