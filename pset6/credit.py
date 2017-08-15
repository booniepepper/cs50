import cs50

def main():
    print('Number: ', end='')
    cc = cs50.get_int()
    
    cc_type = whose(cc)
    luhn_res = luhn(cc)

    if luhn_res != 0:
        print('Thought it was {}'.format(cc_type))
        print('Failed Luhn\'s algorithm with {}'.format(luhn_res))
        cc_type = 'INVALID'
    
    print(cc_type)

# return boolean value for whether the input cc
# (credit card number) passes luhn's algorithm
def luhn(cc):
    my_cc = str(cc)
    total = 0
    
    # traverse credit card number - will use i as iteration # but not index
    for i in range(len(my_cc)):
        # reverse order
        index = len(my_cc) - i - 1

        # pull out digit at index
        my_digit = int(my_cc[index])
        
        # set amount to add to the digit
        add = my_digit
        
        # on every other digit, double the amount, add digits (max 2)
        if i % 2 == 1:
            add *= 2
            add = add % 10 + add // 10
        
        #print('[Luhn] Total: {} Digit: {} Will Add: {}'.format(total, my_digit, add))
        total += add
    
    #print('[Luhn] Grand Total: {}'.format(total))
    return total % 10

# return a string value for the credit card owner
# based on number of digits and leading digits
def whose(cc):
    cc = str(cc)
    ln = len(cc)
    ld = int(cc[0:2])
    fd = int(cc[0:1])
    
    if ln == 15 and ld in [34, 37]:
        return 'AMEX'
    elif ln == 16 and ld in range(51,56):
        return 'MASTERCARD'
    elif ln in [13, 16] and fd == 4:
        return 'VISA'
    
    return 'INVALID'

if __name__ == '__main__':
    main()