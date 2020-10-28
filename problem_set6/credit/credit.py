from cs50 import get_string

def main():
    while True:
        number = get_string("Number: ")
        if int(number) >= 0:
            break
    if check_validity(number):
        print_credit_card_brand(number)
    else:
        print("INVALID")

# Hans Luhn's algorithm to check validity
def check_validity(number):
    checksum = 0
    for i in range(len(number)):
        if (i % 2 == 0):
            checksum += int(number[len(number) - i - 1])
        else:
            temp = int(number[len(number) - i - 1]) * 2
            checksum += temp // 10 + temp % 10
            #checksum += temp % 10

    return checksum % 10 == 0

# Prints credit card supplier depending on card number
def print_credit_card_brand(number):
    if (number[0:2] == "34" or number[0:2] == "37"):
        print("AMEX")
    elif (number[0] == "4"):
        print("VISA")
    else:
        print("MASTERCARD")

# statement to initiate main function
if __name__ == "__main__":
    main()