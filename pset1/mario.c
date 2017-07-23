#include <cs50.h>
#include <stdio.h>

void newline(void);
void repeat_char(char c, int n);

int main(void)
{
    int height = -1;
    char block = '#';
    char space = ' ';

    // Get height of pyramid
    while (height < 0 || height > 23)
    {
        printf("Height: ");
        height = get_int();
    }
    
    // Display pyramid
    for (int i = height - 1; i >= 0; i--)
    {
        repeat_char(space, i);
        repeat_char(block, height - i);
        
        repeat_char(space, 2);
        
        repeat_char(block, height - i);
        
        newline();
    }
}

void repeat_char(char c, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}

void newline(void)
{
    printf("\n");
}