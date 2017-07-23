#include <stdio.h>
#include <cs50.h>
#include <math.h>

void print_min_coins(float amount);

int main(void)
{
    printf("O hai! ");
    float amount = -1.0;
    while (amount < 0)
    {
        printf("How much change is owed?\n");
        amount = get_float();
    }
    print_min_coins(amount);
}

void print_min_coins(float amount)
{
    int remaining_cents = round(amount * 100);
    
    int quarters = remaining_cents / 25;
    remaining_cents = remaining_cents % 25;
    
    int dimes = remaining_cents / 10;
    remaining_cents = remaining_cents % 10;
    
    int nickels = remaining_cents / 5;
    remaining_cents = remaining_cents % 5;
    
    printf("%i\n", quarters + dimes + nickels + remaining_cents);
}