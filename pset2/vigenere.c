#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

bool is_alpha_str(string s);
int to_int(char c);
char shift_char(char c, int offset);

int main (int argc, string argv[])
{
    // Requirement is to take one string of only alphabet characters
    // if more than 1 key is passed OR key is not only alpha, exit gracefully
    if (argc != 2 || !is_alpha_str(argv[1]))
    {
        // Give "how to use" prompt
        printf("Usage: ./vigenere k\n");
        printf("k is required and can be any sequence of alpha characters, but no special characters\n");
        return 1;
    }
    
    string vkey = argv[1];
    //printf("Valid key, proceeding. (%s)\n", vkey);
    
    // Get plaintext from user
    printf("plaintext: ");
    string ptext = get_string();
    
    // Apply key to plaintext, pump into ctext
    string ctext = ptext;
    
    int key_offset = 0;
    
    for (int i = 0, j = 0; i < strlen(ptext); i++)
    {
        j = (i - key_offset) % strlen(vkey);
        if (isalpha(ptext[i]))
        {
            ctext[i] = shift_char(ptext[i], to_int(vkey[j]));
        }
        else
        {
            ctext[i] = ptext[i];
            key_offset++;
        }
    }
    
    // Show ciphertext to user
    printf("ciphertext: %s\n", ctext);
    
}


// Return true if all characters in string are alpha, false if any other garbage
bool is_alpha_str(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isalpha(s[i]))
        {
            return false;
        }
    }

    return true;
}


// Return a zero-aligned integer for a-z or A-Z with a=0, b=1, etc.
int to_int(char c)
{
    return (int) toupper(c) - 65;
}


// Shift character up by offset integers.
// - If character passes Z, roll-over back to A.
// - Preserve case
char shift_char(char c, int offset)
{
    bool lower = islower(c);
    c = toupper(c) + offset;
    
    if (c > 'Z')
    {
        c = (c % 'Z') + 'A' - 1;
    }
    
    if (lower)
    {
        c = tolower(c);
    }
    
    return c;
}