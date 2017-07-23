#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

// http://docs.cs50.net/problems/initials/more/initials.html

int main(void)
{
    // Get name from user. Per requirements, don't show any input prompt
    // DO handle leading and trailing spaces
    // DON'T handle special characters e.g. .,'-
    string name = get_string();
    
    // used for traversing string, start at -1 to increment later
    int i = -1;
    
    // Print initials for all character groups separated by spaces
    do
    {
        char initial;
        
        // Find index of first non-space character
        do
        {
            i++;
            initial = name[i];
        }
        while (initial != '\0' && initial == ' ');
        
        // If string hasn't ended, print the initial as a capital
        if (initial != '\0')
        {
            printf("%c", toupper(initial));
        }
        
        // Skip ahead to next space or end of string
        while (name[i] != '\0' && name[i] != ' ')
        {
            i++;
        }
    }
    while (name[i] != '\0');
    
    // End and print a newline
    printf("\n");
}
