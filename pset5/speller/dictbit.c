/**
 * Declares a dictBit's functionality.
 * 
 * Note: bit here colloquially means "a little bit of a dictionary"
 *       as opposed to a literal binary value
 */
 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "dictbit.h"

/**
 * returns a=0 through z=25 and also (apostrophe) '=26
 * for use as indexes for children dictBit nodes
 * return -1 for newline character
 */
int charToIndex(char c)
{
    // Find which index to go to, or if we are at an end of word return NULL
    switch (c)
    {
        case '\'':
            return 26;
            break;
        case '\n':
            return -1;
            break;
    }
    
    // per instructions, we can assume dictionary is valid
    // no need to check isalpha() so just return and go
    return toupper(c) - 65;
}

/**
 * Adds a bit to current dictBit and returns a pointer to the newly created bit
 */
dictBit *addBit(dictBit *currBit, char c)
{
    // set a variable for index for insertion
    int i = charToIndex(c);
        
    // handle end of word by allocating child and returning a NULL pointer
    if (i == -1)
    {
        currBit->ends_word = true;
        return NULL;
    }
    
    //printf("proceeding in addBit with index=%i for char='%c'\n", i, c);

    // if child bit doesn't exist yet, allocate it in memory
    if (currBit->children[i] == NULL)
    {
        currBit->children[i] = newBit();
    }
    
    // return next dictBit
    return currBit->children[i];
}

/**
 * create a blank dictBit node
 */
dictBit *newBit(void)
{
    dictBit *newBit = malloc(sizeof(dictBit));
    newBit->ends_word = false;
    for (int x = 0; x < 27; x++)
    {
        newBit->children[x] = NULL;
    }
    
    return newBit;
}

/**
 * examine a single dictBit node
 */
void seeBit(dictBit *currBit)
{
    //printf("Examining a dictBit: \n");
    
    // print what each key represents
    for(int x = 0; x < 26; x++)
    {
        printf(" %c", (char) x + 65);
    }
    printf(" '\n");
    
    // print values
    for (int x = 0; x < 27; x++)
    {
        printf(" %i", (currBit->children[x] == NULL)? 0: 1);
    }
    
    printf("\n");
}

/**
 * return true if dictBit has no children
 */
bool isEmptyBit(dictBit *currBit)
{
    for (int x = 0; x < 27; x++)
    {
        if(currBit->children[x] != NULL)
        {
            return false;
        }
    }
    
    return true;
}

/**
 * free a dictBit node (and makes a call to kill all children dictBit nodes)
 */
bool killKidBits(dictBit *currBit)
{
    //printf("Attempting to kill a bit that looks like this:\n");
    //seeBit(currBit);
    
    dictBit *myChild = NULL;
    for(int x = 0; x < 27; x++)
    {
        myChild = currBit->children[x];
        
        if(myChild != NULL)
        {
            //printf("killing a bit at index = %i\n", x);
            killKidBits(myChild);
            free(myChild);
        }
    }
    
    // should return empty dictBit with no children remaining
    return currBit;
}