from cs50 import get_string


def is_letter(word):
    new = ""
    for char in word:
        if char.isalpha():
            new += char
    return len(new)


def index(text):
    # number of words
    words = text.split()
    w = len(words)

    # number of letters & sentences
    l = 0
    s = 0
    for word in words:
        if '.' in word:
            word = word.split('.')
            word = word[0]
            s += 1
        elif '?' in word:
            word = word.split('?')
            word = word[0]
            s += 1
        elif '!' in word:
            word = word.split('!')
            word = word[0]
            s += 1
        l += is_letter(word)

    # calculation
    L = l / w * 100
    S = s / w * 100
    grade = int(round(0.0588 * L - 0.296 * S - 15.8, 0))
    
    # return
    if grade < 1:
        return "Before Grade 1"
    elif grade >= 16:
        return "Grade 16+"
    else:
        return "Grade " + str(grade)


print(index(get_string("Text: ")))
