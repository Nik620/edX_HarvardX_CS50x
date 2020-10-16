from cs50 import get_int


def validity(credit):
    # amex
    if (credit < 350000000000000 and credit >= 340000000000000) or (credit < 380000000000000 and credit > 350000000000000):
        return("AMEX")
    # mastercard
    elif credit < 5600000000000000 and credit >= 5100000000000000:
        return("MASTERCARD")
    # visa
    elif (credit < 5000000000000 and credit >= 4000000000000) or (380000000000000 and credit >= 370000000000000):
        return("VISA")
    # invalid
    else:
        return "INVALID"


print(validity(get_int("Number: ")))
