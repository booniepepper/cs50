// Output and helper data types
#include <stdio.h>
#include <cs50.h>

// String manipulation
#include <string.h>

// Crypt dependencies
#define _XOPEN_SOURCE
#include <unistd.h>

// Testing
#include <assert.h>


// Util
string get_salt_from_hash(string hash);
char to_char(int i);

// Encryption functions
char *crypt(const char *key, const char *salt);

// Decryption functions
string decrypt(string hash);
void test_decrypt(void);


int main (int argc, string argv[])
{
    // Requirement is to take one hash string of 13 (printable) ASCII characters
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
        // Run any tests
        test_decrypt();
        
        return 0;
    }
    else if (strlen(argv[1]) != 13)
    {
        // Give "how to use" prompt
        printf("Usage: ./crack hash\n");
        printf("hash should be a series of 13 printable ASCII characters (where the first two characters represent the salt itself)\n");
        printf("See: man crypt for some details\n");
        return 2;
    }
    
    string hash = argv[1];
    
    printf("%s\n", decrypt(hash));
}


// Given a hash, find salt and key and decrypt
string decrypt(string hash)
{
    string salt = get_salt_from_hash(hash);
    
    static char key[] = "\0\0\0\0";
    
    // Check up to length=4 strings, where a string is going to be checked as char array [a, b, c, d]
    // For negative values of a,b,c or d, set that character to \0
    for (int d = -1; d < 52; d++)
    {
        key[3] = to_char(d);
        
        for (int c = -1; c < 52; c++)
        {
            key[2] = to_char(c);
            
            for (int b = -1; b < 52; b++)
            {
                key[1] = to_char(b);
            
                for (int a = 0; a < 52; a++)
                {
                    key[0] = to_char(a);
                    
                    if(strcmp(crypt(key, salt), hash) == 0)
                    {
                        return key;
                    }
                }
            }
        }
    }
    
    return "No results found.";
}


// converts int to char with 0-25=A-Z and 26-51=a-z
char to_char(int i)
{
    if(0 <= i && i < 52)
    {
        return (i < 26? 65 : 71) + i;
    }
    
    return '\0';
}


// Return salt from a hash (salt will be first two characters of the hash)
string get_salt_from_hash(string hash)
{
    assert(strlen(hash) > 1);
    
    static char salt[2] = "ZZ";
    
    salt[0] = hash[0];
    salt[1] = hash[1];
    
    return salt;
}


// Test that decrypt works with known hash to password maps
void test_decrypt(void)
{
    assert(strcmp(decrypt("50fkUxYHbnXGw"), "rofl") == 0);
    
    printf("decrypt() seems to be working!\n");
}