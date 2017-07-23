#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
// http://docs.cs50.net/problems/initials/more/initials.html

int main(void)
{
    // Get name from user. Per requiements, no input helper
    string name = get_string();
    
    // Get leading characters of each non-space sequence
    string initials = "";
    
    // Capitalize and output initials
    printf("input name: %s\n",name);
    printf("initials: %s\n", initials);
}

string pop_initial(string name);
{
    
}
