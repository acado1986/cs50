/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Return error and usage if no arguments or optional seed input
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // Convert string argument into an integer
    int n = atoi(argv[1]);

    // Initialize a pseudo-random number generator with seed value for drand48() 
    if (argc == 3)
    {
        // Use the optional seed given in the arguments
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        // Use current time as seed number
        srand48((long int) time(NULL));
    }

    // Print n times a generated pseudo-random double caste into an int
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}