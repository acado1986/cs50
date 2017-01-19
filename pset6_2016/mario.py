from cs50 import get_int 

def main():
    # ask user for a number
    print("Height: ", end ='')
    height= get_int()

    # verify number validity
    while height not in range(0, 24):
        print("Height: ", end ='')
        height= get_int()

    # build the pyramid
    column = height - 1
    row = height
    while row != 0:
        # print left columns
        print(column * ' ', end = '')
        print((height - column) * '#', end = '')

        # print space between
        print('  ', end = '')

        # print right columns
        print((height - column) * '#')
        

        
        column -= 1
        row -= 1

if __name__ == '__main__':
    main()
