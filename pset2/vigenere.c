/* Bogdan Ioan Nechifor
 * November 10th 2016
 * The program will encrypt a message using Vinegere's cipher method 
 * It will ask the user for a message and with the string key provided 
 * in the command line argument will print a new  encrypted message
 * Usage : caesar <string key>
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
        printf("Usage : caesar <string key>\n");
        return 1;
    }
    
    // Save the key into a variable string
    char* key = argv[1];
    // Lenght of key string
    int key_len = strlen(key); 
    // Check if contains any non-alphabetical character
    for (int i =0; i < key_len; i++)
    {
        if(!isalpha(key[i]))
        {
            // Explain the proper usage and exit with error 1
            printf("Usage : caesar <string key>\n");
            return 1;
        }
    }

    // Container variable for the original message
    char* origin_msg;
    // Prompt the user to provide a message for encryption
    origin_msg = GetString();

    // Encrypt the message
    // Container variable for the encrypted message
    char encrypt_msg[50];

    int len = strlen(origin_msg);

    // Counter for the key string
    int j = 0;
    // Iterate trough the message
    for (int i = 0; i < len; i++) 
    {
        // Check if it is a letter
        if (isalpha(origin_msg[i]))
        {
            // Make sure the jth character of key to be a letter
            while (!isalpha(key[j % key_len]))
                j++;

            // Check if it is upper or lowecase
            if (islower(origin_msg[i]))
            {
                // Encrypt letter by adding to the original the key letter
                encrypt_msg[i] = (((origin_msg[i] + tolower(key[j % key_len])) - 2 * 'a') % 26) + 'a';
                // Increase key counter for the next letter
                j++;
            }else
            {
                // Encrypt letter by adding to the original the key letter
                encrypt_msg[i] = (((origin_msg[i] - 'A' + tolower(key[j % key_len])) - 'a') % 26) + 'A';
                // Increase key counter for the next letter
                j++;
            }
        }else
        {
            // Add the character without rotating
            encrypt_msg[i] = origin_msg[i];
        }
    }
    
    // End the encrypted with a null character
    encrypt_msg[len] = '\0';
    
    // Print the encrypted message to console
    printf("%s\n", encrypt_msg);
    return 0;
}
