import crypt
# Usage: crypt.crypt(word, salt)

import sys
from string import ascii_uppercase, ascii_lowercase

# assumptions:
# - No longer than 4 characters
# - Only alphabetical characters
# - Can be upper/lowercase

def main():
    if len(sys.argv) != 2:
        print('Usage: python crack.py hash')
        exit(1)
    elif (len(sys.argv[1]) != 13):
        print('Usage: python crack.py hash')
        print('[Error] hash must be 13 characters long')
        exit(2)
    
    key = sys.argv[1]
    salt = get_salt(key)
    
    chars = list(ascii_lowercase + ascii_uppercase)
    chars.insert(0, '')
    
    for w in chars:
        for x in chars:
            for y in chars:
                for z in chars:
                    check(w+x+y+z, salt, key)

def get_salt(key):
    return key[0:2]

def check(word, salt, key):
    if crypt.crypt(word, salt) == key:
        print(word)
        exit(0)

if __name__ == '__main__':
    main()