/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n < 0)
        return false;
    
    int index = n/2;
    int bottom = 0;
    int top = n - 1;
    int curr_value;
    
    while (bottom != top && bottom < top)
    {
        curr_value = values[index];
        
        printf("Looking for needle at haystack[%i]. Will %i=%i?\n", index, curr_value, value);
        
        if (value == curr_value)
            return true;
        else if (value < curr_value)
            top = index - 1;
        else if (value > curr_value)
            bottom = index + 1;
        
        index = (top - bottom) / 2 + bottom;
        
        if (top == bottom)
        {
            printf("Looking for needle at haystack[%i]. Will %i=%i?\n", index, values[index], value);
            if(value == values[index])
                return true;
        }
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    if (n < 0)
        return;
    
    // Try my hand at counting sort
    // To be compatible with generate, allocate max int size for array size
    static int counts[65536];
    // Needed to initialize values to 0? this is from string.h but works for arrays of ints too
    memset(counts, 0, 65536);
    
    int biggest = 0;
    
    // Count up the occurrences of each value in values[]
    for (int i = 0; i < n; i++)
    {
        int this_value = values[i];
        counts[this_value]++;
        
        if (this_value > biggest)
            biggest = this_value;
    }
    
    int index = 0;
    // Plop integers back into the values based on count.
    // Only go as high as the largest int found earlier
    for (int m = 0; m <= biggest; m++)
    {
        for (int n = 0; n < counts[m]; n++)
        {
            values[index] = m;
            index ++;
        }
    }
    
    return;
}
