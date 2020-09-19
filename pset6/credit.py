from cs50 import get_string
from sys import exit


def main():

    # Get input from user
    number = get_string("Number: ")
    digits = len(number)

    # Check length is valid
    if digits < 13 or digits > 16 or digits == 14:
        print("INVALID")
        exit(1)

    # Luhn's algorithm
    if luhns(number) is False:
        print("INVALID")
        exit(1)

    # VISA or MASTERCARD
    if digits == 13 or digits == 16:
        # Use the number at the beginning to determine the type
        if number[0] == "4":
            print("VISA")
            exit(0)

        elif (int(number[:2]) > 50) and (int(number[:2]) < 56):
            print("MASTERCARD")
            exit(0)

        else:
            print("INVALID")
            exit(0)

    # AMEX
    elif digits == 15:
        # Check the beginning of the number is correct
        if number[:2] == "34" or number[:2] == "37":
            print("AMEX")
            exit(0)
        else:
            print("INVALID")
            exit(1)


# Function takes the number (as a string) as an input, and returns a boolean value
# which indicates whether the number is or is not a valid credit card number
def luhns(number):

    even_digits = []
    odd_digits = []

    for i in range(len(number)):
        # If even, append digit in int form to odds list
        if i % 2 == 0:
            odd_digits.append(int(number[len(number) - i - 1]))
        # If not, append to evens list
        else:
            even_digits.append(int(number[len(number) - i - 1]))

    # Multiply all entries in even_digits by 2
    even_digits = [i * 2 for i in even_digits]

    # If we have a two digit number, preemptively sum the digits
    for k in range(len(even_digits)):
        # Can be at most two digits
        if even_digits[k] > 9:
            even_digits[k] = 1 + (even_digits[k] - 10)

    # Now we can just sum over the two lists
    output = sum(even_digits) + sum(odd_digits)

    if output % 10 == 0:
        return True

    else:
        return False


main()

