import cs50

space = ' '
block = '#'

def main():
    height = -1
    while height < 0 or 23 < height:
        height = get_height()
    
    for i in range(height):
        sp = height - i - 1
        bsp = i + 1
        
        print(space * sp,  end='')
        print(block * bsp, end='')
        print(space * 2,   end='')
        print(block * bsp, end='')
        
        print()
        
def get_height():
    print('Height: ', end='')
    height = cs50.get_int()
    return height;
        
if __name__ == '__main__':
    main()