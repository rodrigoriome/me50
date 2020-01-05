from cs50 import get_int
import re

while True:
    card_number = get_int("Number: ")
    if card_number > 0:
        break

def main():
    if is_amex(card_number):
        print("AMEX")
    elif is_mastercard(card_number):
        print("MASTERCARD")
    elif is_visa(card_number):
        print("VISA")
    else:
        print("INVALID")

def sum_digits(number):
    accumulator = 0
    for i in range(len(str(number))):
        accumulator += int(str(number)[i])
    return accumulator

def validate_luhn(number):
    luhn_sum = 0

    for i in range(len(str(number))):
        if i % 2 == 0:
            luhn_sum += int(str(number)[::-1][i])
        else:
            luhn_sum += sum_digits(int(str(number)[::-1][i]) * 2)

    sum_last_digit = int(str(luhn_sum)[::-1][0])
    return True if sum_last_digit == 0 else False

def is_amex(number):
    if (len(str(number)) == 15 and
        re.search("^34|37", str(number)) and
        validate_luhn(number)):
        return True
    return False

def is_mastercard(number):
    if (len(str(number)) == 16 and
        re.search("^5(1|2|3|4|5)", str(number)) and
        validate_luhn(number)):
        return True
    return False

def is_visa(number):
    if (len(str(number)) == 16 or
        len(str(number)) == 13 and
        re.search("^4", str(number)) and
        validate_luhn(number)):
        return True
    return False

main()
