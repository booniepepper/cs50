/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictbit.h"
#include "dictionary.h"

dictBit *dictRoot;

long wordCount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int len = strlen(word);
    
    //printf("Checking \"%s\"with length=%i\n", word, len);
    
    // Node to traverse
    dictBit *checkBit = dictRoot;
    
    // index storage for letter in word
    int i;
    
    // Check each letter in word
    for(int x = 0; x < len; x++)
    {
        i = charToIndex(word[x]);

        if (checkBit->children[i] != NULL)
        {
            checkBit = checkBit->children[i];
        }
        else
        {
            return false;
        }
    }

    if (checkBit != NULL && checkBit->ends_word == true)
    {
        return true;
    }
    
    return false;
    
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Load file that the dictionary (relative path) points to
    FILE *fd = fopen(dictionary, "r");
    if (fd == NULL)
    {
        return false;
    }
    
    // Initialize root
    dictRoot = newBit();
    
    // Create the "working dictBit"
    dictBit *workingBit = dictRoot;
    
    // Iterate through characters in dict file
    for (int c = fgetc(fd); c != EOF; c = fgetc(fd))
    {
        //printf("%c", (char) c);
        workingBit = addBit(workingBit, c);
        
        // At end of a word, switch to the next word
        if(workingBit == NULL)
        {
            workingBit = dictRoot;
            wordCount++;
        }
    }
    
    fclose(fd);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    bool unloaded = killKidBits(dictRoot);
    
    if(unloaded)
    {
        free(dictRoot);
        return true;
    }
    
    return false;
}