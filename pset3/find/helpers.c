/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Linear searching algorithm
    // If n is not a positive number return false
    if (n < 0)
    {
        return false;
    }else
    {
        // Search the value by iterating through each array element
        for ( int i = 0; i < n; i++)
        {
            if (value == values[i])
            {
                return true;
            }
        }
    }
    
    // Return false if value not found
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Insertion sort, an O(n^2) sorting algorithm
    // Iterate through the elements of the unsorted portion of the array
    for ( int i = 1; i < n - 1; i++)
    {
        // Store the current array element into a temporary value
        int temp_value = values[i];
        
        // Index for iterating through the already sorted portion
        int sorted_idx = i;
        
        // Compare the current element to the the sorted elements
        // Determine the position where to insert
        while ( sorted_idx > 0 && values[sorted_idx - 1] > temp_value)
        {
            values[sorted_idx] = values[sorted_idx - 1];
            sorted_idx = sorted_idx - 1;
        }
        
        // Store the element in the sorted portion of the array
        values[sorted_idx] = temp_value;
    }
}