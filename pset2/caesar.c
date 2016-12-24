/* Bogdan Ioan Nechifor
 * November 10th 2016
 * The program will encrypt a message using Caesar’s cipher method of encryption
 * It will ask the user for a message and with the key provided 
 * in the command line argument will print a new  encrypted message
 * Usage : caesar <key>
 */
#include <stdio.h>
#include "cs50.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    // Sanity check
    // Check if user typed an argument (key) for the program
    if (argc != 2)
    {
        // Explain the proper usage and exit with error 1
        printf("Usage : caesar <key>\n");
        return 1;
    }

    // Container variable for the original message
    char* origin_msg;
    // Prompt the user to provide a message for encryption
    origin_msg = GetString();

    // Using the ternary operator assign to integer key the argument string 
    // Print the original message if is not a valid non-negative integral number
    int key;
    (atoi(argv[1]) > 0) ? (key = atoi(argv[1])):printf("%s\n", origin_msg);
    
    // Encrypt the message
    // Container variable for the encrypted message
    char encrypt_msg[50];

    int len = strlen(origin_msg);
    // Iterate trough the message
    for (int i = 0; i <= len; i++)
    {
        // Check if it is a letter
        if (isalpha(origin_msg[i]))
        {
            // Check if it is upper or lowecase
            if (islower(origin_msg[i]))
            {
                // Rotate the letter by key and add it to encrypted message
                encrypt_msg[i] = ((origin_msg[i] - 'a' + key) % 26) + 'a';
            }else
            {
                // Rotate the letter by key and add it to encrypted message
                encrypt_msg[i] = ((origin_msg[i] - 'A' + key) % 26) + 'A';
            }
        }else
        {
            // Add the character without rotating
            encrypt_msg[i] = origin_msg[i];
        }
    }

    // Print the encrypted message to console
    printf("%s\n", encrypt_msg);
    return 0;
}
