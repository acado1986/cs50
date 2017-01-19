import cs50

def main():
    # ask user for a card number
    print("Number: ", end ='')

    # force the user to input a calid number
    card_num = cs50.get_int()

    # transform int in string for checking
    card_num = repr(card_num)

    # verify card company name
    length = len(card_num)
    if length == 15 and (card_num[:2] == '34' or card_num[:2] == '37'):
        card_name = "Amex"
    elif length == 16 and (True if card_num[:2] in ["{:02d}".format(x) for x in range(51, 56)] else False):
        card_name = "MasterCard"
    elif (length == 13 or length == 16) and card_num[0] == '4':
        card_name = "Visa"
    else:
        print("Invalid")

        # exit the program
        raise SystemExit

    # verify validity with Luhn's algorithm
    # extract digits in two lists, one for multiplied digits
    # and the other for the rest
    second_digits = list(map(int, card_num[length-2::-2]))
    last_digits = list(map(int, card_num[length::-2]))

    # multiply by two the second to last digits
    mult_second_digits = [x * 2 for x in second_digits]

    # add each digit of the products
    mult_second_digits = [(((x % 10)) + 1 if x >= 10 else x) for x in mult_second_digits]

    # add the sum of all digits, the multiplied ones and the rest
    result = sum(mult_second_digits) + sum(last_digits)

    # transform number in string for checking the last digit
    result = repr(result)

    print(card_name if result[-1] == '0' else "Invalid")
if __name__ == '__main__':
    main()
