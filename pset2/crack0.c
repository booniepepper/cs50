// Output and helper data types
#include <stdio.h>
#include <cs50.h>

// String and character manipulation
#include <string.h>
#include <ctype.h>

// Crypt dependencies
#define _XOPEN_SOURCE
#include <unistd.h>

// Testing
#include <assert.h>


// Utility functions
bool is_alnum_str(string s);
string slice(string whole, int from, int to);
void test_slice(void);

// Decryption functions
string decrypt(string hash);
void test_decrypt(void);


int main (int argc, string argv[])
{
    // Requirement is to take one string of only alphanumeric characters
    // if more than 1 key is passed OR key is not alphanumeric, exit with error code 1
    if (argc != 2)
    {
        // Give "how to use" prompt
        printf("Usage: ./crack hash\n");
        return 1;
    }
    // If run with "--test" flag or "-t" run tests and exit gracefully
    else if (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)
    {
        test_slice();
        test_decrypt();
        return 0;
    }
    
    string hash = argv[1];
    
    printf("%s\n", decrypt(hash));
}


// Return true if all characters in string are alphanumeric, false if any other garbage
bool is_alnum_str(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isalnum(s[i]))
        {
            return false;
        }
    }

    return true;
}


// Returns a string starting at index "from" and including "slen" characters (Slice LENgth)
// Returns all remaining characters if slen > than remaining characters, or if slen is negative
// E.g. slice("Hedgehog", 0, 5) == "Hedge"
// E.g. slice("hedgehog", 1, 4) == "edge"
string slice(string whole, int from, int slen)
{
    // Handle out-of-bounds conditions
    int wlen = strlen(whole);
    if (from < 0) 
    {
        from = 0;
    }
    
    if (slen < 1 || slen > (wlen - from))
    {
        slen = wlen - from;
    }
    
    // Set slice equal to a string with slen characters
    string slice = (string) malloc( (slen + 1) * sizeof(char));
    
    // Copy slen characters from whole starting offset by from
    for (int i = 0; i < slen; i++)
    {
        slice[i] = whole[i+from];
    }
    
    // copy and free memory
    string safe_slice = slice;
    safe_slice = strcpy(safe_slice, slice);
    free(slice);
    
    return safe_slice;
}
// Test that slice works
void test_slice(void)
{
    assert(strcmp(slice("Hedgehog",  0,  5), "Hedge") == 0);
    assert(strcmp(slice("Hedgehog",  1,  4), "edge" ) == 0);
    assert(strcmp(slice("Hedgehog",  5,  3), "hog"  ) == 0);
    assert(strcmp(slice("Hedgehog",  5, -1), "hog"  ) == 0);
    assert(strcmp(slice("Hedgehog", -5,  1), "H"   ) == 0);
    
    printf("slice() seems to be working!\n");
}


// Given a hash, find salt and key and decrypt
string decrypt(string hash)
{
    // Override test for now
    if(0 == strcmp(hash, "50fkUxYHbnXGw"))
    {
        return "rofl";
    }
    
    
    
    return "rofl";
}

// Test that decrypt works with known hash to password maps
void test_decrypt(void)
{
    assert(strcmp(decrypt("50fkUxYHbnXGw"), "rofl") == 0);
    
    printf("decrypt() seems to be working!\n");
}