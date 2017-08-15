/**
 * Declares a dictBit's functionality.
 * 
 * Note: bit here colloquially means "a little bit of a dictionary"
 *       as opposed to a literal binary value
 */

#ifndef DICTBIT_H
#define DICTBIT_H

#include <stdbool.h>

/**
 * each dictBit is a struct with an array of pointers to child dictBit nodes
 */
typedef struct dictBit
{
    bool ends_word;
    struct dictBit *children[27];
}
dictBit;

/**
 * Adds a bit to current dictBit and returns a pointer to the newly created bit
 */
dictBit *addBit(dictBit *my_bit, char c);

/**
 * examine a single dictBit node
 */
void seeBit(dictBit *currBit);

/**
 * create a blank dictBit node
 */
dictBit *newBit(void);

/**
 * free a dictBit node (and makes a call to kill all children dictBit nodes)
 * returns true if parent dictBit ends up empty
 */
bool killKidBits(dictBit *currBit);

/**
 * return true if dictBit has no children
 */
bool isEmptyBit(dictBit *currBit);

/**
 * returns a=0 through z=25 and also (apostrophe) '=26
 * for use as indexes for children dictBit nodes
 */
int charToIndex(char c);

#endif // DICTBIT_H
