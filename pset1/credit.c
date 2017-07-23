#include <stdio.h>
#include <cs50.h>
#include <math.h>

int digits_in_ll(long long ll);
bool pass_luhn_algo(long long ll);
int two_leading_digits(long long ll);
bool is_cc(long long cc);
bool is_known_cc(int digits, int lead);
void newline(void);

int main(void)
{
    printf("Number: ");
    long long cc = get_long_long();
    
    is_cc(cc);
    
    newline();
}

bool is_cc(long long cc)
{
    bool is_cc = pass_luhn_algo(cc);
    int digit_count = digits_in_ll(cc);
    int lead_digits = two_leading_digits(cc);
    
    //is_cc? printf("Could be a credit card number.") : printf("Is not a credit card number.");
    //newline();
    //printf("Has %i digits, leading two are %i.\n", digit_count, lead_digits);
    
    if(is_known_cc(digit_count, lead_digits))
    {
        return true;
    }
    else
    {
        printf("INVALID");
        return false;
    }
}

bool pass_luhn_algo(long long ll)
{
    int total = 0;
    bool odd = true;
    for (long long i = 1; i <= ll; i *= 10)
    {
        int digit = (ll / i) % 10;

        if (odd)
        {
            total += digit;
        }
        else
        {
            digit = digit * 2;
            if (digit < 10)
            {
                total += digit;
            }
            else
            {
                total += digit % 10;
                total += round(digit / 10);
            }
        }

        odd = !odd;
    }

    return (total % 10) == 0;
}

bool is_known_cc(int digits, int lead)
{
    switch(lead)
    {
        case 34:
        case 37:
            if (digits == 15)
            {
                printf("AMEX");
                return true;
            }
            break;
        
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            if (digits == 16)
            {
                printf("MASTERCARD");
                return true;
            }
            break;
        
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
            if (digits == 13 || digits == 16)
            {
                printf("VISA");
                return true;
            }
            break;
    }
    
    return false;
}

int two_leading_digits(long long ll)
{
    while (ll >= 100)
    {
        ll = round(ll / 10);
    }
    
    return (int) ll;
}

int digits_in_ll(long long ll)
{
    int digits = 0;
    for (long long i = 1; i <= ll; i *= 10)
    {
        digits++;
    }
    return digits;
}

void newline(void)
{
    printf("\n");
}