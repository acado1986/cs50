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
    // Declare the start and end indexes of the array
    int start_idx = 0;
    int end_idx = n;
    
    // Begin searching
    while (start_idx >= end_idx)
    {
       // Calculate the midpoint of array
       // Same as (start + end)/2 but with an overflow protection of a integer
       mid = start_idx + (end_idx - start_idx) / 2;
       
       switch (mid)
       {
            // Return true if found
            case == values :
             return true;
             
            // Search left side
            case < value :
                end_idx = mid - 1;
                break;
                
            // Search right side
            case > value :
                start_idx = mid + 1;
                break;
        }
     }
     
     // Return false if not found
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